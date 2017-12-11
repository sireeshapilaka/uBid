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

UserAgent::UserAgent(Ue* containingUe, double budgetPerSession, vector<AppBWReq*> rpis, int numOfAuctions) {
    this->containingUe = containingUe;
    this->budget = budgetPerSession;
    this->alpha = uaUtils::genAlpha();
    this->rpisOfDay = rpis;
    this->numAuctionsPerDay = numOfAuctions;
    this->totalDailyBudget = numOfAuctions*budgetPerSession;
    this->containingUe->bRemProgressionPerRound2AllDays.record(totalDailyBudget);
}

UserAgent::~UserAgent() {
    // TODO Auto-generated destructor stub
}

// TODO: POLICY!: Currently: Always bid on the highest RPI
void UserAgent::handleRPIResponse(list<AppBWRes*> rpis) {
    if (this->rng == NULL) {
        rng = this->containingUe->getParentModule()->getRNG(0);
        if (rng == NULL) {
            throw cRuntimeError("No RNG found!");
        }
    }
    if (rpis.size()  == 1 && rpis.front() != NULL) {
        AppBWRes* rpiOfInterest = rpis.front();

        rpiDownlinkThroughput = rpiOfInterest->getDlBandwidth();
        rpiUplinkThroughput = rpiOfInterest->getUlBandwidth();

        double utility = getUtility(rpiOfInterest);
        this->containingUe->utilityPerAuction.record(utility);
        if (utility >= utilityThreshold) {
            double trueValuation = utility*utilityScalingFactor;
            int bidAmount = min(trueValuation, budget + remainingBudgetFromLastAuction);
            this->containingUe->bidPerAuction.record(bidAmount);
            cout << "Utility for regular user is: " << utility << " and bid amount is "<< bidAmount <<  endl;
            this->containingUe->numRound2Sessions++;
            if(bidAmount!=0)
                submitBid(rpis.front(), bidAmount);
            else {
                // TODO: Check the record status
                this->containingUe->breakStatusPerAuction.record(2);
                delete rpis.front();
                handleBidRejection();
            }
        } else {
            this->containingUe->breakStatusPerAuction.record(2);
            delete rpis.front();
            handleBidRejection();
        }
    } else if (rpis.size() == 0 || (rpis.size() == 1 && rpis.front() == NULL)) {
        this->containingUe->breakStatusPerAuction.record(1);
        // Let the UE know this is infeasible
        handleBidRejection();
    }
    if (rpis.size() > 1) {
        throw cRuntimeError("There can be no more than 1 RPI in the list!");
    }
}

void UserAgent::submitBid(AppBWRes* rpi, double bid) {
    EV_DEBUG << "Submitting bid with budget " << to_string((int)bid) << endl;
    if (myName < 0) {
        myName = this->containingUe->getIndex();
        EV_DEBUG << "Index " << myName << endl;
        if (myName < 0) {
            throw cRuntimeError("UE Name cannot be empty");
        }
    }
    networkAgent->submitBid(this->containingUe->myType, myName, rpi, bid);
}

void UserAgent::handleBidResponse(BidResponse* bidResult) {
    if (bidResult->isBidResult()) {// Won the bid
        this->containingUe->breakStatusPerAuction.record(4);
        double payment = bidResult->getPayment();
        this->containingUe->paymentPerRound2Won.record(payment);
        remainingBudgetFromLastAuction = (remainingBudgetFromLastAuction + budget) - payment;
        moneySpentAggregate += payment;
        moneySpentToday += payment;
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
        this->containingUe->breakStatusPerAuction.record(3);
        handleBidRejection();
        remainingBudgetFromLastAuction += budget;
    }
    this->containingUe->bRemProgressionPerRound2AllDays.record(totalDailyBudget - moneySpentToday);
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
void UserAgent::getReservedAccess() {
    if (this->networkAgent == NULL) {
        networkAgent = dynamic_cast<NetworkAgent*>(this->containingUe->getParentModule()->getSubmodule("networkAgent"));
        if (networkAgent == NULL) {
            throw cRuntimeError("Network Agent Module not found!");
        }
    }

    if (currentAuction == 0 ){ // 1st Auction of the pseudo day
        moneySpentToday = 0;
        this->containingUe->bRemProgressionPerRound2AllDays.record(totalDailyBudget);
        remainingBudgetFromLastAuction = 0; // Money leftover from previous day doesnt rollover
    }

    AppBWReq* rpiOfInterest = rpisOfDay[currentAuction];
    ongoingActivity = rpiOfInterest->getActivityType();
    askingDownlinkThroughput = rpiOfInterest->getDlBandwidth();
    askingUplinkThroughput = rpiOfInterest->getUlBandwidth();
    askingDlDuration = rpiOfInterest->getDlDuration();
    askingUlDuration = rpiOfInterest->getUlDuration();

    currentAuction++;
    if(currentAuction==numAuctionsPerDay) {
        currentAuction=0;
    }
    AppBWReq* bwReq = new AppBWReq(askingUplinkBytes, askingDownlinkBytes, ongoingActivity, askingUlDuration, askingDlDuration, askingUplinkThroughput, askingDownlinkThroughput);
    handleRPIResponse(networkAgent->getRPIs(bwReq));
}

double UserAgent::getUtility(AppBWRes* rpi) {
    // TODO: There should be jitter penalty
    // Ignoring uplink
    double dlphisum = 0;
    double ulphisum = 0;
    int dlduration = rpi->getDlDuration();
    int ulduration = rpi->getUlDuration();
    if (dlduration==0 && ulduration==0) return 0;
    int i;
    string app = rpi->getActivityType();
    double dl_phi_asked = calcPhiApp(app, askingDownlinkThroughput);
    double ul_phi_asked = calcPhiApp(app, askingUplinkThroughput);

    for(i=0; i<dlduration; i++) {
        double phi_given = calcPhiApp(app, rpi->getDlBandwidth(i));
        double iTerm = phi_given/(1+dl_phi_asked-phi_given);
        dlphisum += iTerm;
    }
    if(dlduration!=0)
        dlphisum = dlphisum/dlduration;
    else
        dlphisum = 0;

    for(i=0; i<ulduration; i++) {
        double phi_given = calcPhiApp(app, rpi->getUlBandwidth(i));
        double iTerm = phi_given/(1+ul_phi_asked-phi_given);
        ulphisum += iTerm;
    }
    if(ulduration!=0)
        ulphisum = ulphisum/ulduration;
    else
        ulphisum = 0;

    double timeInMin = ((double)dlduration+ulduration)/60;
    double a = 1-(this->alpha);
    double tTerm = (pow(timeInMin,a))/a;

    return tTerm*(ulphisum + dlphisum);

}
