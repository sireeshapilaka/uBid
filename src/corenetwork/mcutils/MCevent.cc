/*
 * MCevent.cc
 *
 *  Created on: Dec 1, 2017
 *      Author: sireeshapilaka
 */

#include <MCevent.h>

MCevent::MCevent(int eveIndex, AppBWReq* rpi_ask, int brem) {
    this->eveIndex = eveIndex;
    this->rpi_ask = rpi_ask;
    this->brem = brem;
}

MCevent::~MCevent() {
    // TODO Auto-generated destructor stub
}

void MCevent::updateBidValue(int bid) {
    this->bid = bid;
}

void MCevent::updateCp(int cp) {
    this->cp = cp;
}

void MCevent::setRes(AppBWRes* res, double u) {
    this->rpi_res = res;
    this->utility = u;
}

AppBWReq* MCevent::getAsk() {
    return rpi_ask;
}

AppBWRes* MCevent::getRes() {
    return rpi_res;
}

int MCevent::getBid() {
    return bid;
}

int MCevent::getCp() {
    return cp;
}

double MCevent::getUtility() {
    // If auction lost, the value is 0
    if(cp==-1) return 0;
    return utility;
}

int MCevent::getBrem() {
    return brem;
}

int MCevent::getIndex() {
    return eveIndex;
}
