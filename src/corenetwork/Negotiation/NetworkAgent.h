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

#ifndef CORENETWORK_NEGOTIATION_NETWORKAGENT_H_
#define CORENETWORK_NEGOTIATION_NETWORKAGENT_H_

#include <omnetpp.h>
#include <string.h>
#include "corenetwork/Negotiation/ResourceManager.h"
#include "gurobi_c++.h"
#include <algorithm>    // std::max
#include <dao/BidResponse.h>
#include <corenetwork/nodes/Ue.h>
#include <chrono>
#include <AppBwReq.h>
#include <AppBwRes.h>

using namespace omnetpp;
using namespace std;
using namespace std::chrono;


class NetworkAgent : public cSimpleModule {
public:
    NetworkAgent();
    virtual ~NetworkAgent();
    list<AppBWRes*> getRPIs(AppBWReq* appBwReq);
    void receiveBid(AppBWReq* appBwReq);
    virtual void initialize();
    void submitBid(int ueId, AppBWRes* rpi, double budget);
    void handleMessage(cMessage* message);
    void finish();
private:
    ResourceManager resourceManager;

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

    list<AppBWRes*> bidsForNextAuction;
    GRBEnv* env;
    simtime_t timeOfNextAuction;
};

#endif /* CORENETWORK_NEGOTIATION_NETWORKAGENT_H_ */
