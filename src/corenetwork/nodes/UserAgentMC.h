/*
 * UserAgentMC.h
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#ifndef CORENETWORK_NODES_USERAGENTMC_H_
#define CORENETWORK_NODES_USERAGENTMC_H_

#include <UserAgent.h>

class UserAgentMC: public UserAgent {
public:
    UserAgentMC(Ue* containingUe);
    virtual ~UserAgentMC();
    void getReservedAccess(string appType, unsigned int downlinkSize, unsigned int uplinkSize, int desiredDurationUplink, int desiredDurationDownlink) override;
    void handleRPIResponse(list<AppBWRes*> rpis) override;
    void handleBidResponse(BidResponse* bidResult) override;
    double computeBid();
};

#endif /* CORENETWORK_NODES_USERAGENTMC_H_ */
