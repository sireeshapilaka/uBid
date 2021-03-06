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

#ifndef APPBWREQ_H_
#define APPBWREQ_H_

#include <omnetpp.h>
#include <string.h>
using namespace omnetpp;
using namespace std;
class AppBWReq : public cMessage {
private:
    unsigned int uplinkBytes;
    unsigned int downlinkBytes;
    string activityType;

public:
    AppBWReq(unsigned int uplinkBytes, unsigned int downlinkBytes, string activityType);
    virtual ~AppBWReq();
    unsigned int getUplinkBytes();
    unsigned int getDownlinkBytes();
    string getActivityType();
};

#endif /* APPBWREQ_H_ */
