/*
 * UserAgentMC.cc
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#include <UserAgentMC.h>

UserAgentMC::UserAgentMC(Ue* containingUe, vector<AppBWReq*> rpisOfDay, int numOfAuctions) : UserAgent(containingUe, 1, rpisOfDay, numOfAuctions) {
    int i;
    numOfAuctions = 0;
    // Only realtime flows allowed
    for(i=0; i< numOfAuctions; i++) {
        AppBWReq* r = rpisOfDay[i];
        if(r->getActivityType()=="RealtimeVideo") {
            this->rpisOfDay.push_back(r);
            this->numAuctionsPerDay++;
        }
    }

    brem = ((UeMC* )(this->containingUe))->getDailybudget();
    currentAuction = 0;
}

UserAgentMC::~UserAgentMC() {
    // TODO Auto-generated destructor stub
}

void UserAgentMC::getReservedAccess() {
    if (this->networkAgent == NULL) {
        networkAgent = dynamic_cast<NetworkAgent*>(this->containingUe->getParentModule()->getSubmodule("networkAgent"));
        if (networkAgent == NULL) {
            throw cRuntimeError("Network Agent Module not found!");
        }
    }
//    // If no budget left, don't start anything
//    if(brem==0){
//        currentEvent = NULL;
//        handleBidRejection();
//        updateAuctionNum();
//        this->containingUe->breakStatusPerAuction.record(5);
//        return;
//    }


    // Get everything from Rpis list as per the num of auction
    AppBWReq* rpiOfInterest = rpisOfDay[currentAuction];
    ongoingActivity = rpiOfInterest->getActivityType();
    askingDownlinkThroughput = rpiOfInterest->getDlBandwidth();
    askingUplinkThroughput = rpiOfInterest->getUlBandwidth();
    askingDlDuration = rpiOfInterest->getDlDuration();
    askingUlDuration = rpiOfInterest->getUlDuration();
//    cout << "MC user negotiating for " << ongoingActivity << " at " << simTime()<<endl;

    AppBWReq* bwReq = new AppBWReq(askingUplinkBytes, askingDownlinkBytes, ongoingActivity, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput);
    currentEvent = new MCevent(currentAuction, new AppBWReq(askingUplinkBytes, askingDownlinkBytes, ongoingActivity, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput), brem);
    handleRPIResponse(networkAgent->getRPIs(bwReq));
}

void UserAgentMC::handleRPIResponse(list<AppBWRes*> rpis) {
//    cout << "MC user handle RPI response! "<< endl;
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
//        cout << "Bidding for "<< ongoingActivity << " at " << simTime() << endl;

        if(currentEvent == NULL) {
            throw cRuntimeError("currentevent is null in handleRPIResponse");
        }
        double thisUtility = getUtility(rpiOfInterest);
        currentEvent->setRes(new AppBWRes(rpiOfInterest->getDlDuration(), rpiOfInterest->getUlDuration(),
                rpiOfInterest->getDlBandwidth(), rpiOfInterest->getUlBandwidth()), thisUtility);
        this->containingUe->utilityPerAuction.record(thisUtility);

        if(brem==0){
            // currentEvent = NULL;
            // handleBidRejection();
            // updateAuctionNum();
            this->containingUe->breakStatusPerAuction.record(5);
            submitBid(rpis.front(), 0); // do not record the bid amount of 0 as this is just for oracle purposes
            // return;
        } else {
            double bidAmount = computeBid(rpiOfInterest->getDlBandwidth(), rpiOfInterest->getUlBandwidth());
            currentEvent->updateBidValue(bidAmount);
            this->containingUe->bidPerAuction.record(bidAmount);
            submitBid(rpis.front(), bidAmount);
            this->containingUe->numRound2Sessions++;
        }
    } else if (rpis.size() == 0 || (rpis.size() == 1 && rpis.front() == NULL)) {
        // Let the UE know this is infeasible
        this->containingUe->breakStatusPerAuction.record(1);
        handleBidRejection();
        updateAuctionNum();
    }
    if (rpis.size() > 1) {
        throw cRuntimeError("There can be no more than 1 RPI in the list!");
    }
}

void UserAgentMC::handleBidResponse(BidResponse* bidResult) {
    if (bidResult->isBidResult()) {// Won the bid
        this->containingUe->breakStatusPerAuction.record(4);
        double payment = bidResult->getPayment();
        brem -= payment;
        this->containingUe->paymentPerRound2Won.record(payment);
        this->containingUe->bRemProgressionPerRound2AllDays.record(brem);

//        cout << "MC user >>> bid win " << brem<< endl;
        if(currentEvent!=NULL)
            currentEvent->updateCp(bidResult->getPayment());

        // Inform the UE
        AppAccessResponse* response = new AppAccessResponse();
        response->setStatus(true);
        response->setActivityType(ongoingActivity);
        if (ongoingActivity == "RealtimeVideo") {
            response->setGrantedDownlinkThroughput(rpiDownlinkThroughput);
            response->setGrantedUplinkThroughput(rpiUplinkThroughput);
        } else {
            throw cRuntimeError("Unrecognized app type in UseragentMC!");
        }
        this->containingUe->processUAResponse(response);
    } else {
//        cout << "MC user >> bid win - lost" << endl;
        this->containingUe->paymentPerRound2Won.record(-1.0*bidResult->getPayment()); // Would-be critical payment used for oracle purposes only
        this->containingUe->breakStatusPerAuction.record(3);
        handleBidRejection();
    }
    updateAuctionNum();
    delete bidResult;
}

double UserAgentMC::computeBid(double* dl, double* ul) {
    if (this->rng == NULL) {
        rng = this->containingUe->getParentModule()->getRNG(0);
        if (rng == NULL) {
            throw cRuntimeError("No RNG found!");
        }
    }

    if (this->epsilonRng == NULL) {
        epsilonRng = this->containingUe->getParentModule()->getRNG(1);
        if (epsilonRng == NULL)
            throw cRuntimeError("No RNG (epsilon) found!");
    }

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

    int action;

    State* state = new State(currentAuction, this->brem, ulDelta, dlDelta, askingUlDuration, askingDlDuration);
    bool isStatePresent = false;
    auto stateSearched = statesVisited.find(state);
    epsilonInverse = 0;
    if(stateSearched!=statesVisited.end()) {
        // State exists
        state = stateSearched->first;
//        double dailybudget =((UeMC* )(this->containingUe))->getDailybudget();
//        epsilonInverse = (double(stateSearched->second)/double(dailybudget))+1;
        epsilonInverse = stateSearched->second;
        action = intuniform(epsilonRng, 1, epsilonInverse);
        isStatePresent = true;
    } else {
        // New state
        action = 1;
    }
//    cout << "Epsilon Inverse: " << epsilonInverse << " auction: " << currentAuction << " action " << action << endl;

    if(action==1) {
        int myBid = intuniform(rng, 1, brem);
        // Random action
        return myBid;
    } else {
        // Optimal action

        vector<int> bids;
        double maxQ = 0;

        if(isStatePresent) {
            // Iterate over the possible bids
            for(i=1; i<=brem; i++) {
            	// Check for the state action pair in the map
                StateActionPair* saOfInterest = new StateActionPair(state, i);
                auto searchSa = qTable.find(saOfInterest);
                if(searchSa!=qTable.end()) {
                    double currentQ = searchSa->second;
                    if(currentQ>maxQ) {
                        bids.clear();
                        bids.push_back(i);
                        maxQ = currentQ;
                    } else if(currentQ==maxQ) {
                        bids.push_back(i);
                    }
                }
            }
        }

        // No known optimal action, so picking random action
        if(bids.size()==0 || maxQ==0) {
            int myBid = intuniform(rng, 1, brem);
            return myBid;
        }

        // Exactly one optimal action
        if(bids.size()==1) {
            return bids[0];
        }

        // Picking one of the optimal actions at random
        int size = bids.size();
        int myBid = bids[intuniform(rng, 0, size-1)];
        return myBid;

    }
}

void UserAgentMC::submitBid(AppBWRes* rpi, double budget) {
    EV_DEBUG << "Submitting bid with budget " << to_string((int)budget) << endl;
    if (myName < 0) {
        myName = this->containingUe->getIndex();
        EV_DEBUG << "Index " << myName << endl;
        if (myName < 0) {
            throw cRuntimeError("UE Name cannot be empty");
        }
    }
    networkAgent->submitBid(this->containingUe->myType, myName, rpi, budget);
}

void UserAgentMC::updateAuctionNum() {
    if(currentEvent!=NULL) {
        episodeLog.push_back(currentEvent);

//        cout << "*****************" <<endl;
//        AppBWReq* ask = currentEvent->getAsk();
//        if(ask==NULL)
//            cout << "NULL ask" << endl;
//        else
//            cout << "Logged " << ask->getActivityType() << " (" <<ask->getDlBandwidth()
//            << ", " << ask->getUlBandwidth() << ") - "
//            << currentEvent->getBid() << ", " <<currentEvent->getCp() << ", " << currentEvent->getUtility()<< endl;
//        cout << "*****************" << endl;

    }
    currentAuction++;
    if(currentAuction==numAuctionsPerDay) {
        qTableUpdate();
        // End of day
        brem = ((UeMC* )(this->containingUe))->getDailybudget();
        currentAuction = 0;
        // this->containingUe->bRemProgressionPerRound2AllDays.record(brem);
    }
}

void UserAgentMC::qTableUpdate() {
    int numOfEvents = episodeLog.size();
//    cout << "\n\nEnd of day update at "<< simTime() <<endl;
    int i;
    double rt = 0;
    for (i = numOfEvents-1; i>=0; i--) {
        MCevent* eve = episodeLog[i];
        AppBWRes* res = eve->getRes();
        AppBWReq* req = eve->getAsk();
        int ulDuration = 0;
        int dlDuration = 0;
        if(res!=NULL) {
            ulDuration = res->getUlDuration();
            dlDuration = res->getDlDuration();
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
        rt = eve->getUtility() + (rewardsDiscountRate*rt);
   
        // State visit update and use the same state for the sa pair as well
        State* prevState = new State(eve->getIndex(), eve->getBrem(), ulDelta, dlDelta, ulDuration, dlDuration);
        auto stateSearched = statesVisited.find(prevState);
        if(stateSearched!=statesVisited.end()) {
            // Exists
            prevState = stateSearched->first;
            int num = stateSearched->second;
            statesVisited.at(prevState) = num+1;
        } else {
            // New state
            statesVisited.insert(make_pair(prevState, 1));
        }

        // Create the key for the qTable map
        StateActionPair* sa = new StateActionPair(prevState, eve->getBid());

        auto search = qTable.find(sa);
        if(search!=qTable.end()) {
            // Key exists - Update
            sa = search->first;
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
//    cout << "MC events erased " << episodeLog.size() << endl;
}
