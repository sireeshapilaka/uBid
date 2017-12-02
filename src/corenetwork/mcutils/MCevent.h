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
    int bid = 0;
    int cp = -1;
public:
    MCevent(AppBWReq* rpi_ask, AppBWRes* rpi_res, int bid, int cp);
    virtual ~MCevent();
};

#endif /* DAO_MCEVENT_H_ */
