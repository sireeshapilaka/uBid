/*
 * UserAgentMC.h
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#ifndef CORENETWORK_NODES_USERAGENTMC_H_
#define CORENETWORK_NODES_USERAGENTMC_H_

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
    map<StateActionPair*, double> qTable;
    vector<AppBWReq*> rpisOfDay;

public:
    UserAgentMC(Ue* containingUe, vector<AppBWReq*> rpisOfDay, int numOfAuctions);
    virtual ~UserAgentMC();
    void getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) override;
    void handleRPIResponse(list<AppBWRes*> rpis) override;
    void handleBidResponse(BidResponse* bidResult) override;
    double computeBid();
    void submitBid(AppBWRes* rpi, double budget) override;
    void handleEndOfEpisode();
    void updateAuctionNum();
};

#endif /* CORENETWORK_NODES_USERAGENTMC_H_ */
