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

#include <AppBwRes.h>

AppBWRes::AppBWRes() {}
AppBWRes::AppBWRes(int dlDuration, int ulDuration, double dlBandwidth, double ulBandwidth) {
    this->dlBandwidth = dlBandwidth;
    this->ulBandwidth = ulBandwidth;
    this->dlDuration = dlDuration;
    this->ulDuration = ulDuration;
}
AppBWRes::~AppBWRes() {}
void AppBWRes::setDlBandwidth(double bandwidth) {
    this->dlBandwidth = bandwidth;
}

void AppBWRes::setUlBandwidth(double bandwidth) {
    this->ulBandwidth = bandwidth;
}

double AppBWRes::getDlBandwidth() {
    return this->dlBandwidth;
}

double AppBWRes::getUlBandwidth() {
    return this->ulBandwidth;
}


void AppBWRes::setDlDuration(int duration) {
    this->dlDuration = duration;
}

void AppBWRes::setUlDuration(int duration) {
    this->ulDuration = duration;
}

int AppBWRes::getDlDuration() {
    return this->dlDuration;
}

int AppBWRes::getUlDuration() {
    return this->ulDuration;
}

void AppBWRes::setActivityType(string activity) {
    this->activityType = activity;
}

string AppBWRes::getActivityType() {
    return this->activityType;
}

void AppBWRes::setBidAmount(double amount) {
    this->bidAmount = amount;
}

double AppBWRes::getBidAmount() {
    return this->bidAmount;
}

void AppBWRes::setUser(int userName) {
    this->ueId = userName;
}

int AppBWRes::getUser() {
    return this->ueId;
}

void AppBWRes::printForDebug() {
    cout << "(" << this->ulBandwidth << ", " << this->ulDuration << ", " << this->dlBandwidth << ", " << this->dlDuration << ")";
}
