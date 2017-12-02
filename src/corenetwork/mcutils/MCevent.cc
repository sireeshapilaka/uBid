/*
 * MCevent.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <MCevent.h>

MCevent::MCevent(AppBWReq* rpi_ask, AppBWRes* rpi_res, int bid, int cp) {
    this->rpi_ask = rpi_ask;
    this->rpi_res = rpi_res;
    this->bid = bid;
    this->cp = cp;
}

MCevent::~MCevent() {
    // TODO Auto-generated destructor stub
}

