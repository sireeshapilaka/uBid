/*
 * UeMC.cc
 *
 *  Created on: Nov 30, 2017
 *      Author: sireeshapilaka
 */

#include <UeMC.h>

Define_Module(UeMC);

UeMC::UeMC() {
    //Asking rpis - Hardcoding them
    numOfAuctions = 3;
    AppBWReq* req = new AppBWReq(0,0,"RealtimeVideo", 5, 5, REALTIME_HIGH, REALTIME_HIGH);
    rpisPerDay.push_back(req);
    startTimes[0] = 4;
    req = new AppBWReq(0,0, "RealtimeVideo", 5, 5, REALTIME_LOW, REALTIME_LOW);
    rpisPerDay.push_back(req);
    startTimes[1] = 10;
    req = new AppBWReq(0,0, "Video", 0, 5, 0, VIDEO_STD);
    rpisPerDay.push_back(req);
    startTimes[2] = 20;
}

UeMC::~UeMC() {
    if(!rpisPerDay.empty()) {
        vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
        while(iter!=rpisPerDay.end()) {
            rpisPerDay.erase(iter++);
        }
    }
    if(startTimes!=NULL)
        delete[] startTimes;
}

void UeMC::initialize() {
    // Initialize the ua
    ua = new UserAgentMC(this, rpisPerDay, numOfAuctions);

    int day, auction;

    for(day=0; day<10; day++) {
        // Dummy uplink and downlink values since we are populating deterministic throughputs in UA
        vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
        for(auction=0;auction<numOfAuctions;auction++) {
            // Our day is 2 min long
            int startTime = day*120+startTimes[auction] ;
            ActivityDAO newActivity(startTime, 10, 10, (*iter)->getActivityType());
            activities.insert(newActivity);
            iter++;
        }
    }

    // Schedule 1st activity
    ActivityDAO firstAc = *activities.begin();
    int startTime = firstAc.getStartTimeInSeconds();
    scheduleAt((simtime_t)startTime, startActivity);
}

int UeMC::getDailybudget() {
    return dailybudget;
}

