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

#ifndef DAO_APPACCESSRESPONSE_H_
#define DAO_APPACCESSRESPONSE_H_

#include <string.h>
#include <omnetpp.h>
using namespace std;
using namespace omnetpp;
class AppAccessResponse : public cMessage {
private:
    bool status = false;
    double grantedDownlinkBytes= 0;
    double grantedUplinkBytes = 0;
    double* grantedDownlinkThroughput = NULL;
    double* grantedUplinkThroughput = NULL;
    double grantedUplinkDuration = 0.0;
    double grantedDownlinkDuration = 0.0;
    string activityType;
public:
    AppAccessResponse();
    virtual ~AppAccessResponse();
    double getGrantedDownlinkBytes() const;
    void setGrantedDownlinkBytes(double grantedDownlinkBytes);
    double getGrantedUplinkBytes() const;
    void setGrantedUplinkBytes(double grantedUplinkBytes);
    bool isStatus() const;
    void setStatus(bool status);
    void setActivityType(string activityType);
    string getActivityType();

    double getGrantedDownlinkDuration() const;
    void setGrantedDownlinkDuration(double grantedDownlinkDurationBytes);
    double getGrantedUplinkDuration() const;
    void setGrantedUplinkDuration(double grantedUplinkDurationBytes);


    double* getGrantedUplinkThroughput() const;
    void setGrantedUplinkThroughput(double* grantedUplinkThroughput);
    double* getGrantedDownlinkThroughput() const;
    void setGrantedDownlinkThroughput(double* grantedDownlinkThroughput);
};

#endif /* DAO_APPACCESSRESPONSE_H_ */
