//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <NetworkAgent.h>
Define_Module(NetworkAgent)
NetworkAgent::NetworkAgent() {
    // Capacity in Kbps
    resourceManager.AddTower(25000, 25000);
}

NetworkAgent::~NetworkAgent() {
    // TODO Auto-generated destructor stub
}

void NetworkAgent::initialize() {
    env = new GRBEnv();
    // Schedule self-timer for clearing auction
    timeOfNextAuction = simTime() + 3;
    scheduleAt(timeOfNextAuction, new cMessage("Auction"));
}

void NetworkAgent::finish() {
    resourceManager.recordStats();
}

/**
 * returns the total value generated for the network if all winners pay what they bid
 */
double NetworkAgent::solveAuction(list<AppBWRes*> rpis, bool* decisions, simtime_t currentTime) {
    int numOfBids = rpis.size();
    if(numOfBids<=0) return 0;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    GRBModel model = GRBModel(*env);
    GRBVar* decisionVars = model.addVars(numOfBids, GRB_BINARY);
    list<AppBWRes*>::iterator iter;
    int longestDuration = 0;
    int i = 0;
    for (iter = rpis.begin(); iter != rpis.end(); iter++) {
        string s = "x_" + to_string(i);
        decisionVars[i].set(GRB_DoubleAttr_Obj, (*iter)->getBidAmount());
        decisionVars[i].set(GRB_StringAttr_VarName, s);
        i++;
        if ((*iter)->getActivityType() == "TCP" || (*iter)->getActivityType() == "Browser") {
            longestDuration = max(longestDuration, (*iter)->getUlDuration()+ (*iter)->getDlDuration());
        } else {
            longestDuration = max(longestDuration, max((*iter)->getUlDuration(), (*iter)->getDlDuration()));
        }
    }

    model.update();
    // Objective is to maximize revenue
    model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);

    // Constraints
    for (int d  = 1; d <= longestDuration; d++) {
        SimTime correspondingSimtime = SimTime(currentTime.dbl() + d);
        GRBLinExpr ulCapConstraint = 0;
        GRBLinExpr dlCapConstraint = 0;
        list<AppBWRes*>::iterator iter;
        int i = 0;
        double* ulCapacities = new double[numOfBids];
        double* dlCapacities = new double[numOfBids];
        for (iter = rpis.begin(); iter != rpis.end(); iter++) {
            string activityType = (*iter)->getActivityType();
            if (activityType == "Video" || activityType == "Audio") {
                // Only downlink traffic reservation
                if ((*iter)->getDlDuration() >= d) {
                    dlCapacities[i] = (*iter)->getDlBandwidth();
                } else {
                    dlCapacities[i] = 0;
                }
                ulCapacities[i] = 0;
            } else if (activityType == "RealtimeVideo") {
                if ((*iter)->getUlDuration() >= d) {
                    ulCapacities[i] = (*iter)->getUlBandwidth();
                } else {
                    ulCapacities[i] = 0;
                }
                if ((*iter)->getDlDuration() >= d) {
                    dlCapacities[i] = (*iter)->getDlBandwidth();
                } else {
                    dlCapacities[i] = 0;
                }
            } else { // Bursty traffic -> First schedule Uplink, then schedule Downlink
                if ((*iter)->getUlDuration() >= d) {
                    ulCapacities[i] = (*iter)->getUlBandwidth();
                    dlCapacities[i] = 0;
                } else {
                    ulCapacities[i] = 0;
                    if ((*iter)->getDlDuration() + (*iter)->getUlDuration() >= d) {
                        dlCapacities[i] = (*iter)->getDlBandwidth();
                    } else {
                        dlCapacities[i] = 0;
                    }
                }
            }
            i++;
        }
        ulCapConstraint.addTerms(ulCapacities, decisionVars, numOfBids);
        dlCapConstraint.addTerms(dlCapacities, decisionVars, numOfBids);

        double availableUplinkCapacity = 25000;
        if (resourceManager.ulCellRsrcAllocMapList->at(0)->find(correspondingSimtime) != resourceManager.ulCellRsrcAllocMapList->at(0)->end()) {
            availableUplinkCapacity -= resourceManager.ulCellRsrcAllocMapList->at(0)->at(correspondingSimtime);
        }

        double availableDownlinkCapacity = 25000;
        if (resourceManager.dlCellRsrcAllocMapList->at(0)->find(correspondingSimtime) != resourceManager.dlCellRsrcAllocMapList->at(0)->end()) {
            availableUplinkCapacity -= resourceManager.dlCellRsrcAllocMapList->at(0)->at(correspondingSimtime);
        }

        model.addConstr(dlCapConstraint, GRB_LESS_EQUAL, availableDownlinkCapacity, "Capacity");
        model.addConstr(ulCapConstraint, GRB_LESS_EQUAL, availableUplinkCapacity, "Capacity");
    }

    model.optimize();
    model.write("debug.lp");
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Time for optimization : " << duration;
    // TODO : REMOVE
    int optimstatus = model.get(GRB_IntAttr_Status);

    cout << "Optimization complete" << endl;
    double objval = 0;
    if (optimstatus == GRB_OPTIMAL) {
        objval = model.get(GRB_DoubleAttr_ObjVal);
        cout << "Optimal objective: " << objval << endl;
    } else if (optimstatus == GRB_INF_OR_UNBD) {
        throw cRuntimeError("Model is infeasible or unbounded");
    } else if (optimstatus == GRB_INFEASIBLE) {
        throw cRuntimeError("Model is infeasible");
    } else if (optimstatus == GRB_UNBOUNDED) {
        throw cRuntimeError("Model is unbounded");
    } else {
        cout << "Optimization was stopped with status = "
                << optimstatus << endl;
    }

    double totalPayment = 0;
    iter = rpis.begin();
    i = 0;
    while (iter != rpis.end()) {
        AppBWRes* bidOfInterest = *iter;
        decisions[i] = (decisionVars[i].get(GRB_DoubleAttr_X) == 1 ? true : false);
        if(decisions[i]) {
            totalPayment += bidOfInterest->getBidAmount();
        }
        iter++;
        i++;
    }

    delete [] decisionVars;
    delete [] model.getConstrs();

    return totalPayment;
}

