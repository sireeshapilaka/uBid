/*
 * UaUtils.h
 *
 *  Created on: Nov 29, 2017
 *      Author: sireeshapilaka
 */

#ifndef UTIL_UAUTILS_H_
#define UTIL_UAUTILS_H_

#include<random>
#include<ModeConstants.h>
#include<string.h>

using namespace std;

namespace uaUtils {
static std::default_random_engine generator;
static std::normal_distribution<double> alphaDistr (0.5,0.05);
double genAlpha();
double calcPhiApp(string activityType, double bandwidth, double aConst, double bConst);

} /* namespace uaUtils */

#endif /* UTIL_UAUTILS_H_ */
