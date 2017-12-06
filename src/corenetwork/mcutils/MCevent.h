/*
 * MCevent.h
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#ifndef DAO_MCEVENT_H_
#define DAO_MCEVENT_H_

#include <dao/AppBwRes.h>
#include <dao/AppBwReq.h>

class MCevent {
private:
    AppBWReq* rpi_ask = NULL;
    AppBWRes* rpi_res = NULL;
    int bid = -1; // -1 indicates not placing a bid
    int cp = -1; // -1 indicates auction lost
    double utility = 0;
    int brem = 0;
    int eveIndex;
public:
    MCevent(int eveIndex, AppBWReq* rpi_ask, int brem);
    virtual ~MCevent();
    void setRes(AppBWRes* res, double u);
    void updateBidValue(int bid);
    void updateCp(int cp);
    AppBWReq* getAsk();
    AppBWRes* getRes();
    int getBid();
    int getCp();
    double getUtility();
    int getBrem();
    int getIndex();
};

#endif /* DAO_MCEVENT_H_ */
