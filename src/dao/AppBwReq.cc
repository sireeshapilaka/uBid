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

#include <AppBwReq.h>


AppBWReq::AppBWReq(unsigned int uplinkBytes, unsigned int downlinkBytes, string activityType, unsigned int ulDuration, unsigned int dlDuration, unsigned int ulBandwidth, unsigned int dlBandwidth) {
    this->uplinkBytes = uplinkBytes;
    this->downlinkBytes = downlinkBytes;
    this->activityType = activityType;
    this->ulDuration = ulDuration;
    this->dlDuration = dlDuration;
    this->ulBandwidth = ulBandwidth;
    this->dlBandwidth = dlBandwidth;
}
AppBWReq::~AppBWReq() {

}
unsigned int AppBWReq::getUplinkBytes() {
    return uplinkBytes;
}
unsigned int AppBWReq::getDownlinkBytes() {
    return downlinkBytes;
}
unsigned int AppBWReq::getUlDuration() {
    return ulDuration;
}
unsigned int AppBWReq::getDlDuration() {
    return dlDuration;
}
unsigned int AppBWReq::getUlBandwidth() {
    return ulBandwidth;
}
unsigned int AppBWReq::getDlBandwidth() {
    return dlBandwidth;
}
string AppBWReq::getActivityType() {
    return activityType;
}
