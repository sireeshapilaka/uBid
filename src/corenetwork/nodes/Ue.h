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

#ifndef CORENETWORK_NODES_UE_H_
#define CORENETWORK_NODES_UE_H_

#include <omnetpp.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <dao/ActivityDAO.h>
#include <dao/AppAccessResponse.h>
#include <dao/BidResponse.h>
#include <INETDefs.h>
#include <HttpUtils.h>

using namespace omnetpp;
using namespace std;
using namespace inet;
using namespace httptools;

class UserAgent;

class Ue : public cSimpleModule {
public:
    Ue();
    virtual ~Ue();
    void setTcpDone();
    virtual void finish() override;
    void endCurrentActivityIfOngoing();
    void scheduleNextActivity();
    void handleFailure(AppAccessResponse* appAccessResponse);
    void startNextActivity();
    void processUAResponse(cMessage* msg);
    void sendRPIResponse(BidResponse* bidResponse);

    int expectedDurationActivity = 0;
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

protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
  bool isAnyApplicationActive();
private:
  UserAgent* ua;
  string lastActiveAppType;
  int numTCPApps;
  string appTrafficFileName;
  string users[21];
  string dates[7];
  cRNG* rng;
  std::multiset<ActivityDAO> activities;
  cMessage* startActivity = new cMessage("StartActivity");
  bool tcpOn = false;
  bool udpOn = false;
  string myAddress;
  bool addressSet = false;
  int numSessions = 0;
  int numRequestedSession = 0;
  int numTCPVideoSessions = 0;
  int numRequestedTCPVideoSessions = 0;
  int numTCPAudioSessions = 0;
  int numRequestedTCPAudioSessions = 0;
  int numBrowserSessions = 0;
  int numRequestedBrowserSessions = 0;
  int numUDPSessions = 0;
  int numRequestedUDPSessions = 0;
};

#endif /* CORENETWORK_NODES_UE_H_ */
