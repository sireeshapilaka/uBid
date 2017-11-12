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

#include <dao/ActivityDAO.h>

ActivityDAO::ActivityDAO(int startTimeInSeconds, unsigned int uplink, unsigned int downlink, string activityType)
{
    this->startTimeInSeconds = startTimeInSeconds;
    this->uplink = uplink;
    this->downlink = downlink;
    this->activityType = activityType;
}

ActivityDAO::~ActivityDAO() {
    // TODO Auto-generated destructor stub
}


string ActivityDAO::getActivityType() {
    return activityType;
}

unsigned int ActivityDAO::getDownlink() {
    return downlink;
}

int ActivityDAO::getStartTimeInSeconds() {
    return startTimeInSeconds;
}

unsigned int ActivityDAO::getUplink() {
    return uplink;
}

bool ActivityDAO::operator< (const ActivityDAO &other) const {
  return this->startTimeInSeconds < other.startTimeInSeconds;
}
