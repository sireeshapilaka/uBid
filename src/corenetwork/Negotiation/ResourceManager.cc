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

#include <ResourceManager.h>
#include <iostream>

using namespace std;

/* Resource Manager is responsible for keeping track of allocated resources and providing a time lapse view of how load on the network changes */
ResourceManager::ResourceManager() {
    ulCellRsrcAllocMapList = new std::vector<CellResourceAllocMap*>();
    dlCellRsrcAllocMapList = new std::vector<CellResourceAllocMap*>();
    ulCellAllocCountMapList = new std::vector<CellAllocationsCountMap*>();
    dlCellAllocCountMapList = new std::vector<CellAllocationsCountMap*>();

    ulCellCapacities = new std::vector<double>();
    dlCellCapacities = new std::vector<double>();
    for (int i = 0; i < 1; i++) {
        std::string s1 = "Cell";
        std::string s2 = s1 + " " + std::to_string(i);
        std::string s3 = s2 + " Downlink";
        downlinkCapacities[i].setName(s3.c_str());
    }
    for (int i = 0; i < 1; i++) {
        std::string s1 = "Cell";
        std::string s2 = s1 + " " + std::to_string(i);
        std::string s3 = s2 + " Uplink";
        uplinkCapacities[i].setName(s3.c_str());
    }
    numUplinkAllocations.setName("Uplink Alloc Count over Time");
    numDownlinkAllocations.setName("Downlink Alloc Count over Time");
    // TODO: without negotiation


    uplinkDemandVisualization.setName("Uplink Demand in Kbps - Without Network");
    downlinkDemandVisualization.setName("Downlink Demand in Kbps - Without Network");
    uplinkDemandCountVisualization.setName("Uplink Count - Without Network");
    downlinkDemandCountVisualization.setName("Downlink Count - Without Network");

    uplinkDemandBurstyVisualization.setName("Uplink Demand Bursty Traffic in Kbps - Without Network");
    downlinkDemandBurstyVisualization.setName("Downlink Demand Bursty Traffic in Kbps - Without Network");
    uplinkDemandCountBurstyVisualization.setName("Uplink Count Bursty Traffic - Without Network");
    downlinkDemandCountBurstyVisualization.setName("Downlink Count Bursty Traffic - Without Network");

    uplinkDemandUDPVisualization.setName("Uplink Demand UDP Traffic in Kbps - Without Network");
    downlinkDemandUDPVisualization.setName("Downlink Demand UDP Traffic in Kbps - Without Network");
    uplinkDemandCountUDPVisualization.setName("Uplink Count UDP Traffic - Without Network");
    downlinkDemandCountUDPVisualization.setName("Downlink Count UDP Traffic - Without Network");

    downlinkDemandAudioVisualization.setName("Downlink Demand Audio Traffic in Kbps - Without Network");
    downlinkDemandVideoVisualization.setName("Downlink Demand Video Traffic in Kbps - Without Network");

    downlinkDemandCountAudioVisualization.setName("Downlink Count Audio Traffic- Without Network");
    downlinkDemandCountVideoVisualization.setName("Downlink Count Video Traffic - Without Network");
}

ResourceManager::~ResourceManager() {
    for (auto it = ulCellRsrcAllocMapList->cbegin(); it != ulCellRsrcAllocMapList->cend(); ++it) {
       delete (*it);
    }
    for (auto it = dlCellRsrcAllocMapList->cbegin(); it != dlCellRsrcAllocMapList->cend(); ++it) {
           delete (*it);
    }
    for (auto it = ulCellAllocCountMapList->cbegin(); it != ulCellAllocCountMapList->cend(); ++it) {
       delete (*it);
    }
    for (auto it = dlCellAllocCountMapList->cbegin(); it != dlCellAllocCountMapList->cend(); ++it) {
           delete (*it);
    }

    delete ulCellRsrcAllocMapList;
    delete dlCellRsrcAllocMapList;
    delete ulCellAllocCountMapList;
    delete dlCellAllocCountMapList;
    delete ulCellCapacities;
    delete dlCellCapacities;
}

int ResourceManager::getNumTowers() {
    return 1;
}

