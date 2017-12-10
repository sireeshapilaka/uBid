//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <Ue.h>
#include <util/AppTrafficParser.h>
#include <dao/ActivityDAO.h>
#include <corenetwork/nodes/UserAgent.h>

Define_Module(Ue);

//int Ue::counter = 0;
void Ue::finish() {
    recordScalar("Initiated Sessions", numSessions);
    recordScalar("Initiated Browser Sessions", numBrowserSessions);
    recordScalar("Initiated TCP Video Sessions", numTCPVideoSessions);
    recordScalar("Initiated TCP Audio Sessions", numTCPAudioSessions);
    recordScalar("Initiated UDP Sessions", numUDPSessions);
}

Ue::Ue() {
}

Ue::~Ue() {
    if(!rpisPerDay.empty()) {
        vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
        while(iter!=rpisPerDay.end()) {
            rpisPerDay.erase(iter++);
        }
    }
    startTimes.clear();
}

void Ue::initialize() {
    deterministic = par("deterministic").boolValue();
    totalBudget = par("budget").longValue();
    if (totalBudget <= 0) {
        throw cRuntimeError("User has budget <= 0!");
    }
    // Populate rpis from the file
    int userId = par("userIndex").longValue();
    this->appTrafficFileName = par("appTrafficConfigFile").stringValue();
    string filePath = this->appTrafficFileName + to_string(userId)+ "/";
    myType = par("nodeType").stringValue();
    cout << "File is " << filePath << endl;

    // Dealing with only RealtimeVideo
    string fileName = filePath+"RealtimeVideo.config";
    std::ifstream scriptfilestream;
    int numAc = 0;
    string activityType = "RealtimeVideo";
    // Every user has only one activity file
    scriptfilestream.open(fileName);
    if (!scriptfilestream.is_open())
       throw cRuntimeError("Could not open script file %s", fileName.c_str());
    int timepart;
    std::string line;
    unsigned int pos, downlink, uplink, dlDuration, ulDuration;
    while (!std::getline(scriptfilestream, line).eof()) {
      line = trim(line);
      pos = line.find(",");
      if (pos == -1)
          continue;
      timepart = stof(line.substr(0, pos));
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
      timepart = (timepart%dayLen);

      rpisPerDay.push_back(new AppBWReq(0, 0, activityType, ulDuration, dlDuration, uplink, downlink));
      startTimes.push_back(timepart);
      numAc++;
    }

    scriptfilestream.close();
    numOfAuctions = numAc;

    double budgetPerSession = totalBudget/numOfAuctions; // Naive Budget distribution strategy by Non MC User;
    if (numOfAuctions <= 0) {
        throw cRuntimeError("This user wants nothing ever?");
    }
    int day, auction;
    ua = new UserAgent(this, budgetPerSession, rpisPerDay, numOfAuctions);

    for(day=0; day<days; day++) {
       // Dummy uplink and downlink values since we are populating deterministic throughputs in UA
       vector<AppBWReq*>::iterator iter = rpisPerDay.begin();
       for(auction=0;auction<numOfAuctions;auction++) {
           // Our day is 2 min long
           int startTime = day*dayLen+startTimes[auction] ;
           ActivityDAO newActivity(startTime, 10, 10, (*iter)->getActivityType());
           activities.insert(newActivity);
           iter++;
       }
    }

    if (this->rng == NULL) {
        rng = this->getParentModule()->getRNG(0);
        if (rng == NULL) {
            throw cRuntimeError("No RNG found!");
        }
    }

    // Schedule 1st activity
    ActivityDAO firstAc = *activities.begin();
    int startTime = firstAc.getStartTimeInSeconds();
    int offset = 0;
    if (!deterministic) { // only possible for non MC users
        offset = intuniform(0, 15, 0);
    }
    scheduleAt((simtime_t)(startTime + offset), startActivity);
}

void Ue::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg == startActivity){
            cancelEvent(startActivity);
            startNextActivity();
        } else {
            throw cRuntimeError("Message unknown");
        }
    } else {
        throw cRuntimeError("Message from unknown!");
    }
}

void Ue::scheduleNextActivity() {
    if (activities.size() > 0) {
        set<ActivityDAO>::iterator iter = activities.begin();
        ActivityDAO activityToLaunch = (*iter);
        int startTimeForActivity = activityToLaunch.getStartTimeInSeconds();
        simtime_t currentTime = simTime();

        int offset = 0;
        if (!deterministic) { // only possible for non MC users
            offset = intuniform(0, 15, 0);
        }

        if (startTimeForActivity < currentTime.dbl()) {
            int schedTime = ceil(currentTime.dbl()) + expectedDurationActivity + offset;
            scheduleAt(SimTime(schedTime), startActivity);
        } else {
            int schedTime = max(startTimeForActivity, (int)ceil(currentTime.dbl()) + expectedDurationActivity) + offset;
            scheduleAt(SimTime(schedTime), startActivity);
        }
        expectedDurationActivity = 0;
    } else {
        EV_DEBUG << "All events are done. Nothing more to schedule" << endl;
    }
}

