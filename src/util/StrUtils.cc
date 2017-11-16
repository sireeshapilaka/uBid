/*
 * HttpUtils.cc
 *
 *  Created on: Nov 16, 2017
 *      Author: sireeshapilaka
 */
#include <algorithm>
#include <ctype.h>
#include <util/StrUtils.h>

namespace strutils {

inline bool isnotspace(int c) { return !isspace(c); }

std::string trimLeft(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), isnotspace));
    return s;
}

std::string trimRight(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), isnotspace).base(), s.end());
    return s;
}

std::string trim(std::string str)
{
    str = trimLeft(str);
    str = trimRight(str);
    return str;
}

}



