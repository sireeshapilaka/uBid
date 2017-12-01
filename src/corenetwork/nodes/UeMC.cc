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

UeMC::~UeMC() {
}

void UeMC::initialize() {
    // Initialize the ua
    ua = new UserAgentMC(this);

    int i;

    int startTime = 4;
    for(i=0; i<10; i++) {
        // Dummy uplink and downlink values since we are populating deterministic throughputs in UA
        //Skype at the t=4
        ActivityDAO newActivity1(startTime, 10, 10, "RealtimeVideo");
        activities.insert(newActivity1);
        //YouTube at t=64
        startTime += 60;
        ActivityDAO newActivity2(startTime, 10, 10, "Video");
        activities.insert(newActivity2);

        //Our day is 2 min long
        startTime += 60;
    }

    // Schedule 1st activity
    ActivityDAO firstAc = *activities.begin();
    startTime = firstAc.getStartTimeInSeconds();
    scheduleAt((simtime_t)startTime, startActivity);
}

