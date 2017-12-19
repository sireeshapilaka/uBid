/*
 * UserAgentMC.h
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#ifndef CORENETWORK_NODES_USERAGENTMC_H_
#define CORENETWORK_NODES_USERAGENTMC_H_

#include <omnetpp.h>
#include <UserAgent.h>
#include <mcutils/State.h>
#include <mcutils/StateActionPair.h>
#include <mcutils/MCevent.h>
#include <UeMC.h>

class UserAgentMC: public UserAgent {
private:
    int brem = 0;
    vector<MCevent*> episodeLog;
    struct saPairComp {
        bool operator() (StateActionPair* sa1, StateActionPair* sa2) const {
            return sa1->isLess(sa2);
        }
    };
    map<StateActionPair*, double, saPairComp> qTable;

    struct stateComp {
        bool operator() (State* s1, State* s2) const {
            return s1->isLess(s2);
        }
    };
    map<State*, int, stateComp> statesVisited;

    MCevent* currentEvent;
    double rewardsDiscountRate = 1;
    double learningRate = 0.01;
    double epsilonInverse;
    cRNG* epsilonRng = NULL;

public:
    UserAgentMC(Ue* containingUe, vector<AppBWReq*> rpisOfDay, int numOfAuctions);
    virtual ~UserAgentMC();
    void getReservedAccess() override;
    void handleRPIResponse(list<AppBWRes*> rpis) override;
    void handleBidResponse(BidResponse* bidResult) override;
    double computeBid(double* dl, double* ul);
    void submitBid(AppBWRes* rpi, double budget) override;
    void handleEndOfEpisode();
    void updateAuctionNum();
    void qTableUpdate();
};

#endif /* CORENETWORK_NODES_USERAGENTMC_H_ */