// User has launched an application. Send the appropriate request to the UA -- No application should be active at this point
void Ue::startNextActivity() {
    if(activities.size()==0) return;
    set<ActivityDAO>::iterator iter = activities.begin();
    ActivityDAO activityToLaunch = (*iter);
    string type = activityToLaunch.getActivityType();
    activities.erase(activities.begin());

    // Start only for realtime
    if(type=="RealtimeVideo") {
        ua->getReservedAccess();
    } else {
        throw cRuntimeError("Activity of any other type than RealtimeVideo is unsupported!");
    }
}

void Ue::processUAResponse(cMessage* message) {
    AppAccessResponse* appAccessResponse = check_and_cast<AppAccessResponse*>(message);
    if (appAccessResponse->isStatus()) { //Indicates Success
        string activityType = appAccessResponse->getActivityType();

       // Absolutely none of these ar ebeing used to influence actual activity flows.
        // They are merely here for knowing what we were guaranteed
        double *uplinkThroughput = appAccessResponse->getGrantedUplinkThroughput();
        double *downlinkThroughput = appAccessResponse->getGrantedDownlinkThroughput();
        double uplinkDuration = appAccessResponse->getGrantedUplinkDuration();
        double downlinkDuration = appAccessResponse->getGrantedDownlinkDuration();
        double uplinkBytes = appAccessResponse->getGrantedUplinkBytes();
        double downlinkBytes = appAccessResponse->getGrantedDownlinkBytes();

        if (activities.size() == 0) {
            // We are done with all scheduled activities
            return;
        }
        set<ActivityDAO>::iterator iter = activities.begin();
        ActivityDAO activityToLaunch = (*iter);

        unsigned int downlink = ((8*activityToLaunch.getDownlink()) < activityToLaunch.getDownlink() ? UINT_MAX: 8*activityToLaunch.getDownlink());
        unsigned int uplink = ((8*activityToLaunch.getUplink()) < activityToLaunch.getUplink() ? UINT_MAX: 8*activityToLaunch.getUplink());
        if (activityType == "Browser" || activityType == "TCP") {
            EV_DEBUG << "Starting Browser Activity for Uplink: " << uplink << " And Downlink " << downlink << endl;
            numBrowserSessions++;
            lastActiveAppType = "Browser";
        }
        else if (activityType == "Audio") {
            EV_DEBUG << "Starting Audio Stream for Uplink: " << uplink << " And Downlink " << downlink << endl;
            numTCPAudioSessions++;
            lastActiveAppType = "Audio";
            expectedDurationActivity = appAccessResponse->getGrantedDownlinkDuration();
        } else if (activityType == "Video") {
            EV_DEBUG << "Starting Video Stream for Uplink: " << uplink << " And Downlink " << downlink << endl;
            numTCPVideoSessions++;
            lastActiveAppType = "Video";
            expectedDurationActivity = appAccessResponse->getGrantedDownlinkDuration();
        } else if (activityType == "RealtimeVideo") {
            // UDP Video streaming
            EV_DEBUG << "Starting Realtime Streaming for Uplink: " << uplink << " And Downlink " << downlink << endl;
            numUDPSessions++;
            lastActiveAppType = "RealtimeVideo";
            expectedDurationActivity = ceil(max(appAccessResponse->getGrantedDownlinkDuration(), appAccessResponse->getGrantedUplinkDuration()));// ceil( max(uplink/uplinkThroughput, downlink/downlinkThroughput));
        }

        if (activityType == "RealtimeVideo") {
            udpOn = true;
        } else {
            tcpOn = true;
        }
        scheduleNextActivity();
    } else {
        EV_DEBUG << "Abort activity" << endl;
        handleFailure(appAccessResponse);
    }
    numSessions++;
    appAccessResponse = NULL;
    delete message;
}

void Ue::handleFailure(AppAccessResponse* appAccessResponse) {
    // Move on
    scheduleNextActivity();
}

void Ue::setTcpDone() {
    this->tcpOn = false;
}

void Ue::sendRPIResponse(BidResponse* bidResponse) {
    ua->handleBidResponse(bidResponse);
}
