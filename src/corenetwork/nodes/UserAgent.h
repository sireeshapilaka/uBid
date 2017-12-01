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

#ifndef CORENETWORK_NODES_USERAGENT_H_
#define CORENETWORK_NODES_USERAGENT_H_

#include <omnetpp.h>
#include <string>
#include <dao/BidResponse.h>
#include <dao/AppAccessResponse.h>
#include <dao/AppBwReq.h>
#include <dao/AppBwRes.h>
#include <corenetwork/Negotiation/NetworkAgent.h>
#include <corenetwork/nodes/Ue.h>
#include <util/ModeConstants.h>
#include <util/UaUtils.h>

using namespace std;
using namespace omnetpp;
using namespace uaUtils;

class UserAgent {
public:
    UserAgent(Ue* ue, double budgetPerSession);
    virtual ~UserAgent();
    virtual void getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink);
    void handleBidRejection();
    virtual void handleRPIResponse(list<AppBWRes*> rpis);
    virtual void handleBidResponse(BidResponse* bidResult);
    void submitBid(AppBWRes* rpi, double budget);
    double getUtility(AppBWRes* rpi);

private:
    // Ongoing = currently under negotiation for
    double budget;
    unsigned int rpiDownlinkBytes = 0;
    unsigned int rpiUplinkBytes = 0;
    double moneySpentAggregate = 0;
    int myName = -1;
    cRNG* rng = NULL;
    double alpha = 0;

protected:
    Ue* containingUe;
    NetworkAgent* networkAgent = NULL;
    string ongoingActivity;
    unsigned int askingDownlinkBytes = 0;
    unsigned int askingUplinkBytes = 0;
    double askingDownlinkThroughput = 0;
    double askingUplinkThroughput = 0;
    int askingDlDuration = 0;
    int askingUlDuration = 0;
    double *rpiDownlinkThroughput = NULL;
    double *rpiUplinkThroughput = NULL;
    int rpiDownlinkDuration = 0;
    int rpiUplinkDuration = 0;

};

#endif /* CORENETWORK_NODES_USERAGENT_H_ */
