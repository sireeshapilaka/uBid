/*
 * UeMC.cc
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#include <UeMC.h>

Define_Module(UeMC);

UeMC::UeMC() {
}

int UeMC::getDailybudget() {
    return dailybudget;
}

void UeMC::initialize() {
    Ue::initialize();
    ua = new UserAgentMC(this, rpisPerDay, numOfAuctions);
}
