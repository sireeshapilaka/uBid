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

using namespace std;
using namespace omnetpp;

class UserAgent {
public:
    UserAgent(Ue* ue, double budgetPerSession);
    virtual ~UserAgent();
    void getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink);
    void handleBidRejection();
    void handleRPIResponse(list<AppBWRes*> rpis);
    void handleBidResponse(BidResponse* bidResult);
    void submitBid(AppBWRes* rpi, double budget);

private:
    Ue* containingUe;
    // Ongoing = currently under negotiation for
    double budget;
    string ongoingActivity;
    unsigned int askingDownlinkBytes = 0;
    unsigned int askingUplinkBytes = 0;
    double askingDownlinkThroughput = 0;
    double askingUplinkThroughput = 0;
    int askingDlDuration = 0;
    int askingUlDuration = 0;

    unsigned int rpiDownlinkBytes = 0;
    unsigned int rpiUplinkBytes = 0;
    double rpiDownlinkThroughput = 0;
    double rpiUplinkThroughput = 0;
    int rpiDownlinkDuration = 0;
    int rpiUplinkDuration = 0;
    double moneySpentAggregate = 0;

    int myName = -1;
    NetworkAgent* networkAgent = NULL;
    cRNG* rng = NULL;

    // Definitions
    // in Mbps
    const double VIDEO_LOW = .5;
    const double VIDEO_MEDIUM = 1.5;
    const double VIDEO_STD = 3;
    const double VIDEO_HIGH = 5;
    const double VIDEO_VHIGH = 8;

    // In Kbps
    const double AUDIO_LOW = 64;
    const double AUDIO_STD = 96;
    const double AUDIO_HIGH = 120;

    // In Realtime Kbps
    const double REALTIME_LOW = 400;
    const double REALTIME_HIGH = 500;
    const double REALTIMEHD_LOW = 1200;
    const double REALTIMEHD_HIGH = 1500;
};

#endif /* CORENETWORK_NODES_USERAGENT_H_ */
