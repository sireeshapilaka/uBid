/*
 * UserAgentMC.cc
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#include <UserAgentMC.h>

UserAgentMC::UserAgentMC(Ue* containingUe, vector<AppBWReq*> rpisOfDay, int numOfAuctions) : UserAgent(containingUe, 1) {
    this->rpisOfDay = rpisOfDay;
    this->numAuctionsPerDay = numOfAuctions;
    brem = ((UeMC* )(this->containingUe))->getDailybudget();
    currentAuction = 0;
}

UserAgentMC::~UserAgentMC() {
    // TODO Auto-generated destructor stub
}

void UserAgentMC::getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) {
    cout << "MC user negotiating for " << appType << " at " << simTime()<<endl;
    if (this->networkAgent == NULL) {
        networkAgent = dynamic_cast<NetworkAgent*>(this->containingUe->getParentModule()->getSubmodule("networkAgent"));
        if (networkAgent == NULL) {
            throw cRuntimeError("Network Agent Module not found!");
        }
    }

    // Get everything from Rpis list as per the num of auction
    AppBWReq* rpiOfInterest = rpisOfDay[currentAuction];
    ongoingActivity = rpiOfInterest->getActivityType();
    askingDownlinkThroughput = rpiOfInterest->getDlBandwidth();
    askingUplinkThroughput = rpiOfInterest->getUlBandwidth();
    askingDlDuration = rpiOfInterest->getDlDuration();
    askingUlDuration = rpiOfInterest->getUlDuration();

    AppBWReq* bwReq = new AppBWReq(askingUplinkBytes, askingDownlinkBytes, appType, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput);
    currentEvent = new MCevent(new AppBWReq(askingUplinkBytes, askingDownlinkBytes, appType, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput), brem);
    handleRPIResponse(networkAgent->getRPIs(bwReq));
}

void UserAgentMC::handleRPIResponse(list<AppBWRes*> rpis) {
    cout << "MC user handle RPI response! "<< endl;
    if (rpis.size()  == 1 && rpis.front() != NULL) {
        AppBWRes* rpiOfInterest = rpis.front();

        ongoingActivity = rpiOfInterest->getActivityType();

        if (ongoingActivity == "Video" || ongoingActivity == "Audio") {
            rpiDownlinkThroughput = rpiOfInterest->getDlBandwidth();
        } else if (ongoingActivity == "RealtimeVideo") {
            rpiDownlinkThroughput = rpiOfInterest->getDlBandwidth();
            rpiUplinkThroughput = rpiOfInterest->getUlBandwidth();
        } else {
            throw cRuntimeError("Unrecognized App Type in handling RPI Response");
        }
        cout << "Bidding for "<< ongoingActivity << " at " << simTime() << endl;

        double bidAmount = computeBid(rpiOfInterest->getDlBandwidth(), rpiOfInterest->getUlBandwidth());
        if(currentEvent!=NULL) {
            currentEvent->setRes(new AppBWRes(rpiOfInterest->getDlDuration(), rpiOfInterest->getUlDuration(),
                    rpiOfInterest->getDlBandwidth(), rpiOfInterest->getUlBandwidth()), getUtility(rpiOfInterest));
            currentEvent->updateBidValue(bidAmount);
        }
        submitBid(rpis.front(), bidAmount);

    } else if (rpis.size() == 0 || (rpis.size() == 1 && rpis.front() == NULL)) {
        // Let the UE know this is infeasible
        handleBidRejection();
        updateAuctionNum();
    }
    if (rpis.size() > 1) {
        throw cRuntimeError("There can be no more than 1 RPI in the list!");
    }
}

void UserAgentMC::handleBidResponse(BidResponse* bidResult) {
    if (bidResult->isBidResult()) {// Won the bid
        brem -=bidResult->getPayment();

        cout << "MC user >>> bid win " << brem<< endl;
        if(currentEvent!=NULL)
            currentEvent->updateCp(bidResult->getPayment());

        // Inform the UE
        AppAccessResponse* response = new AppAccessResponse();
        response->setStatus(true);
        response->setActivityType(ongoingActivity);
        if (ongoingActivity == "Video" || ongoingActivity == "Audio") {
            response->setGrantedDownlinkThroughput(rpiDownlinkThroughput);
        } else if (ongoingActivity == "RealtimeVideo") {
            response->setGrantedDownlinkThroughput(rpiDownlinkThroughput);
            response->setGrantedUplinkThroughput(rpiUplinkThroughput);
        } else { //Bursty Traffic
            // TODO We need to maintain uplink and downlink durations differently
            // However, we are not going to change anything for bursty traffic, just monitoring whether
            // the duration we receive is as promised.
            response->setGrantedUplinkDuration(rpiUplinkDuration);
            response->setGrantedDownlinkDuration(rpiDownlinkDuration);
        }
        this->containingUe->processUAResponse(response);
    } else {
        cout << "MC user >> bid win - lost" << endl;
        handleBidRejection();
    }
    updateAuctionNum();
}

double UserAgentMC::computeBid(double* dl, double* ul) {
    if (this->rng == NULL) {
        rng = this->containingUe->getParentModule()->getRNG(0);
        if (rng == NULL) {
            throw cRuntimeError("No RNG found!");
        }
    }

    // Using a constant epsilon
    // TODO: Change this to 1/n
    if (this->epsilonRng == NULL) {
        epsilonRng = this->containingUe->getParentModule()->getRNG(1);
        if (epsilonRng == NULL)
            throw cRuntimeError("No RNG (epsilon) found!");
    }

    int action = intuniform(epsilonRng, 1, epsilonInverse);
    cout << "MC user action picked "<< action << endl;
    if(action==1) {
        // Random action
        cout << "MC user placing random bid" << endl;
        return intuniform(rng, 1, brem);
    } else {
        // Optimal action
        int* ulDelta;
        int* dlDelta;
        if(askingDlDuration!=0)
            dlDelta = new int[askingDlDuration];
        if(askingUlDuration!=0)
            ulDelta = new int[askingUlDuration];
        int i;
        for(i=0; i<askingDlDuration; i++) {
            dlDelta[i] = askingDownlinkThroughput-dl[i];
        }
        for(i=0; i<askingUlDuration; i++) {
            ulDelta[i] = askingUplinkThroughput-ul[i];
        }

        State* s = new State(this->brem, ulDelta, dlDelta, askingUlDuration, askingDlDuration);
        vector<int> bids;
        double maxQ = 0;

        map<StateActionPair*, double>::iterator iter;
        for(iter=qTable.begin(); iter!=qTable.end(); iter++) {
            StateActionPair* saOfInterest = iter->first;
            double currentQ = iter->second;
            // I get list of actions, if 1, choose.. If more, choose from them
            if(saOfInterest->isState(s)) {
                if(currentQ>maxQ) {
                    bids.clear();
                    bids.push_back(saOfInterest->getAction());
                } else if(currentQ==maxQ) {
                    bids.push_back(saOfInterest->getAction());
                }
            }
        }

        // No known optimal action, so picking random action
        if(bids.size()==0)
            return intuniform(rng, 1, brem);

        // Exactly one optimal action
        if(bids.size()==1)
            return bids[0];

        // Picking one of the optimal actions at random
        int size = bids.size();
        return bids[intuniform(rng, 0, size-1)];

    }
}

void UserAgentMC::submitBid(AppBWRes* rpi, double budget) {
    EV_DEBUG << "Submitting bid with budget " << to_string((int)budget) << endl;
    if (myName < 0) {
        myName = this->containingUe->getIndex() + 50000;
        EV_DEBUG << "Index " << myName << endl;
        if (myName < 0) {
            throw cRuntimeError("UE Name cannot be empty");
        }
    }
    networkAgent->submitBid(myName, rpi, budget);
}

void UserAgentMC::updateAuctionNum() {
    if(currentEvent!=NULL) {
        episodeLog.push_back(currentEvent);

        cout << "*****************" <<endl;
        AppBWReq* ask = currentEvent->getAsk();
        if(ask==NULL)
            cout << "NULL ask" << endl;
        else
            cout << "Logged " << ask->getActivityType() << " (" <<ask->getDlBandwidth()
            << ", " << ask->getUlBandwidth() << ") - "
            << currentEvent->getBid() << ", " <<currentEvent->getCp() << ", " << currentEvent->getUtility()<< endl;
        cout << "*****************" << endl;

    }
    currentAuction++;
    if(currentAuction==numAuctionsPerDay) {
        qTableUpdate();
        // End of day
        brem = ((UeMC* )(this->containingUe))->getDailybudget();
        currentAuction = 0;
    }
}

void UserAgentMC::qTableUpdate() {
    int numOfEvents = episodeLog.size();
    cout << "End of day update for "<<numOfEvents << " events"<<endl;
    int i;
    double rt = 0;
    for (i = numOfEvents-1; i>=0; i--) {
        MCevent* eve = episodeLog[i];
        AppBWRes* res = eve->getRes();
        AppBWReq* req = eve->getAsk();
        int ulDuration = 0;
        int dlDuration = 0;
        if(res!=NULL) {
            res->getUlDuration();
            res->getDlDuration();
        }

        // Compute delta for each auction
        int* ulDelta = NULL;
        int* dlDelta = NULL;
        if(ulDuration!=0)
            ulDelta = new int[ulDuration];
        if(dlDuration!=0)
            dlDelta = new int[dlDuration];
        int j;

        double askUl = req->getUlBandwidth();
        double askDl = req->getDlBandwidth();
        for(j=0; j<ulDuration; j++) {
            ulDelta[j] = askUl - res->getUlBandwidth(j);
        }
        for(j=0; j<dlDuration; j++) {
            dlDelta[j] = askDl - res->getDlBandwidth(j);
        }

        // Discounted reward
        rt = eve->getUtility() + (gamma*rt);

        // Create the key in the map
        State* prevState = new State(eve->getBrem(), ulDelta, dlDelta, ulDuration, dlDuration);
        StateActionPair* sa = new StateActionPair(prevState, eve->getBid());

        auto search = qTable.find(sa);
        if(search!=qTable.end()) {
            // Key exists - Update
            double val = search->second;
            double newVal = val + learningRate*(rt-val);
            qTable.at(sa) = newVal;
        } else {
            // New key
            qTable.insert(make_pair(sa, rt));
        }

    }

    // Empty the episode log
    episodeLog.clear();
    cout << "MC events erased " << episodeLog.size() << endl;
}