void ResourceManager::recordStats() {
    CellResourceAllocMap* uplinkMap = ulCellRsrcAllocMapList->front();
    CellResourceAllocMap* downlinkMap = dlCellRsrcAllocMapList->front();
    CellAllocationsCountMap* ulCount = ulCellAllocCountMapList->front();
    CellAllocationsCountMap* dlCount = dlCellAllocCountMapList->front();
    for (int i = 0; i < simTime().dbl(); i++) {
        simtime_t timeNow = SimTime(i);
        if (uplinkMap->find(timeNow) != uplinkMap->end()) {
            uplinkDemandVisualization.recordWithTimestamp(timeNow, uplinkMap->at(timeNow));
        } else {
            uplinkDemandVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (downlinkMap->find(timeNow) != downlinkMap->end()) {
            downlinkDemandVisualization.recordWithTimestamp(timeNow, downlinkMap->at(timeNow));
        } else {
            downlinkDemandVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (dlCount->find(timeNow) != dlCount->end()) {
            downlinkDemandCountVisualization.recordWithTimestamp(timeNow, dlCount->at(timeNow));
        } else {
            downlinkDemandCountVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (ulCount->find(timeNow) != ulCount->end()) {
            uplinkDemandCountVisualization.recordWithTimestamp(timeNow, ulCount->at(timeNow));
        } else {
            uplinkDemandCountVisualization.recordWithTimestamp(timeNow, 0);
        }

        if (uplinkBurstyMap->find(timeNow) != uplinkBurstyMap->end()) {
            uplinkDemandBurstyVisualization.recordWithTimestamp(timeNow, uplinkBurstyMap->at(timeNow));
        } else {
            uplinkDemandBurstyVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (downlinkBurstyMap->find(timeNow) != downlinkBurstyMap->end()) {
            downlinkDemandBurstyVisualization.recordWithTimestamp(timeNow, downlinkBurstyMap->at(timeNow));
        } else {
            downlinkDemandBurstyVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (ulBurstyCount->find(SimTime(i)) != ulBurstyCount->end()) {
            uplinkDemandCountBurstyVisualization.recordWithTimestamp(timeNow, ulBurstyCount->at(timeNow));
        } else {
            uplinkDemandCountBurstyVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (dlBurstyCount->find(SimTime(i)) != dlBurstyCount->end()) {
            downlinkDemandCountBurstyVisualization.recordWithTimestamp(timeNow, dlBurstyCount->at(timeNow));
        } else {
            downlinkDemandCountBurstyVisualization.recordWithTimestamp(timeNow, 0);
        }

        if (uplinkUDPMap->find(timeNow) != uplinkUDPMap->end()) {
            uplinkDemandUDPVisualization.recordWithTimestamp(timeNow, uplinkUDPMap->at(timeNow));
        } else {
            uplinkDemandUDPVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (downlinkUDPMap->find(timeNow) != downlinkUDPMap->end()) {
            downlinkDemandUDPVisualization.recordWithTimestamp(timeNow, downlinkUDPMap->at(timeNow));
        } else {
            downlinkDemandUDPVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (ulUDPCount->find(SimTime(i)) != ulUDPCount->end()) {
            uplinkDemandCountUDPVisualization.recordWithTimestamp(timeNow, ulUDPCount->at(timeNow));
        } else {
            uplinkDemandCountUDPVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (dlUDPCount->find(SimTime(i)) != dlUDPCount->end()) {
            downlinkDemandCountUDPVisualization.recordWithTimestamp(timeNow, dlUDPCount->at(timeNow));
        } else {
            downlinkDemandCountUDPVisualization.recordWithTimestamp(timeNow, 0);
        }

        if (downlinkAudioMap->find(timeNow) != downlinkAudioMap->end()) {
            downlinkDemandAudioVisualization.recordWithTimestamp(timeNow, downlinkAudioMap->at(timeNow));
        } else {
            downlinkDemandAudioVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (dlAudioCount->find(SimTime(i)) != dlAudioCount->end()) {
            downlinkDemandCountAudioVisualization.recordWithTimestamp(timeNow, dlAudioCount->at(timeNow));
        } else {
            downlinkDemandCountAudioVisualization.recordWithTimestamp(timeNow, 0);
        }

        if (downlinkVideoMap->find(timeNow) != downlinkVideoMap->end()) {
            downlinkDemandVideoVisualization.recordWithTimestamp(timeNow, downlinkVideoMap->at(timeNow));
        } else {
            downlinkDemandVideoVisualization.recordWithTimestamp(timeNow, 0);
        }
        if (dlVideoCount->find(SimTime(i)) != dlVideoCount->end()) {
            downlinkDemandCountVideoVisualization.recordWithTimestamp(timeNow, dlVideoCount->at(timeNow));
        } else {
            downlinkDemandCountVideoVisualization.recordWithTimestamp(timeNow, 0);
        }

    }
// TODO: without negotiaiton
//    for (int i = 0; i < 1; i++) {
//        downlinkCapacities[i].record(dlCellRsrcAllocMapList->at(i)->lower_bound(simTime())->second);
//    }
//    for (int i = 0; i < 1; i++) {
//        uplinkCapacities[i].record(ulCellRsrcAllocMapList->at(i)->lower_bound(simTime())->second);
//    }
//    for (int i = 0; i < 1; i++) {
//        uplinkCapacities[i].record(ulCellRsrcAllocMapList->at(i)->lower_bound(simTime())->second);
//    }
//    int maxCount = 0;
//    for (int i = 0; i < 1; i++) {
//        int ithcellCount = ulCellAllocCountMapList->at(i)->lower_bound(simTime())->second;
//        if (ithcellCount > maxCount) {
//            maxCount = ithcellCount;
//        }
//        if (maxCount >= 1e3) {
//            maxCount = 0;
//        }
//    }
//    numUplinkAllocations.record(maxCount);
//
//    maxCount = 0;
//    for (int i = 0; i < 1; i++) {
//        int ithcellCount = dlCellAllocCountMapList->at(i)->lower_bound(simTime())->second;
//        if (ithcellCount > maxCount) {
//            maxCount = ithcellCount;
//        }
//        if (maxCount >= 1e3) {
//            maxCount = 0;
//        }
//    }
//    numDownlinkAllocations.record(maxCount);
}

void ResourceManager::AddTower(double ulCapacity, double dlCapacity){

    ulCellCapacities->push_back(ulCapacity);
    dlCellCapacities->push_back(dlCapacity);

    CellResourceAllocMap* resMap = new CellResourceAllocMap();
    resMap->insert(std::pair<SimTime,double>(simTime(), 0));
    ulCellRsrcAllocMapList->push_back(resMap);
    resMap = new CellResourceAllocMap();
    resMap->insert(std::pair<SimTime,double>(simTime(), 0));
    dlCellRsrcAllocMapList->push_back(resMap);

    CellAllocationsCountMap* resCountMap = new CellAllocationsCountMap();
    resCountMap->insert(std::pair<SimTime,double>(simTime(), 0));
    ulCellAllocCountMapList->push_back(resCountMap);
    resCountMap = new CellAllocationsCountMap();
    resCountMap->insert(std::pair<SimTime,double>(simTime(), 0));
    dlCellAllocCountMapList->push_back(resCountMap);
}

void ResourceManager::clearElapsedEntries() {
    /* clear all the entries corresponding to elapsed time from UL and DL bandwidth maps for all cells
       * This is to prevent the dictionaries from bloating up as simulation proceeds */
      for (auto it = ulCellRsrcAllocMapList->cbegin(); it != ulCellRsrcAllocMapList->cend(); ++it) {
          std::map<SimTime,double>::iterator itlow;
          CellResourceAllocMap* resrcMap = (*it);
          itlow = resrcMap->lower_bound(simTime());
          // Erases all keys < current simTime (elapsed time)
          resrcMap->erase(resrcMap->begin(),itlow);
      }

      for (auto it = dlCellRsrcAllocMapList->cbegin(); it != dlCellRsrcAllocMapList->cend(); ++it) {
          std::map<SimTime,double>::iterator itlow;
          CellResourceAllocMap* resrcMap = (*it);
          itlow = resrcMap->lower_bound(simTime());
          // Erases all keys < current simTime (elapsed time)
          resrcMap->erase(resrcMap->begin(),itlow);
      }

      for (auto it = ulCellAllocCountMapList->cbegin(); it != ulCellAllocCountMapList->cend(); ++it) {
          std::map<SimTime,double>::iterator itlow;
          CellAllocationsCountMap* resrcMap = (*it);
          itlow = resrcMap->lower_bound(simTime());
          // Erases all keys < current simTime (elapsed time)
          resrcMap->erase(resrcMap->begin(),itlow);
      }

      for (auto it = dlCellAllocCountMapList->cbegin(); it != dlCellAllocCountMapList->cend(); ++it) {
          std::map<SimTime,double>::iterator itlow;
          CellAllocationsCountMap* resrcMap = (*it);
          itlow = resrcMap->lower_bound(simTime());
          // Erases all keys < current simTime (elapsed time)
          resrcMap->erase(resrcMap->begin(),itlow);
      }
}

//void ResourceManager::ReserveResources(BidCandidate* winningBid){
//    std::list<BidComponent*> *componentList = winningBid->bidDesc->componentList;
//    std::vector<CellResourceAllocMap*> *resrcAllocMapList;
//    std::vector<CellAllocationsCountMap*> *resrcAllocCountMapList;
//
//    for (std::list<BidComponent*>::iterator it = componentList->begin(); it != componentList->end(); it++){
//        BidComponent *bidComponent = (*it);
//        if (bidComponent->direction == uplink){
//            resrcAllocMapList = ulCellRsrcAllocMapList;
//            resrcAllocCountMapList = ulCellAllocCountMapList;
//        } else {
//            resrcAllocMapList = dlCellRsrcAllocMapList;
//            resrcAllocCountMapList = dlCellAllocCountMapList;
//        }
//
//        CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->at(bidComponent->cell);
//        CellAllocationsCountMap* resrcAllocCountMap = resrcAllocCountMapList->at(bidComponent->cell);
//
//        /* We maintain dictionary in terms of 5 second units*/ // You mean 1 second
//        std::map<SimTime,double>::iterator iter;
//        std::map<SimTime,double>::iterator countIter;
//  /*      if (simTime().dbl() < 35000) {
//            cout << "Reserving resources for cell 5 at time " << simTime().dbl() << " for " << getBwInKbps(bidComponent->bandwidth) <<
//                    "for duration between " << bidComponent->startTime.dbl() << " and " << bidComponent->endTime.dbl() << endl;
//        }*/
//
//        for (SimTime t = bidComponent->startTime; t < bidComponent->endTime; t = t + SimTime(1)  ){
//            iter = resrcAllocMap->find(t);
//            if (iter == resrcAllocMap->end()){
//                resrcAllocMap->insert(std::pair<SimTime,double>(t,(getBwInKbps(bidComponent->bandwidth))));
//            } else {
//                iter->second = iter->second + getBwInKbps(bidComponent->bandwidth);
//            }
//
//            countIter = resrcAllocCountMap->find(t);
//            if (countIter == resrcAllocCountMap->end()){
//                resrcAllocCountMap->insert(std::pair<SimTime,double>(t,1));
//            } else {
//                countIter->second = countIter->second + 1;
//            }
//        }
//    }
//}


// Bandwidth in Kbps and duration in seconds
void ResourceManager::ReserveResources(double uplinkBandwidth, double downlinkBandwidth, int uplinkDuration, int downlinkDuration,
        bool startSameTime, simtime_t startTime){
    std::vector<CellResourceAllocMap*> *resrcAllocMapList;
    std::vector<CellAllocationsCountMap*> *resrcAllocCountMapList;
    if (uplinkDuration > 0) {
        resrcAllocMapList = ulCellRsrcAllocMapList;
        resrcAllocCountMapList = ulCellAllocCountMapList;

        CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->at(0);
        CellAllocationsCountMap* resrcAllocCountMap = resrcAllocCountMapList->at(0);

        std::map<SimTime,double>::iterator iter;
        std::map<SimTime,double>::iterator countIter;

        for (SimTime t = SimTime(startTime); t < SimTime(startTime + uplinkDuration); t = t + SimTime(1)  ) {
            iter = resrcAllocMap->find(t);
            if (iter == resrcAllocMap->end()){
                resrcAllocMap->insert(std::pair<SimTime,double>(t, uplinkBandwidth));
            } else {
                iter->second = iter->second + uplinkBandwidth;
            }

            countIter = resrcAllocCountMap->find(t);
            if (countIter == resrcAllocCountMap->end()){
                resrcAllocCountMap->insert(std::pair<SimTime,double>(t,1));
            } else {
                countIter->second = countIter->second + 1;
            }
       }
    }

    if (downlinkDuration > 0) {
        resrcAllocMapList = dlCellRsrcAllocMapList;
        resrcAllocCountMapList = dlCellAllocCountMapList;

        CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->at(0);
        CellAllocationsCountMap* resrcAllocCountMap = resrcAllocCountMapList->at(0);

        std::map<SimTime,double>::iterator iter;
        std::map<SimTime,double>::iterator countIter;
        int realStartTime;
        if ((uplinkDuration > 0 && startSameTime) || uplinkDuration <= 0) {
            realStartTime = startTime.dbl();
        } else if (uplinkDuration > 0 && !startSameTime) {
            realStartTime = startTime.dbl() + uplinkDuration;
        }
        for (SimTime t = SimTime(realStartTime); t < SimTime(realStartTime + downlinkDuration); t = t + SimTime(1)  ) {
            iter = resrcAllocMap->find(t);
            if (iter == resrcAllocMap->end()){
                resrcAllocMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
            } else {
                iter->second = iter->second + downlinkBandwidth;
            }

            countIter = resrcAllocCountMap->find(t);
            if (countIter == resrcAllocCountMap->end()){
                resrcAllocCountMap->insert(std::pair<SimTime,double>(t,1));
            } else {
                countIter->second = countIter->second + 1;
            }
        }
    }
}


// Bandwidth in Kbps and duration in seconds
void ResourceManager::ReserveResourcesFake(double uplinkBandwidth, double downlinkBandwidth, int uplinkDuration, int downlinkDuration,
        bool startSameTime, simtime_t startTime, string activityType){
    std::vector<CellResourceAllocMap*> *resrcAllocMapList;
    std::vector<CellAllocationsCountMap*> *resrcAllocCountMapList;
    if (uplinkDuration > 0) {
        resrcAllocMapList = ulCellRsrcAllocMapList;
        resrcAllocCountMapList = ulCellAllocCountMapList;

        CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->at(0);
        CellAllocationsCountMap* resrcAllocCountMap = resrcAllocCountMapList->at(0);

        std::map<SimTime,double>::iterator iter;
        std::map<SimTime,double>::iterator countIter;

        for (SimTime t = SimTime(startTime); t < SimTime(startTime + uplinkDuration); t = t + SimTime(1)  ) {
            iter = resrcAllocMap->find(t);
            if (iter == resrcAllocMap->end()){
                resrcAllocMap->insert(std::pair<SimTime,double>(t, uplinkBandwidth));
            } else {
                iter->second = iter->second + uplinkBandwidth;
            }

            countIter = resrcAllocCountMap->find(t);
            if (countIter == resrcAllocCountMap->end()){
                resrcAllocCountMap->insert(std::pair<SimTime,double>(t,1));
            } else {
                countIter->second = countIter->second + 1;
            }

            if (activityType == "RealtimeVideo") {
                iter = uplinkUDPMap->find(t);
                if (iter == uplinkUDPMap->end()){
                    uplinkUDPMap->insert(std::pair<SimTime,double>(t, uplinkBandwidth));
                } else {
                    iter->second = iter->second + uplinkBandwidth;
                }

                countIter = ulUDPCount->find(t);
                if (countIter == ulUDPCount->end()){
                    ulUDPCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            } else if (activityType == "TCP" || activityType == "Browser") {
                iter = uplinkBurstyMap->find(t);
                if (iter == uplinkBurstyMap->end()){
                    uplinkBurstyMap->insert(std::pair<SimTime,double>(t, uplinkBandwidth));
                } else {
                    iter->second = iter->second + uplinkBandwidth;
                }

                countIter = ulBurstyCount->find(t);
                if (countIter == ulBurstyCount->end()){
                    ulBurstyCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            } else {
                throw cRuntimeError("Illegal activity type here");
            }
       }
    }

    if (downlinkDuration > 0) {
        resrcAllocMapList = dlCellRsrcAllocMapList;
        resrcAllocCountMapList = dlCellAllocCountMapList;

        CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->at(0);
        CellAllocationsCountMap* resrcAllocCountMap = resrcAllocCountMapList->at(0);

        std::map<SimTime,double>::iterator iter;
        std::map<SimTime,double>::iterator countIter;
        int realStartTime;
        if ((uplinkDuration > 0 && startSameTime) || uplinkDuration <= 0) {
            realStartTime = startTime.dbl();
        } else if (uplinkDuration > 0 && !startSameTime) {
            realStartTime = startTime.dbl() + uplinkDuration;
        }
        for (SimTime t = SimTime(realStartTime); t < SimTime(realStartTime + downlinkDuration); t = t + SimTime(1)  ) {
            iter = resrcAllocMap->find(t);
            if (iter == resrcAllocMap->end()){
                resrcAllocMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
            } else {
                iter->second = iter->second + downlinkBandwidth;
            }

            countIter = resrcAllocCountMap->find(t);
            if (countIter == resrcAllocCountMap->end()){
                resrcAllocCountMap->insert(std::pair<SimTime,double>(t,1));
            } else {
                countIter->second = countIter->second + 1;
            }

            if (activityType == "Audio") {
                iter = downlinkAudioMap->find(t);
                if (iter == downlinkAudioMap->end()){
                    downlinkAudioMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
                } else {
                    iter->second = iter->second + downlinkBandwidth;
                }

                countIter = dlAudioCount->find(t);
                if (countIter == dlAudioCount->end()){
                    dlAudioCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            } else if (activityType == "Video") {
                iter = downlinkVideoMap->find(t);
                if (iter == downlinkVideoMap->end()){
                    downlinkVideoMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
                } else {
                    iter->second = iter->second + downlinkBandwidth;
                }

                countIter = dlVideoCount->find(t);
                if (countIter == dlVideoCount->end()){
                    dlVideoCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            } else if (activityType == "RealtimeVideo") {
                iter = downlinkUDPMap->find(t);
                if (iter == downlinkUDPMap->end()){
                    downlinkUDPMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
                } else {
                    iter->second = iter->second + downlinkBandwidth;
                }

                countIter = dlUDPCount->find(t);
                if (countIter == dlUDPCount->end()){
                    dlUDPCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            } else {
                iter = downlinkBurstyMap->find(t);
                if (iter == downlinkBurstyMap->end()){
                    downlinkBurstyMap->insert(std::pair<SimTime,double>(t, downlinkBandwidth));
                } else {
                    iter->second = iter->second + downlinkBandwidth;
                }

                countIter = dlBurstyCount->find(t);
                if (countIter == dlBurstyCount->end()){
                    dlBurstyCount->insert(std::pair<SimTime,double>(t,1));
                } else {
                    countIter->second = countIter->second + 1;
                }
            }
        }
    }
}

double ResourceManager::computePeakLoadOnGivenCellDuringGivenDuration(int cell, int direction, simtime_t startTime, simtime_t endTime){
    std::vector<CellResourceAllocMap*> *resrcAllocMapList;

    if (direction == 0){ //Uplink
        resrcAllocMapList = ulCellRsrcAllocMapList;
    } else {
        resrcAllocMapList = dlCellRsrcAllocMapList;
    }
    CellResourceAllocMap* resrcAllocMap = resrcAllocMapList->front(); // MADHU: since there is only 1 tower in our configuration
    std::map<SimTime,double>::iterator itlow = resrcAllocMap->lower_bound(startTime);
    auto itup =  resrcAllocMap->upper_bound(endTime);

    double peakLoad = 0;
    /* compute the peak load during the interval [startTime, endTime) */
    if (itlow != resrcAllocMap->end()) {
        for (std::map<SimTime,double>::iterator it = itlow; it != itup; it++){

            /* It may happen that last iteration contains an entry at end time and spans for next 5 seconds
             * Since we are not interested in this allocation, ignore it */
            if (it->first == endTime){
                continue;
            }

            if (it->second > peakLoad){
                peakLoad = it->second;
            }
        }
    }
    return peakLoad;
}

NetworkLoadLevel ResourceManager::computeNwLoadLevelDuringRequestedAlloc(int cell, double bwDemand, int direction, simtime_t startTime, simtime_t endTime){
    /* TODO currently we are not considering the bw demanded by the new resource allocation while computing the load. In our current scheme,
     * we make this decision based on resources already allocated in the cell during given time slot */
    double peakLoad = 0;
    double cellCapacity;
    if (direction == 0){ // Uplikn
        cellCapacity = ulCellCapacities->at(cell);
    } else {
        cellCapacity = dlCellCapacities->at(cell);
    }
    peakLoad = computePeakLoadOnGivenCellDuringGivenDuration(cell, direction, startTime, endTime);

    double loadPercentage = peakLoad /cellCapacity;
    if (loadPercentage > HIGH_LOAD_THRESHOLD){
        return HIGH;
    } else if (loadPercentage > MEDIUM_LOAD_THRESHOLD){
        return MEDIUM;
    } else {
        return LOW;
    }
}

double* ResourceManager::getResourceAllocationBundle(double duration, string appType, double throughput, int direction) {
    unsigned int d = (ceil(duration)<UINT_MAX) ? ceil(duration) : UINT_MAX;
    double* allocationThroughputs = new double[d];
    double cellCapacity;
    if (direction == 0){ // Uplink
        cellCapacity = ulCellCapacities->front();
    } else { //Downlink
        cellCapacity = dlCellCapacities->front();
    }

    double* modesList;
    int modes;
    if(appType=="RealTimeVideo") {
        modesList = new double[4];
        modes = 4;
        modesList[0] = REALTIMEHD_HIGH;
        modesList[1] = REALTIMEHD_LOW;
        modesList[2] = REALTIME_HIGH;
        modesList[3] = REALTIME_LOW;
    } else if(appType=="Audio") {
        modesList = new double[3];
        modes = 3;
        modesList[0] = AUDIO_HIGH;
        modesList[1] = AUDIO_STD;
        modesList[2] = AUDIO_LOW;
    } else if(appType=="Video") {
        modesList = new double[5];
        modes = 5;
        modesList[0] = VIDEO_VHIGH*1000;
        modesList[1] = VIDEO_HIGH*1000;
        modesList[2] = VIDEO_STD*1000;
        modesList[3] = VIDEO_MEDIUM*1000;
        modesList[4] = VIDEO_LOW*1000;
    } else {
        modesList = new double[1];
        modes = 1;
        modesList[0] = throughput;
    }

    int startMode = 0;
    while(startMode<modes) {
        if (throughput>=modesList[startMode]) {
            startMode--;
            break;
        }
        startMode++;
    }
    if(startMode<0) startMode = 0;
    if(startMode>modes-1) startMode = modes-1;

    simtime_t currentTime = simTime();
    int t = 0;
    while(t<d) {
        simtime_t tmpTime = currentTime+1+t;
        double peakLoad = computePeakLoadOnGivenCellDuringGivenDuration(0, direction, tmpTime, tmpTime+1 );
        int currMode = startMode;

        while(currMode<modes) {
            if(peakLoad + modesList[currMode] <= cellCapacity)
                break;
            currMode++;
        }

        if(currMode<modes)
            allocationThroughputs[t] = modesList[currMode];
        else
            allocationThroughputs[t] = 0;

        t++;
    }

    delete [] modesList;
    return allocationThroughputs;

}

bool ResourceManager::isResourceAllocationFeasible(double duration, double throughput, int direction) {
    double cellCapacity;
    if (direction == 0){ // Uplink
        cellCapacity = ulCellCapacities->front();
    } else { //Downlink
        cellCapacity = dlCellCapacities->front();
    }
    simtime_t currentTime = simTime();
    // Always compute from the next second
    double peakLoad = computePeakLoadOnGivenCellDuringGivenDuration(0, direction, currentTime + 1, (simtime_t)(currentTime.dbl()+1+ceil(duration)));
    if ((peakLoad + throughput) > cellCapacity){
        return false;
    }
    return true;
}

//double ResourceManager::getProjectedCapacity(BidComponent *bidComponent) {
//    int cell = bidComponent->cell;
//    int direction = bidComponent->direction;
//
//    double cellCapacity;
//    if (direction == uplink){
//        cellCapacity = ulCellCapacities->at(cell);
//    } else {
//        cellCapacity = dlCellCapacities->at(cell);
//    }
//    double peakLoad = computePeakLoadOnGivenCellDuringGivenDuration(cell, direction, bidComponent->startTime, bidComponent->endTime);
//    return cellCapacity - peakLoad;
//}
//
//simtime_t  ResourceManager::getResourceReleaseTime(BidCandidate *winningBid){
//    /* TODO Explore if there is a better mechanism to do this. Probably, sending BW descriptions and bidding on per BW descriptor basis may be
//     * useful in computing the release time. However, in our initial implementation, this mechanism should work
//     */
//    std::list<BidComponent*> *componentList =  winningBid->bidDesc->componentList;
//    simtime_t releaseTime = SimTime(0);
//    for (std::list<BidComponent*>::iterator it = componentList->begin(); it != componentList->end(); it++){
//        BidComponent *bidComponent = (*it);
//        // compute release time as the last instant we spend on last cell according to application description
//        if (bidComponent->endTime > releaseTime){
//            releaseTime = bidComponent->endTime;
//        }
//    }
//
//    return releaseTime;
//}
