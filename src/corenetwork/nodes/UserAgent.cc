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

#include <UserAgent.h>

UserAgent::UserAgent(Ue* containingUe, double budgetPerSession) {
    this->containingUe = containingUe;
    this->budget = budgetPerSession;
}

UserAgent::~UserAgent() {
    // TODO Auto-generated destructor stub
}

// TODO: POLICY!: Currently: Always bid on the highest RPI
void UserAgent::handleRPIResponse(list<AppBWRes*> rpis) {
    EV_DEBUG << "Received RPI response" << endl;
    bool bidding = true;
    if (rpis.size()  == 1 && rpis.front() != NULL) {
        AppBWRes* rpiOfInterest = rpis.front();
        if (ongoingActivity == "Video" || ongoingActivity == "Audio") {
            rpiDownlinkThroughput = rpiOfInterest->getDlBandwidth();
        } else if (ongoingActivity == "RealtimeVideo") {
            rpiDownlinkThroughput = rpiOfInterest->getDlBandwidth();
            rpiUplinkThroughput = rpiOfInterest->getUlBandwidth();
        } else {
            throw cRuntimeError("Unrecognized App Type in handling RPI Response");
        }

        if (bidding) {
            // Currently bidding is always true; but we may later want to do some utility-based decision for which RPI to pick, if at all
            double bidAmount = intuniform(rng, 1, budget);;
            submitBid(rpis.front(), bidAmount);
        } else {
            delete rpis.front();
        }
    } else if (rpis.size() == 0 || (rpis.size() == 1 && rpis.front() == NULL)) {
        // Let the UE know this is infeasible
        handleBidRejection();
    }
    if (rpis.size() > 1) {
        throw cRuntimeError("There can be no more than 1 RPI in the list!");
    }
}

void UserAgent::submitBid(AppBWRes* rpi, double budget) {
    EV_DEBUG << "Submitting bid with budget " << to_string((int)budget) << endl;
    if (myName < 0) {
        myName = this->containingUe->getIndex();
        EV_DEBUG << "Index " << myName << endl;
        if (myName < 0) {
            throw cRuntimeError("UE Name cannot be empty");
        }
    }
    networkAgent->submitBid(myName, rpi, budget);
}

void UserAgent::handleBidResponse(BidResponse* bidResult) {
    if (bidResult->isBidResult()) {// Won the bid
        cout << "Bid amount " << bidResult->getPayment();
        cout << endl;
        moneySpentAggregate += bidResult->getPayment();
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
        handleBidRejection();
    }
    delete bidResult;
}

void UserAgent::handleBidRejection() {
    // Just inform the UE
    AppAccessResponse* response = new AppAccessResponse();
    response->setStatus(false);
    response->setActivityType(ongoingActivity);
    // All params are set to 0 by default

    this->containingUe->processUAResponse(response);
}

// Current policy/default: Always ask for the STD in all classes
void UserAgent::getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) {
    if (this->networkAgent == NULL) {
        networkAgent = dynamic_cast<NetworkAgent*>(this->containingUe->getParentModule()->getSubmodule("networkAgent"));
        if (networkAgent == NULL) {
            throw cRuntimeError("Network Agent Module not found!");
        }
    }
    if (this->rng == NULL) {
        rng = this->containingUe->getParentModule()->getRNG(0);
        if (rng == NULL) {
            throw cRuntimeError("No RNG found!");
        }
    }

    ongoingActivity = appType;
    askingDownlinkBytes = downlinkSize;
    askingUplinkBytes = uplinkSize;
    // Initializing to 0
    askingUplinkThroughput = 0.0;
    askingDownlinkThroughput = 0.0;
    askingDlDuration = 0;
    askingDlDuration = 0;

    if (appType == "Browser" || appType == "TCP") { // Browser or TCP  - basically file download/upload
        // Do not negotiate for Bursty Trafficl; Just send a bid
        if (downlinkSize > 0 && desiredDurationDownlink == 0) {
            desiredDurationDownlink = ceil((downlinkSize*8.0)/1000000.0); //Desired duration @ rate of 1Mbps
        }
        askingDlDuration = desiredDurationDownlink;
        if (uplinkSize > 0 && desiredDurationUplink == 0) {
            desiredDurationUplink = ceil((uplinkSize*8.0)/75000.0);// Desired duration uplink @ rate of 75Kbps
        }
        askingUlDuration = desiredDurationUplink;
        double bidAmount = intuniform(rng, 1, budget);
        rpiUplinkDuration = desiredDurationUplink;
        rpiDownlinkDuration = desiredDurationDownlink;
        AppBWRes* rpi = new AppBWRes();
        rpi->setActivityType(appType);
        // Duration specified in seconds
        rpi->setDlDuration(desiredDurationDownlink);
        rpi->setUlDuration(desiredDurationUplink);
        // Bandwidth specified in Kbps
        rpi->setDlBandwidth(1000);
        rpi->setUlBandwidth(75);
        submitBid(rpi, bidAmount);
    } else {
        AppBWReq* bwReq = new AppBWReq(askingUplinkBytes, askingDownlinkBytes, appType);
        //TODO: not negoitating
        networkAgent->getRPIs(bwReq);
        // Sent RPI request to the Network


        //TODO: not negoitating
        //handleRPIResponse(networkAgent->getRPIs(bwReq));
    }
}