void NetworkAgent::handleMessage(cMessage* msg) {
    resourceManager.clearElapsedEntries();

    // Clear Auction with bids so far, convey results
    int numOfBids = bidsForNextAuction.size();
    if (numOfBids <= 0) {
        scheduleAt((simtime_t)simTime() + 1, msg);
        return;
    }

    // Solve the auction
    bool* decisions = new bool[numOfBids];
    simtime_t currentTime = simTime();
    double totalPayment = solveAuction(bidsForNextAuction, decisions, currentTime);

    // Compute the prices to charge
    list<AppBWRes*>::iterator iter = bidsForNextAuction.begin();
    int i = 0;
    double* pricesToCharge = new double[numOfBids];

    while(iter !=bidsForNextAuction.end()) {
        AppBWRes* bidOfInterest = *iter;
        BidResponse* bidResponse = new BidResponse();
        bool decision = (decisions[i] == 1 ? true : false);
        if (decision) {
            // Run auction without this winner
            list<AppBWRes*> rpis;
            list<AppBWRes*>::iterator iter_temp = bidsForNextAuction.begin();
            while (iter_temp != bidsForNextAuction.end()) {
                AppBWRes* bid = *iter_temp;
                if(bid!=bidOfInterest)
                    rpis.push_back(bid);
                iter_temp++;
            }
            bool* decisions_temp = new bool[numOfBids-1];
            double paymentWithoutWinner = solveAuction(rpis, decisions_temp, currentTime);

            iter_temp = rpis.begin();
            while(iter_temp != rpis.end()) {
                rpis.erase(iter_temp++);
            }
            delete [] decisions_temp;

            //Price to charge this winner
            double price = (paymentWithoutWinner)-(totalPayment-bidOfInterest->getBidAmount());
            if(price<0) price = 0-price;
            pricesToCharge[i] = price;
        } else {
            pricesToCharge[i] = 0;
        }

        iter++;
        i++;
    }

    iter = bidsForNextAuction.begin();
    i = 0;
    while (iter != bidsForNextAuction.end()) {
        AppBWRes* bidOfInterest = *iter;
        BidResponse* bidResponse = new BidResponse();
        bool decision = (decisions[i] == 1 ? true : false);
        cout << "Decision for bid " << i << " is: " << decisions[i] << "("<<pricesToCharge[i]<< ")"<< endl;
        if (decision) {
            // Get corresponding bid
            bool startSameTime = false;
            if (bidOfInterest->getActivityType() == "RealtimeVideo") {
                startSameTime = true;
            }

            resourceManager.ReserveResources(bidOfInterest->getUlBandwidth(), bidOfInterest->getDlBandwidth(),
                    bidOfInterest->getUlDuration(), bidOfInterest->getDlDuration(), startSameTime, currentTime);
            // Find the corresponding UE and notify
            bidResponse->setBidResult(true);
            bidResponse->setPayment(pricesToCharge[i]);
        } else {
            // Find the corresponding UE and notify
            bidResponse->setBidResult(false);
        }

        Ue* ueOfInterest = dynamic_cast<Ue*>(getParentModule()->getSubmodule("ue", bidOfInterest->getUser()));
        if (ueOfInterest == NULL) {
            throw cRuntimeError("UE could not be found!");
        }
        ueOfInterest->sendRPIResponse(bidResponse);
        i++;
        bidsForNextAuction.erase(iter++);
    }

    //Free arrays
    delete [] decisions;
    delete [] pricesToCharge;

    // Schedule next auction event
    timeOfNextAuction = (simtime_t)currentTime + 3;
    scheduleAt(timeOfNextAuction, new cMessage("Auction"));
}

