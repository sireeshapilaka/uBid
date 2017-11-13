// 
//                           SimuLTE
// 
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself, 
// and cannot be removed from it.
// 


package ubid.corenetwork.nodes;
// 
// User Equipment Module
//
module Ue
{
    parameters:
        @networkNode();
        @display("i=device/pocketpc;bgb=400,518");
		@class(Ue);
        //# Mobility
        //string mobilityType = default("BonnMotionMobility");
        string mobilityType = default("StationaryMobility");
        //# Apps
		string appTrafficConfigFile = default("");
		int userIndex = default(0);
		int dateIndex = default(0);
		bool negotiated = default(true);
		string fileSubPath;
        //# Node specs
        string nodeType = "UE";  // DO NOT CHANGE
        int budget;
}
