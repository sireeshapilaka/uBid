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
#include <util/StrUtils.h>
#include <util/ModeConstants.h>
#include <dao/AppBwReq.h>

using namespace omnetpp;
using namespace std;
using namespace strutils;

class UserAgent;

class Ue : public cSimpleModule {
public:
    Ue();
    virtual ~Ue();
    virtual void finish() override;
    void endCurrentActivityIfOngoing();
    void scheduleNextActivity();
    void handleFailure(AppAccessResponse* appAccessResponse);
    void startNextActivity();
    void processUAResponse(cMessage* msg);
    void sendRPIResponse(BidResponse* bidResponse);

    int expectedDurationActivity = 0;
    string myType;
    int totalBudget = -1;
    int numRound2Sessions = 0;
    int numRound1Sessions = 0;
    int numDataSessions = 0;
    cOutVector utilityPerAuction; // for the rounds that actually returns with a bundle
    cOutVector bidPerAuction; // for the rounds we actually get around to bidding in
    cOutVector breakStatusPerAuction; // For each scheduled rpi need, this is 1: if round1 returned no feasible RPIs, 2: if utility did not exceed threshold, 3: if bid was lost, 4: if bid was won
    cOutVector rpiDesiredAt;  // The value here doesnt matter; it only matters that we get the timestamp at this point
    cOutVector paymentPerRound2Won;
    cOutVector bRemProgressionPerRound2AllDays;
protected:
  virtual void initialize() override;
  virtual void handleMessage(cMessage *msg) override;
private:
  string users[21];
  string dates[7];
  cRNG* rng;
  string myAddress;
  bool addressSet = false;
protected:
  UserAgent* ua;
  std::multiset<ActivityDAO> activities;
  cMessage* startActivity = new cMessage("StartActivity");
  string appTrafficFileName;
  vector<AppBWReq*> rpisPerDay;
  int numOfAuctions;
  vector<int> startTimes;
  bool deterministic = true;
};

#endif /* CORENETWORK_NODES_UE_H_ */