void NetworkAgent::submitBid(int ueId, AppBWRes* rpi, double budget) {

    if (rpi != NULL) {
        bool sameStartTime = (rpi->getActivityType() == "RealtimeVideo" ? true : false);
        resourceManager.ReserveResourcesFake(rpi->getUlBandwidth(), rpi->getDlBandwidth(), rpi->getUlDuration(), rpi->getDlDuration(),
                sameStartTime, simTime(), rpi->getActivityType());
        //TODO: not negotiating
        // rpis.push_back(appBwRes);
    }

    rpi->setBidAmount(budget);
    rpi->setUser(ueId);
    // Modify RPI to include budget
    bidsForNextAuction.push_back(rpi);
}

list<AppBWRes*> NetworkAgent::getRPIs(AppBWReq* appBwReq) {
    // Generate RPIs for this BW request and return
    list<AppBWRes*> rpis;
    string activityType = appBwReq->getActivityType();
    unsigned int downlinkSize = ((8*appBwReq->getDownlinkBytes()) < appBwReq->getDownlinkBytes() ? UINT_MAX: 8*appBwReq->getDownlinkBytes());
    unsigned int uplinkSize = ((8*appBwReq->getUplinkBytes()) < appBwReq->getUplinkBytes() ? UINT_MAX: 8*appBwReq->getUplinkBytes());
    delete appBwReq;

    AppBWRes* appBwRes = NULL;
    bool impossible = false;
    if (activityType == "Video") {
        double highestPossibleBandwidth = VIDEO_VHIGH*1000; // InKbps
        double duration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
        if (true) {
            highestPossibleBandwidth = VIDEO_HIGH*1000;
            duration = (double)(downlinkSize/(1000*highestPossibleBandwidth));
            if (true) {
                highestPossibleBandwidth = VIDEO_STD*1000;
                duration = (double)(downlinkSize/(1000.0*highestPossibleBandwidth));
                if (false) {
                    highestPossibleBandwidth = VIDEO_MEDIUM*1000;
                    duration = (double)(downlinkSize/(1000.0*highestPossibleBandwidth));
                    if (!resourceManager.isResourceAllocationFeasible(duration, highestPossibleBandwidth, 1)) {
                        highestPossibleBandwidth = VIDEO_LOW*1000;
                        duration = (double)(downlinkSize/(1000.0*highestPossibleBandwidth));
                        if (!resourceManager.isResourceAllocationFeasible(duration, highestPossibleBandwidth, 1)) {
                            impossible = true;
                        }
                    }
                }
            }
        }
   //     if (!impossible) {
            appBwRes = new AppBWRes();
            appBwRes->setDlBandwidth(highestPossibleBandwidth);
            appBwRes->setDlDuration(ceil(duration));
            appBwRes->setActivityType("Video");
    //    }
    } else if (activityType == "Audio") {
        double highestPossibleBandwidth = AUDIO_HIGH;
        double duration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
        if (true) {
            highestPossibleBandwidth = AUDIO_STD;
            duration = (double)(downlinkSize/(1000.0*highestPossibleBandwidth));
            if (false) {
                highestPossibleBandwidth = AUDIO_LOW;
                duration = (double)(downlinkSize/(1000.0*highestPossibleBandwidth));
                if (!resourceManager.isResourceAllocationFeasible(duration, highestPossibleBandwidth, 1)) {
                    impossible = true;
                }
            }
        }
    //    if (!impossible) {
            appBwRes = new AppBWRes();
            appBwRes->setDlBandwidth(highestPossibleBandwidth);
            appBwRes->setDlDuration(ceil(duration));
            appBwRes->setActivityType("Audio");
    //    }
    } else if (activityType == "RealtimeVideo") {
        double highestPossibleBandwidth = REALTIMEHD_HIGH;
        double dlDuration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
        double ulDuration = (double)uplinkSize/(1000.0*highestPossibleBandwidth);
        bool isDownlink = downlinkSize > 0 ? true : false;
        bool isUplink = uplinkSize > 0 ? true : false;
        if (true) {
            highestPossibleBandwidth = REALTIMEHD_LOW;
            double dlDuration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
            double ulDuration = (double)uplinkSize/(1000.0*highestPossibleBandwidth);
            if (true) {
                highestPossibleBandwidth = REALTIME_HIGH;
                double dlDuration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
                double ulDuration = (double)uplinkSize/(1000.0*highestPossibleBandwidth);
                if (false) {
                    highestPossibleBandwidth = REALTIME_LOW;
                    double dlDuration = (double)downlinkSize/(1000.0*highestPossibleBandwidth);
                    double ulDuration = (double)uplinkSize/(1000.0*highestPossibleBandwidth);
                    if (!(resourceManager.isResourceAllocationFeasible(dlDuration, highestPossibleBandwidth, 1)) &&
                                   (resourceManager.isResourceAllocationFeasible(ulDuration, highestPossibleBandwidth, 0))) {
                        impossible = true;
                    }
                }
            }
        }
   //     if (!impossible) {
            appBwRes = new AppBWRes();
            if (isDownlink) {
                appBwRes->setDlBandwidth(highestPossibleBandwidth);
                appBwRes->setDlDuration(ceil(dlDuration));
            }
            if (isUplink) {
                appBwRes->setUlBandwidth(highestPossibleBandwidth);
                appBwRes->setUlDuration(ceil(ulDuration));
            }
            appBwRes->setActivityType("RealtimeVideo");
    //    }
    }
    if (appBwRes != NULL) {
        bool sameStartTime = (appBwRes->getActivityType() == "RealtimeVideo" ? true : false);
        resourceManager.ReserveResourcesFake(appBwRes->getUlBandwidth(), appBwRes->getDlBandwidth(), appBwRes->getUlDuration(), appBwRes->getDlDuration(),
                sameStartTime, simTime(), activityType);
        //TODO: not negotiating
        // rpis.push_back(appBwRes);
    }
    return rpis;
}


