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

#ifndef DAO_ACTIVITYDAO_H_
#define DAO_ACTIVITYDAO_H_

#include <omnetpp.h>
#include <string>

using namespace omnetpp;
using namespace std;

class ActivityDAO : public cMessage {
public:
    ActivityDAO(int startTimeInSeconds, unsigned int uplink, unsigned int downlink, string activityType);
    virtual ~ActivityDAO();
    int getStartTimeInSeconds();
    unsigned int getUplink();
    unsigned int getDownlink();
    string getActivityType();
    bool operator< (const ActivityDAO &other) const;
private:
    int startTimeInSeconds;
    unsigned int uplink;
    unsigned int downlink;
    string activityType;
};

#endif /* DAO_ACTIVITYDAO_H_ */
