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

#ifndef CORENETWORK_NEGOTIATION_RESOURCEMANAGER_H_
#define CORENETWORK_NEGOTIATION_RESOURCEMANAGER_H_

#include <stdexcept>
#include <omnetpp.h>
#include <string.h>
using namespace std;
using namespace omnetpp;
#include <util/ModeConstants.h>
//typedef struct _bidCandidate{
//    BidDescriptor *bidDesc;
//    int seqNum;
//    const char* userId;
//    double bidPrice = 0; // $/kb
//}BidCandidate;

enum NetworkLoadLevel {LOW, MEDIUM, HIGH};
#define HIGH_LOAD_THRESHOLD 0.70
#define MEDIUM_LOAD_THRESHOLD 0.50

typedef std::map<SimTime, double> CellResourceAllocMap;
typedef std::map<SimTime, double> CellAllocationsCountMap;

class ResourceManager {


public:
    //private:
    // Map of resources allocated to each cell at various points in time. Cell ID is the index in the vector
    std::vector<CellResourceAllocMap*> *ulCellRsrcAllocMapList;
    std::vector<CellResourceAllocMap*> *dlCellRsrcAllocMapList;
    std::vector<CellAllocationsCountMap*> *ulCellAllocCountMapList;
    std::vector<CellAllocationsCountMap*> *dlCellAllocCountMapList;
    // Vector of UL and DL capacities of each cell present in this cell network.
    std::vector<double> *ulCellCapacities;
    std::vector<double> *dlCellCapacities;
    cOutVector downlinkCapacities[6];
    cOutVector uplinkCapacities[6];
    cOutVector numUplinkAllocations;
    cOutVector numDownlinkAllocations;

    cOutVector uplinkDemandVisualization;
    cOutVector downlinkDemandVisualization;
    cOutVector uplinkDemandCountVisualization;
    cOutVector downlinkDemandCountVisualization;

    cOutVector uplinkDemandBurstyVisualization;
    cOutVector downlinkDemandBurstyVisualization;
    cOutVector uplinkDemandCountBurstyVisualization;
    cOutVector downlinkDemandCountBurstyVisualization;

    cOutVector uplinkDemandUDPVisualization;
    cOutVector downlinkDemandUDPVisualization;
    cOutVector uplinkDemandCountUDPVisualization;
    cOutVector downlinkDemandCountUDPVisualization;

    cOutVector downlinkDemandAudioVisualization;
    cOutVector downlinkDemandVideoVisualization;
    cOutVector downlinkDemandCountAudioVisualization;
    cOutVector downlinkDemandCountVideoVisualization;


    CellResourceAllocMap* uplinkUDPMap = new CellResourceAllocMap();
    CellResourceAllocMap* downlinkUDPMap = new CellResourceAllocMap();
    CellAllocationsCountMap* ulUDPCount = new CellAllocationsCountMap();
    CellAllocationsCountMap* dlUDPCount = new CellAllocationsCountMap();

    CellResourceAllocMap* uplinkBurstyMap = new CellResourceAllocMap();
    CellResourceAllocMap* downlinkBurstyMap = new CellResourceAllocMap();
    CellAllocationsCountMap* ulBurstyCount = new CellAllocationsCountMap();
    CellAllocationsCountMap* dlBurstyCount = new CellAllocationsCountMap();

    CellResourceAllocMap* downlinkVideoMap = new CellResourceAllocMap();
    CellAllocationsCountMap* dlVideoCount = new CellAllocationsCountMap();

    CellResourceAllocMap* downlinkAudioMap = new CellResourceAllocMap();
    CellAllocationsCountMap* dlAudioCount = new CellAllocationsCountMap();

    void ReserveResources(double *uplinkBandwidth, double *downlinkBandwidth, int uplinkDuration, int downlinkDuration, bool startSameTime, simtime_t startTime);
    double computePeakLoadOnGivenCellDuringGivenDuration(int cell, int direction, simtime_t timeSlot);

    // end private
    ResourceManager();
    virtual ~ResourceManager();
    int getNumTowers();
    void AddTower(double ulCapacity, double dlCapacity);
    void clearElapsedEntries();
    void clearAllEntries();
    void recordStats();
    double* getResourceAllocationBundle(int duration, string appType, double throughput, int direction);
};

#endif /* RESOURCEMANAGER_H_ */
