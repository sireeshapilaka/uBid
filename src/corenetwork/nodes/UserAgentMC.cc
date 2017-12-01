/*
 * UserAgentMC.cc
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#include <UserAgentMC.h>

UserAgentMC::UserAgentMC(Ue* containingUe) : UserAgent(containingUe, 1) {
    // TODO Auto-generated constructor stub

}

UserAgentMC::~UserAgentMC() {
    // TODO Auto-generated destructor stub
}

void UserAgentMC::getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) {
    if (this->networkAgent == NULL) {
        networkAgent = dynamic_cast<NetworkAgent*>(this->containingUe->getParentModule()->getSubmodule("networkAgent"));
        if (networkAgent == NULL) {
            throw cRuntimeError("Network Agent Module not found!");
        }
    }

    ongoingActivity = appType;
    // Initializing to 0
    askingUplinkThroughput = 0.0;
    askingDownlinkThroughput = 0.0;
    askingDlDuration = 0;
    askingUlDuration = 0;

    // Hardcoding values
    if(appType == "RealtimeVideo") {
        askingDownlinkThroughput = REALTIME_HIGH;
        askingUplinkThroughput = REALTIME_HIGH;
        askingDlDuration = 5;
        askingUlDuration = 5;
    } else if(appType == "Video") {
        askingDownlinkThroughput = VIDEO_HIGH;
        askingDlDuration = 5;
    } else {
        // Don't handle any other type
        AppAccessResponse* response = new AppAccessResponse();
        response->setStatus(false);
        response->setActivityType(ongoingActivity);
        this->containingUe->processUAResponse(response);
    }

    AppBWReq* bwReq = new AppBWReq(askingUplinkBytes, askingDownlinkBytes, appType, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput);
    handleRPIResponse(networkAgent->getRPIs(bwReq));
}

void UserAgentMC::handleRPIResponse(list<AppBWRes*> rpis) {
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

        double bidAmount = computeBid();
        submitBid(rpis.front(), bidAmount);

    } else if (rpis.size() == 0 || (rpis.size() == 1 && rpis.front() == NULL)) {
        // Let the UE know this is infeasible
        handleBidRejection();
    }
    if (rpis.size() > 1) {
        throw cRuntimeError("There can be no more than 1 RPI in the list!");
    }
}

void UserAgentMC::handleBidResponse(BidResponse* bidResult) {
    if (bidResult->isBidResult()) {// Won the bid
        //moneySpentAggregate += bidResult->getPayment();

        // TODO: Do something with the money!!


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
}

double UserAgentMC::computeBid() {
    // TODO: implement this
    return 5;
}
