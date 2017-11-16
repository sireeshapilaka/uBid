/*
 * HttpUtils.h
 *
 *  Created on: Nov 16, 2017
 *      Author: sireeshapilaka
 */

#ifndef UTIL_STRUTILS_H_
#define UTIL_STRUTILS_H_


#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>

namespace strutils {

std::string trimLeft(std::string str);
std::string trimRight(std::string str);

std::string trim(std::string str);

}


#endif /* UTIL_STRUTILS_H_ */
