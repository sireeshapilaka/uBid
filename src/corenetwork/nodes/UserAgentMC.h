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
    int numAuctionsPerDay = 0;
    int currentAuction = 0;
    vector<MCevent*> episodeLog;
    struct comp {
        bool operator() (StateActionPair* sa1, StateActionPair* sa2) const {
            return sa1->isEqual(sa2);
        }
    };

    map<StateActionPair*, double, comp> qTable;
    vector<AppBWReq*> rpisOfDay;
    MCevent* currentEvent;
    double gamma = 0.5;
    double learningRate = 0.01;
    double epsilonInverse = 4;
    cRNG* epsilonRng = NULL;

public:
    UserAgentMC(Ue* containingUe, vector<AppBWReq*> rpisOfDay, int numOfAuctions);
    virtual ~UserAgentMC();
    void getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) override;
    void handleRPIResponse(list<AppBWRes*> rpis) override;
    void handleBidResponse(BidResponse* bidResult) override;
    double computeBid(double* dl, double* ul);
    void submitBid(AppBWRes* rpi, double budget) override;
    void handleEndOfEpisode();
    void updateAuctionNum();
    void qTableUpdate();
};

#endif /* CORENETWORK_NODES_USERAGENTMC_H_ */
