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

#ifndef UTIL_APPTRAFFICPARSER_H_
#define UTIL_APPTRAFFICPARSER_H_

#include <omnetpp.h>
#include <string>
#include <dao/ActivityDAO.h>
#include <iostream>
#include <sstream>

using namespace omnetpp;
using namespace std;
class AppTrafficParser {
public:
//    static list<ActivityDAO*> getAppTrafficEvents(cRNG* rng, string fileName);
    static vector<string> split(string &s, char delim);
    template<typename Out>
    static void split(const std::string &s, char delim, Out result);
private:
    AppTrafficParser();
    virtual ~AppTrafficParser();
};

#endif /* UTIL_APPTRAFFICPARSER_H_ */
