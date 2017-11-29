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

#include <AppAccessResponse.h>

AppAccessResponse::AppAccessResponse() {
    // TODO Auto-generated constructor stub

}

AppAccessResponse::~AppAccessResponse() {
    // TODO Auto-generated destructor stub
}


double AppAccessResponse::getGrantedDownlinkBytes() const {
    return grantedDownlinkBytes;
}

void AppAccessResponse::setGrantedDownlinkBytes(double grantedDownlinkBytes) {
    this->grantedDownlinkBytes = grantedDownlinkBytes;
}

double AppAccessResponse::getGrantedDownlinkDuration() const {
    return grantedDownlinkDuration;
}

double AppAccessResponse::getGrantedUplinkDuration() const {
    return grantedUplinkDuration;
}

void AppAccessResponse::setGrantedDownlinkDuration(double grantedDownlinkDuration) {
    this->grantedDownlinkDuration = grantedDownlinkDuration;
}

void AppAccessResponse::setGrantedUplinkDuration(double grantedUplinkDuration) {
    this->grantedUplinkDuration = grantedUplinkDuration;
}

double AppAccessResponse::getGrantedUplinkBytes() const {
    return grantedUplinkBytes;
}

void AppAccessResponse::setGrantedUplinkBytes(double grantedUplinkBytes) {
    this->grantedUplinkBytes = grantedUplinkBytes;
}

bool AppAccessResponse::isStatus() const {
    return status;
}

void AppAccessResponse::setStatus(bool status) {
    this->status = status;
}


void AppAccessResponse::setActivityType(string activityType) {
    this->activityType = activityType;
}

string AppAccessResponse::getActivityType() {
    return this->activityType;
}

double* AppAccessResponse::getGrantedUplinkThroughput() const {
    return grantedUplinkThroughput;
}

void AppAccessResponse::setGrantedUplinkThroughput(double *grantedUplinkThroughput) {
    this->grantedUplinkThroughput = grantedUplinkThroughput;
}

double* AppAccessResponse::getGrantedDownlinkThroughput() const {
    return grantedDownlinkThroughput;
}

void AppAccessResponse::setGrantedDownlinkThroughput(double *grantedDownlinkThroughput) {
    this->grantedDownlinkThroughput = grantedDownlinkThroughput;
}
