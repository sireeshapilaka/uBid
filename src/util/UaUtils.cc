/*
 * UaUtils.cc
 *
 *  Created on: Nov 29, 2017
 *      Author: sireeshapilaka
 */

#include <UaUtils.h>

namespace uaUtils {

double genAlpha() {
    return alphaDistr(generator);
}

double calcPhiApp(string activityType, double bandwidth, double aConst, double bConst){
    // TODO : Revisit the null bandwidth case

    int app_bw = 0;
    int max_app_bw = 1;
    if(activityType=="Audio") {
        max_app_bw = 3;
        if(bandwidth>=AUDIO_HIGH) {
            app_bw = 3;
        } else if(bandwidth>=AUDIO_STD) {
            app_bw = 2;
        } else if(bandwidth>=AUDIO_LOW){
            app_bw = 1;
        } else {
            app_bw = 0;
        }
    } else if(activityType=="Video") {
        max_app_bw = 5;
        if(bandwidth>=VIDEO_VHIGH) {
            app_bw = 5;
        } else if(bandwidth>=VIDEO_HIGH) {
            app_bw = 4;
        } else if(bandwidth>=VIDEO_STD) {
            app_bw = 3;
        } else if(bandwidth>=VIDEO_MEDIUM){
            app_bw = 2;
        } else if(bandwidth>=VIDEO_LOW) {
            app_bw = 1;
        } else {
            app_bw = 0;
        }
    } else if(activityType=="RealtimeVideo") {
        max_app_bw = 4;
        if(bandwidth>=REALTIMEHD_HIGH) {
            app_bw = 4;
        } else if(bandwidth>=REALTIMEHD_LOW) {
            app_bw = 3;
        } else if(bandwidth>=REALTIME_HIGH) {
            app_bw = 2;
        } else if(bandwidth>=REALTIME_LOW) {
            app_bw = 1;
        } else {
            app_bw = 0;
        }
    } else {
        max_app_bw = 1;
        if(bandwidth>0)
            app_bw = 1;
        else
            app_bw = 0;
    }

    return ceil(bConst*(pow(aConst, app_bw)));

    //double random = (double)rand() / RAND_MAX;
    //return ((double)app_bw/(double)max_app_bw);// + (0.01*random) ;
}

} /* namespace uaUtils */
