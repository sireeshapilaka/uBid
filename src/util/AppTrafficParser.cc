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

#include <util/AppTrafficParser.h>
#include <dao/ActivityDAO.h>
using namespace std;
AppTrafficParser::AppTrafficParser() {
}

AppTrafficParser::~AppTrafficParser() {
    // TODO Auto-generated destructor stub
}
//
//list<ActivityDAO*> AppTrafficParser::getAppTrafficEvents(cRNG* rng, string fileName) {
//    cEnvir *env = getSimulation()->getActiveEnvir();
//    string genericApp = "inet.applications.tcpapp.TCPBasicClientApp";
//    string fullFileName = fileLocationPrefix + fileName;
//    cXMLElement *root = env->getXMLDocument(fullFileName.c_str());
//    double currentTime = simtime_t().dbl();
//
//    cXMLElement *activity = root->getFirstChildWithTag("activity");
//    list<ActivityDAO*> activityDAOs;
//    do {
//        string startTime = activity->getFirstChildWithTag("start_time")->getNodeValue();
//
//        std::vector<std::string> timeParts = split(startTime, ':');
//
//        int timeInSeconds = stoi(timeParts[1]) + stoi(timeParts[0])*60;
//        if (timeInSeconds > currentTime) {
//            string application = activity->getFirstChildWithTag("application")->getNodeValue();
//            string description = activity->getFirstChildWithTag("description")->getNodeValue();
//            int downlink = static_cast<unsigned int>(*(activity->getFirstChildWithTag("consumption")->getFirstChildWithTag("downlink")->getNodeValue()));
//            int uplink = static_cast<unsigned int>(*(activity->getFirstChildWithTag("consumption")->getFirstChildWithTag("uplink")->getNodeValue()));
//
//            string inetApp;
//            try {
//                inetApp = appTypeMapping.at(application);
//            } catch (out_of_range e) {
//                inetApp = genericApp;
//            }
//            // Add randomness to startTime in the order of 5 minutes
//            activityDAOs.push_back(new ActivityDAO(intuniform(rng, timeInSeconds, timeInSeconds + 300), downlink, uplink, inetApp));
//        }
//        activity = activity->getNextSiblingWithTag("activity");
//    } while(activity != NULL);
//    return activityDAOs;
//}

std::vector<std::string> AppTrafficParser::split(string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

template<typename Out>
void AppTrafficParser::split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}


