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
    if(!rpisPerDay.empty()) {
        vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
        while(iter!=rpisPerDay.end()) {
            rpisPerDay.erase(iter++);
        }
    }
    startTimes.clear();
}

void UeMC::initialize() {
    // Populate rpis from the file
    int userId = par("userIndex").longValue();
    this->appTrafficFileName = par("appTrafficConfigFile").stringValue();
    string filePath = this->appTrafficFileName + to_string(userId)+ "/";
    EV_DEBUG << "File is " << filePath << endl;

    // Dealing with only RealtimeVideo
    string files[] = {filePath+"RealtimeVideo.config"};
    std::ifstream scriptfilestream;
    int numAc = 0;
    for (int i = 0; i < 1; i++) {
        // Scan each file and add their events to a set
        string activityType = (i == 0 ? "RealtimeVideo" : (i == 1 ? "Video" : (i == 2 ? "Audio" : (i == 3 ? "Browser" : "TCP"))));
        string fileName = files[i];
        scriptfilestream.open(fileName);
        if (!scriptfilestream.is_open())
            throw cRuntimeError("Could not open script file %s", fileName.c_str());
       int timepart;
       std::string line;
       unsigned int pos, downlink, uplink, dlDuration, ulDuration;
       int currentSimulationTime = simTime().dbl();
       while (!std::getline(scriptfilestream, line).eof()) {
           line = trim(line);
           if (line.find("#") == 0)
               continue;
           pos = line.find(",");
           if (pos == -1)
               continue;
           timepart = stof(line.substr(0, pos));
           if (currentSimulationTime > timepart) {
               continue;
           }
           int oldPos = pos;
           pos = line.find(",", pos+1);
           uplink = stoi(line.substr(oldPos + 1, pos));
           oldPos = pos;
           pos = line.find(",", pos+1);
           ulDuration = stoi(line.substr(oldPos+1, pos));
           oldPos = pos;
           pos = line.find(",", pos+1);
           downlink = stoi(line.substr(oldPos+1, pos));
           dlDuration = stoi(line.substr(pos+1, line.size()-1));
           timepart = (timepart%dayLength);

           rpisPerDay.push_back(new AppBWReq(0, 0, activityType, ulDuration, dlDuration, uplink, downlink));
           startTimes.push_back(timepart);
           numAc++;
       }
       scriptfilestream.close();
    }
    numOfAuctions = numAc;

    // Initialize the ua
    ua = new UserAgentMC(this, rpisPerDay, numOfAuctions);

    int day, auction;

    for(day=0; day<numOfDays; day++) {
        // Dummy uplink and downlink values since we are populating deterministic throughputs in UA
        vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
        for(auction=0;auction<numOfAuctions;auction++) {
            // Our day is 2 min long
            int startTime = day*dayLength+startTimes[auction] ;
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

