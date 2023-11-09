//#pragma once
//
//#include <iostream>
//#include <fstream>
//#include <chrono>
//#include <thread>
//#include <unordered_map>
//#include <stdio.h>
//
//#include "openvr.h"
//#include "Mathematics.h"
//#include "UDPSender.h"
//#include "Structs.h"
//
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
//
//struct OpenVRIDAndUnrealID {
//	uint8_t openVRID;
//	uint8_t unrealID;
//};
//
//class VRTrackerReader {
//private:
//	const static uint8_t MAXTRACKERS = 3;
//	const std::string FILENAME = "VRTrackerReader.ini";
//	const std::string DELIMITER = "=";
//
//	vr::IVRSystem *vrSystem = NULL;
//	std::unordered_map<std::string, uint8_t> mapDeviceNameToUnrealID;
//	std::vector<OpenVRIDAndUnrealID> openVRIDAndUnrealIDs;
//
//public:
//	bool configurationMode = false;
//
//	void run();
//	void printInformationAboutConnectedDevices();
//
//	void getData(TrackingData& data, uint32_t identifier);
//	void setTrackingResult(TrackingData& data, vr::ETrackingResult result);
//
//	std::string getManufacturerInformation(vr::TrackedDeviceIndex_t device);
//
//	void createConfigurationFile();
//	void readConfigurationFile();
//	void applyConfiguration();
//};
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <stdio.h>

#include "openvr.h"
#include "Mathematics.h"
#include "UDPSender.h"
#include "Structs.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

struct OpenVRIDAndUnrealID {
	uint8_t openVRID;
	uint8_t unrealID;
};

class VRTrackerReader {
private:
	const static uint8_t MAXTRACKERS = 6;
	const std::string FILENAME = "VRTrackerReader.ini";
	const std::string DELIMITER = "=";

	vr::IVRSystem* vrSystem = NULL;
	std::unordered_map<std::string, uint8_t> mapDeviceNameToUnrealID;
	std::vector<OpenVRIDAndUnrealID> openVRIDAndUnrealIDs;

public:
	bool configurationMode = false;

	void run();
	void printInformationAboutConnectedDevices();
	std::string FileCreate();
	void getData(TrackingData& data, uint32_t identifier, std::ofstream& outputFile, float* ini_x, float* ini_y, float* ini_z, bool a);
	void setTrackingResult(TrackingData& data, vr::ETrackingResult result);

	std::string getManufacturerInformation(vr::TrackedDeviceIndex_t device);

	void createConfigurationFile();
	void readConfigurationFile();
	void applyConfiguration();
};