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
    users[0] = "9079826788";
    users[1] = "9072329683";
    users[2] = "9073153977";
    users[3] = "9073542489";
    users[4] = "9073543211";
    users[5] = "9073543250";
    users[6] = "9073543849";
    users[7] = "9073546052";
    users[8] = "9073546744";
    users[9] = "9073552126";
    users[10] = "9073552424";
    users[11] = "9073554901";
    users[12] = "9073555454";
    users[13] = "9073558632";
    users[14] = "9073559725";
    users[15] = "9073559885";
    users[16] = "9078630022";
    users[17] = "9078630110";
    users[18] = "9078632460";
    users[19] = "9078633333";
    users[20] = "9078638997";

    dates[0] = "May4";
    dates[1] = "Apr28";
    dates[2] = "Apr29";
    dates[3] = "Apr30";
    dates[4] = "May1";
    dates[5] = "May2";
    dates[6] = "May3";
    // TODO Auto-generated constructor stub
}

Ue::~Ue() {
    // TODO Auto-generated destructor stub
}

void Ue::initialize() {
    int budgetPerSession = par("budget").longValue();
    ua = new UserAgent(this, (double)budgetPerSession);
    // Read in the App Traffic Model file and schedule the appropriate traffic events
    this->appTrafficFileName = par("appTrafficConfigFile").stringValue();

    int userId = par("userIndex").longValue();
    int dateId = par("dateIndex").longValue();

    string filePath = this->appTrafficFileName + users[userId] + "/" + dates[dateId] + "/";

    EV_DEBUG << "File is " << filePath << endl;
    string files[] = {filePath + "Audio.config", filePath + "Video.config", filePath+"RealtimeVideo.config", filePath + "Browser.config", filePath + "TCP.config"};
    std::ifstream scriptfilestream;
    int numAc = 0;
    for (int i = 0; i < 5; i++) {
        // Scan each file and add their events to a set
        string activityType = (i == 0 ? "Audio" : (i == 1 ? "Video" : (i == 2 ? "RealtimeVideo" : (i == 3 ? "Browser" : "TCP"))));
        string fileName = files[i];
        scriptfilestream.open(fileName);
        if (!scriptfilestream.is_open())
            throw cRuntimeError("Could not open script file %s", fileName.c_str());
       int timepart;
       std::string line;
       unsigned int pos, downlink, uplink;
       simtime_t t;
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
           downlink = stoi(line.substr(oldPos + 1, pos));
           uplink = stoi(line.substr(pos+1, line.size()-1));
           timepart = (timepart%1200);
           if (activityType == "Audio" || activityType == "Video") { // TCP Video or Audio Flows
               if (downlink <= 1500) { // whats the point in streaming if its not atleast 1500 bytes
                   continue;
               }
           } else if (activityType == "RealtimeVideo") { // UDP Realtime Video
               if (downlink <= 1500 && uplink <= 1500) { // Whats the point in streaming this
                   continue;
               }
           }
           t = (simtime_t)timepart;

           ActivityDAO newActivity(timepart, uplink, downlink, activityType);
           activities.insert(newActivity);
           numAc++;
       }
       scriptfilestream.close();
    }
    // Schedule a self-message for the first item in the set
    if (!activities.empty()) {
        ActivityDAO firstAc = *activities.begin();
        int startTime = firstAc.getStartTimeInSeconds();
        // Dont start anything immediately. Give setup a second
        if (startTime < 2) {
            startTime = 2;
        }
        scheduleAt((simtime_t)startTime, startActivity);
    } else {
        //throw cRuntimeError("No activities for UE!");
    }
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
        if (startTimeForActivity < currentTime.dbl()) {
            int schedTime = ceil(currentTime.dbl()) + expectedDurationActivity;
            scheduleAt(SimTime(schedTime), startActivity);
        } else {
            int schedTime = max(startTimeForActivity, (int)ceil(currentTime.dbl()) + expectedDurationActivity);
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
    // TODO: provide desired duration for uplink and downlink
    ua->getReservedAccess(activityToLaunch.getActivityType(), activityToLaunch.getDownlink(), activityToLaunch.getUplink(), 0, 0);
    activities.erase(activities.begin());
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
