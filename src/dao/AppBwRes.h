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

#ifndef APPBWRES_H_
#define APPBWRES_H_

#include <omnetpp.h>
#include <string.h>

using namespace omnetpp;
using namespace std;

enum status {granted = 1,rejected,revoked,released,partially_granted};

class AppBWRes: public cMessage {
    double *dlBandwidth = NULL;
    double *ulBandwidth = NULL;
    int dlDuration = 0;
    int ulDuration = 0;
    string activityType;
    double bidAmount = 0;
    int ueId;
public:
    AppBWRes();
    AppBWRes(int dlDuration, int ulDuration, double *dlBandwidth, double *ulBandwidth);
    virtual ~AppBWRes();

    void setDlBandwidth(double *bandwidth);
    void setUlBandwidth(double *bandwidth);
    double *getDlBandwidth();
    double *getUlBandwidth();
    double getDlBandwidth(int i);
    double getUlBandwidth(int i);

    void setDlDuration(int dlDuration);
    void setUlDuration(int ulDuration);
    int getDlDuration();
    int getUlDuration();

    void setActivityType(string activity);
    string getActivityType();

    void setBidAmount(double amount);
    double getBidAmount();

    void setUser(int userName);
    int getUser();
};

#endif /* APPBWRES_H_ */
