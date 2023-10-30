//#include "VRTrackerReader.h"
//#include <stdio.h>
//#include <windows.h>
//
//void VRTrackerReader::getData(TrackingData& data, uint32_t identifier) {
//	vr::TrackedDevicePose_t devicePose;
//	vr::VRControllerState_t controllerState;
//	//std::cout << identifier << std::endl;
//	vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);
//
//	//memcpy(data->angularVelocity, devicePose.vAngularVelocity.v, sizeof(data->angularVelocity));
//	//memcpy(data->velocity, devicePose.vVelocity.v, sizeof(data->velocity));
//	data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
//	setTrackingResult(data, devicePose.eTrackingResult);
//
//	Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);
//
//	if (identifier == 1 || identifier == 4) {
//		Quaternion orientation = toQuaternion(pose);
//		std::cout << " identifier : " << identifier <<  " z : " << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS <<  " x : " << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS << " y : " << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS << " q.x : " << orientation.x << " q.y : " << orientation.y << " q.z : " << orientation.z << " q.w : " << orientation.w <<  std::endl;
//		Sleep(1000); //1초정지
//
//		data.rotation = calculateYPR(orientation);
//	}
//};
//
//void VRTrackerReader::printInformationAboutConnectedDevices() {
//	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//		if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//			std::cout << "Index: " << unDevice << " Class: ";
//			switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//			case vr::TrackedDeviceClass_Controller:
//				std::cout << "Controller ";
//				break;
//			case vr::TrackedDeviceClass_DisplayRedirect:
//				std::cout << "Redirect ";
//				break;
//			case vr::TrackedDeviceClass_GenericTracker:
//				std::cout << "Tracker ";
//				break;
//			case vr::TrackedDeviceClass_HMD:
//				std::cout << "HMD ";
//				break;
//			case vr::TrackedDeviceClass_TrackingReference:
//				std::cout << "Tracking Reference ";
//				break;
//			default:
//				std::cerr << "Error: No valid device class detected!" << std::endl;
//				break;
//			}
//			std::cout << getManufacturerInformation(unDevice) << std::endl;
//		}
//	}
//}
//
//void VRTrackerReader::setTrackingResult(TrackingData& data, vr::ETrackingResult result) {
//	switch (result) {
//	case	vr::TrackingResult_Running_OK:
//		data.trackingResult = TrackingResult_Running_OK;
//		break;
//	case	vr::TrackingResult_Fallback_RotationOnly:
//		data.trackingResult = TrackingResult_Fallback_RotationOnly;
//		break;
//	case	vr::TrackingResult_Running_OutOfRange:
//		data.trackingResult = TrackingResult_Running_OutOfRange;
//		break;
//	case 	vr::TrackingResult_Uninitialized:
//		data.trackingResult = TrackingResult_Uninitialized;
//		break;
//	case	vr::TrackingResult_Calibrating_InProgress:
//		data.trackingResult = TrackingResult_Calibrating_InProgress;
//		break;
//	case	vr::TrackingResult_Calibrating_OutOfRange:
//		data.trackingResult = TrackingResult_Calibrating_OutOfRange;
//		break;
//	}
//}
//
//std::string VRTrackerReader::getManufacturerInformation(vr::TrackedDeviceIndex_t device) {
//	char buf[150];
//	char manufacturer[50];
//	char modelnumber[50];
//	char serialnumber[50];
//	vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ManufacturerName_String, manufacturer, sizeof(manufacturer));
//	vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ModelNumber_String, modelnumber, sizeof(modelnumber));
//	vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, serialnumber, sizeof(serialnumber));
//	sprintf_s(buf, sizeof(buf), "%s - %s [%s]", manufacturer, modelnumber, serialnumber);
//	return std::string(buf);
//}
//
//void VRTrackerReader::createConfigurationFile() {
//	std::ofstream configurationFile;
//	configurationFile.open(FILENAME);
//	uint8_t index = 0;
//	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//		if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//			std::string line;
//			switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//			case vr::TrackedDeviceClass_Controller:
//			case vr::TrackedDeviceClass_GenericTracker:
//			case vr::TrackedDeviceClass_HMD:
//				line = getManufacturerInformation(unDevice) + "=" + (index == MAXTRACKERS ? "-1" : std::to_string(index++));
//				configurationFile << line << std::endl;
//				std::cout << line << std::endl;
//			}
//		}
//	}
//	configurationFile.close();
//	std::cout << "Configuration file created: " << FILENAME << std::endl;
//}
//
//void VRTrackerReader::readConfigurationFile() {
//	std::ifstream configurationFile;
//	configurationFile.open(FILENAME);
//	std::string line;
//	while (std::getline(configurationFile, line)) {
//		size_t pos = 0;
//		pos = line.find(DELIMITER);
//		std::string name = line.substr(0, pos);
//		line.erase(0, pos + DELIMITER.length());
//		int id = std::stoi(line);
//		if (id != -1) {
//			mapDeviceNameToUnrealID.emplace(name, id);
//		}
//	}
//	configurationFile.close();
//	std::cout << "Read configuration file."<< std::endl;
//}
//
//void VRTrackerReader::applyConfiguration() {
//	uint8_t index = 0;
//	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//		if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//			switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//			case vr::TrackedDeviceClass_Controller:
//			case vr::TrackedDeviceClass_GenericTracker:
//			case vr::TrackedDeviceClass_HMD:
//				std::string deviceName = getManufacturerInformation(unDevice);
//				if (mapDeviceNameToUnrealID.find(deviceName) != mapDeviceNameToUnrealID.end()) {
//					OpenVRIDAndUnrealID openVRAndUnrealId;
//					openVRAndUnrealId.openVRID = unDevice;
//					openVRAndUnrealId.unrealID = mapDeviceNameToUnrealID[deviceName];
//					openVRIDAndUnrealIDs.push_back(openVRAndUnrealId);
//				}
//			}
//		}
//	}
//}
//
//void VRTrackerReader::run() {
//
//	vr::EVRInitError eError = vr::VRInitError_None;
//	vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
//	if (eError != vr::VRInitError_None)
//	{
//		vrSystem = NULL;
//		std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
//		exit(0);
//	}
//
//	printInformationAboutConnectedDevices();
//
//	if (configurationMode) {
//		createConfigurationFile();
//		exit(0);
//	}
//
//	readConfigurationFile();
//	applyConfiguration();
//
//	UDPSender sender;
//	sender.initUDPSender();
//
//	while (true) {
//		if (GetKeyState('C') & 0x8000 || GetKeyState('Q') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
//			sender.close();
//			std::cout << "End of program" << std::endl;
//			exit(0);
//		}
//		for (auto& openVRIdAndUnrealId : openVRIDAndUnrealIDs) {
//			std::this_thread::sleep_for(std::chrono::milliseconds(5));
//			TrackingData data;
//			getData(data, openVRIdAndUnrealId.openVRID);
//			sender.sendData(openVRIdAndUnrealId.unrealID, data);
//		}
//	}
//}
//
//int main(int argc, char* argv[])
//{
//	std::cout << "Welcome to OpenVR Tracker Reader!" << std::endl;
//	VRTrackerReader v;
//	if (argc == 2 && std::string(argv[1]) == "-c") {
//		v.configurationMode = true;
//		std::cout << "Will create configuration based of currently connected devices." << std::endl;
//	}
//	v.run();
//}

//#include "VRTrackerReader.h"
//#include <stdio.h>
//#include <windows.h>
//
//void VRTrackerReader::getData(TrackingData& data, uint32_t identifier, std::ofstream& outputFile, float* ini_x, float* ini_y, float* ini_z) {
//    vr::TrackedDevicePose_t devicePose;
//    vr::VRControllerState_t controllerState;
//    //std::cout << identifier << std::endl;
//    vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);
//
//    //memcpy(data->angularVelocity, devicePose.vAngularVelocity.v, sizeof(data->angularVelocity));
//    //memcpy(data->velocity, devicePose.vVelocity.v, sizeof(data->velocity));
//    data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
//    setTrackingResult(data, devicePose.eTrackingResult);
//    //std::cout << "*ini_x : " <<*ini_x << *ini_y << *ini_z << std::endl;
//    if (identifier == 7 && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0){
//        *ini_x = devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS;
//        *ini_y = devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS;
//        *ini_z = -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS;
//        //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
//        outputFile << " *ini_x : " << *ini_x
//            << " *ini_y : " << *ini_y
//            << " *ini_z : " << *ini_z << std::endl;
//    }
//
//
//    Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);
//    //x : -422.182 y : -131.563 z : 35.4936 q.x : -0.0145596 q.y : -0.00448831 q.z : 0.249201 q.w : -0.968332
//    if (identifier == 7) {
//        Quaternion orientation = toQuaternion(pose);
//        // 현재 시간을 얻기
//        auto currentTime = std::chrono::high_resolution_clock::now();
//
//        // 타임스탬프를 밀리초로 변환
//        auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch().count();
//
//        std::cout << timestamp << "      x : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
//                  << " y : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
//                  << " z : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
//                  << ", q.w : " << std::left << std::setw(10) << orientation.w
//                  << ", q.x : " << std::left << std::setw(10) << orientation.x
//                  << ", q.y : " << std::left << std::setw(10) << orientation.y
//                  << ", q.z : " << std::left << std::setw(10) << orientation.z << std::endl;
//
//        outputFile << timestamp << " " << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
//                   << " " << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
//                   << " " << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
//                   << " " << orientation.w
//                   << " " << orientation.x
//                   << " " << orientation.y
//                   << " " << orientation.z << std::endl;
//        Sleep(1000);
//    }
//
//    if (identifier == 1) {
//        Quaternion orientation = toQuaternion(pose);
//        // 현재 시간을 얻기
//        auto currentTime = std::chrono::high_resolution_clock::now();
//
//        // 타임스탬프를 밀리초로 변환
//        auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch().count();
//
//        std::cout << timestamp << "      1x : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
//            << " 1y : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
//            << " 1z : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
//            << ", 1q.w : " << std::left << std::setw(10) << orientation.w
//            << ", 1q.x : " << std::left << std::setw(10) << orientation.x
//            << ", 1q.y : " << std::left << std::setw(10) << orientation.y
//            << ", 1q.z : " << std::left << std::setw(10) << orientation.z << std::endl;
//
//        outputFile << timestamp << " " << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
//            << " " << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
//            << " " << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
//            << " " << orientation.w
//            << " " << orientation.x
//            << " " << orientation.y
//            << " " << orientation.z << std::endl;
//        Sleep(1000);
//    }
//
//};
//
//void VRTrackerReader::printInformationAboutConnectedDevices() {
//    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//            std::cout << "Index: " << unDevice << " Class: ";
//            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//            case vr::TrackedDeviceClass_Controller:
//                std::cout << "Controller ";
//                break;
//            case vr::TrackedDeviceClass_DisplayRedirect:
//                std::cout << "Redirect ";
//                break;
//            case vr::TrackedDeviceClass_GenericTracker:
//                std::cout << "Tracker ";
//                break;
//            case vr::TrackedDeviceClass_HMD:
//                std::cout << "HMD ";
//                break;
//            case vr::TrackedDeviceClass_TrackingReference:
//                std::cout << "Tracking Reference ";
//                break;
//            default:
//                std::cerr << "Error: No valid device class detected!" << std::endl;
//                break;
//            }
//            std::cout << getManufacturerInformation(unDevice) << std::endl;
//        }
//    }
//}
//
//void VRTrackerReader::setTrackingResult(TrackingData& data, vr::ETrackingResult result) {
//    switch (result) {
//    case   vr::TrackingResult_Running_OK:
//        data.trackingResult = TrackingResult_Running_OK;
//        break;
//    case   vr::TrackingResult_Fallback_RotationOnly:
//        data.trackingResult = TrackingResult_Fallback_RotationOnly;
//        break;
//    case   vr::TrackingResult_Running_OutOfRange:
//        data.trackingResult = TrackingResult_Running_OutOfRange;
//        break;
//    case    vr::TrackingResult_Uninitialized:
//        data.trackingResult = TrackingResult_Uninitialized;
//        break;
//    case   vr::TrackingResult_Calibrating_InProgress:
//        data.trackingResult = TrackingResult_Calibrating_InProgress;
//        break;
//    case   vr::TrackingResult_Calibrating_OutOfRange:
//        data.trackingResult = TrackingResult_Calibrating_OutOfRange;
//        break;
//    }
//}
//
//std::string VRTrackerReader::getManufacturerInformation(vr::TrackedDeviceIndex_t device) {
//    char buf[150];
//    char manufacturer[50];
//    char modelnumber[50];
//    char serialnumber[50];
//    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ManufacturerName_String, manufacturer, sizeof(manufacturer));
//    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ModelNumber_String, modelnumber, sizeof(modelnumber));
//    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, serialnumber, sizeof(serialnumber));
//    sprintf_s(buf, sizeof(buf), "%s - %s [%s]", manufacturer, modelnumber, serialnumber);
//    return std::string(buf);
//}
//
//void VRTrackerReader::createConfigurationFile() {
//    std::ofstream configurationFile;
//    configurationFile.open(FILENAME);
//    uint8_t index = 0;
//    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//            std::string line;
//            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//            case vr::TrackedDeviceClass_Controller:
//            case vr::TrackedDeviceClass_GenericTracker:
//            case vr::TrackedDeviceClass_HMD:
//                line = getManufacturerInformation(unDevice) + "=" + (index == MAXTRACKERS ? "-1" : std::to_string(index++));
//                configurationFile << line << std::endl;
//                std::cout << line << std::endl;
//            }
//        }
//    }
//    configurationFile.close();
//    std::cout << "Configuration file created: " << FILENAME << std::endl;
//}
//
//void VRTrackerReader::readConfigurationFile() {
//    std::ifstream configurationFile;
//    configurationFile.open(FILENAME);
//    std::string line;
//    while (std::getline(configurationFile, line)) {
//        size_t pos = 0;
//        pos = line.find(DELIMITER);
//        std::string name = line.substr(0, pos);
//        line.erase(0, pos + DELIMITER.length());
//        int id = std::stoi(line);
//        if (id != -1) {
//            mapDeviceNameToUnrealID.emplace(name, id);
//        }
//    }
//    configurationFile.close();
//    std::cout << "Read configuration file." << std::endl;
//}
//
//void VRTrackerReader::applyConfiguration() {
//    uint8_t index = 0;
//    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
//        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
//            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
//            case vr::TrackedDeviceClass_Controller:
//            case vr::TrackedDeviceClass_GenericTracker:
//            case vr::TrackedDeviceClass_HMD:
//                std::string deviceName = getManufacturerInformation(unDevice);
//                if (mapDeviceNameToUnrealID.find(deviceName) != mapDeviceNameToUnrealID.end()) {
//                    OpenVRIDAndUnrealID openVRAndUnrealId;
//                    openVRAndUnrealId.openVRID = unDevice;
//                    openVRAndUnrealId.unrealID = mapDeviceNameToUnrealID[deviceName];
//                    openVRIDAndUnrealIDs.push_back(openVRAndUnrealId);
//                }
//            }
//        }
//    }
//}
//
//std::string VRTrackerReader::FileCreate() {
//    auto now = std::chrono::system_clock::now();
//    auto in_time_t = std::chrono::system_clock::to_time_t(now);
//
//    std::stringstream filename;
//    filename << std::put_time(std::localtime(&in_time_t), "%d%H%M");
//
//    std::string fileName = "data_" + filename.str() + ".txt";
//
//    return fileName;
//}
//
//void VRTrackerReader::run(std::ofstream& outputFile) {
//
//    float value = 0;
//
//    float ini_x = value;
//    float ini_y = value;
//    float ini_z = value;
//
//    vr::EVRInitError eError = vr::VRInitError_None;
//    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
//    if (eError != vr::VRInitError_None)
//    {
//        vrSystem = NULL;
//        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
//        exit(0);
//    }
//
//    printInformationAboutConnectedDevices();
//
//    if (configurationMode) {
//        createConfigurationFile();
//        exit(0);
//    }
//
//    readConfigurationFile();
//    applyConfiguration();
//
//    UDPSender sender;
//    sender.initUDPSender();
//
//    while (true) {
//        if (GetKeyState('C') & 0x8000 || GetKeyState('Q') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
//            sender.close();
//            std::cout << "End of program" << std::endl;
//            outputFile.close();
//            exit(0);
//        }
//        for (auto& openVRIdAndUnrealId : openVRIDAndUnrealIDs) {
//            std::this_thread::sleep_for(std::chrono::milliseconds(5));
//            TrackingData data;
//            getData(data, openVRIdAndUnrealId.openVRID, outputFile, &ini_x, &ini_y, &ini_z);
//            sender.sendData(openVRIdAndUnrealId.unrealID, data);
//        }
//    }
//}
//
//int main(int argc, char* argv[])
//{
//    std::cout << "Welcome to OpenVR Tracker Reader!" << std::endl;
//    VRTrackerReader v;
//    if (argc == 2 && std::string(argv[1]) == "-c") {
//        v.configurationMode = true;
//        std::cout << "Will create configuration based of currently connected devices." << std::endl;
//    }
//
//
//    std::string fileName = v.FileCreate();
//    std::ofstream outputFile("C:/Users/jh813" + fileName);
//
//    v.run(outputFile);
//}


//////////////////////////////////////////////////////////
#include "VRTrackerReader.h"
#include <stdio.h>
#include <windows.h>

float tr_x = 0;
float tr_y = 0;
float tr_z = 0;
void VRTrackerReader::getData(TrackingData& data, uint32_t identifier, std::ofstream& outputFile, float* ini_x, float* ini_y, float* ini_z, bool a) {
    vr::TrackedDevicePose_t devicePose;
    vr::VRControllerState_t controllerState;
    //std::cout << identifier << std::endl;
    vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);

    //memcpy(data->angularVelocity, devicePose.vAngularVelocity.v, sizeof(data->angularVelocity));
    //memcpy(data->velocity, devicePose.vVelocity.v, sizeof(data->velocity));
    data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
    setTrackingResult(data, devicePose.eTrackingResult);
    //std::cout << "*ini_x : " <<*ini_x << *ini_y << *ini_z << std::endl;


    //initilization
    if (identifier == 7 && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0 && a){
        *ini_x = devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS;
        *ini_y = devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS;
        *ini_z = -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS;
        outputFile << " *ini_x : " << *ini_x
            << " *ini_y : " << *ini_y
            << " *ini_z : " << *ini_z << std::endl;
    }

    Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);

    if (identifier == 7 && a) {
        Quaternion orientation = toQuaternion(pose);
        
        // get current time
        //std::time_t unix_timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());


          // 현재 시간을 나타내는 시간 포인트를 생성
        auto currentTimePoint = std::chrono::system_clock::now();

        // 시간 포인트를 초 단위로 변환
        auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(currentTimePoint);

        // 초 단위로 변환된 시간 포인트를 출력
        std::cout << "Seconds since epoch: " << seconds.time_since_epoch().count() << " seconds." << std::endl;

        // get current time
        auto currentTime = std::chrono::high_resolution_clock::now();

        // transform timestamps to ms
        auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch().count();

        std::cout << timestamp << std::endl;



        // put_time을 사용하여 날짜와 시간 형식 지정하기
        //std::cout << std::put_time(date, "%Y-%m-%d %H:%M:%S") << std::endl;


        std::cout << " x : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
            << ", y : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
            << ", z : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
            << ", q.x : " << std::left << std::setw(10) << orientation.x
            << ", q.y : " << std::left << std::setw(10) << orientation.y
            << ", q.z : " << std::left << std::setw(10) << orientation.z
            << ", q.w : " << std::left << std::setw(10) << orientation.w << std::endl;

        ////data save
        /*outputFile << timestamp  << " " << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
                   << " " << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
                   << " " << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
                   << " " << orientation.x
                   << " " << orientation.y
                   << " " << orientation.z 
                   << " " << orientation.w << std::endl;*/

        //initilization data
        ::tr_x = devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x;
        ::tr_y = devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y;
        ::tr_z = -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z;
    }


    //tracker dist error
    else if(identifier != 0 && a && tr_x != 0){
        double distance = sqrt(((devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x - ::tr_x) * (devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x - ::tr_x)) + ((devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y - ::tr_y) * (devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y - ::tr_y)) + ((-devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z - ::tr_z) * (-devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z - ::tr_z)));


        std::cout << identifier << " x' : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
            << " y' : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
            << " z' : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
            << " distance : " << distance << std::endl;

        Sleep(1000);
    }

};

void VRTrackerReader::printInformationAboutConnectedDevices() {
    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
            std::cout << "Index: " << unDevice << " Class: ";
            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
            case vr::TrackedDeviceClass_Controller:
                std::cout << "Controller ";
                break;
            case vr::TrackedDeviceClass_DisplayRedirect:
                std::cout << "Redirect ";
                break;
            case vr::TrackedDeviceClass_GenericTracker:
                std::cout << "Tracker ";
                break;
            case vr::TrackedDeviceClass_HMD:
                std::cout << "HMD ";
                break;
            case vr::TrackedDeviceClass_TrackingReference:
                std::cout << "Tracking Reference ";
                break;
            default:
                std::cerr << "Error: No valid device class detected!" << std::endl;
                break;
            }
            std::cout << getManufacturerInformation(unDevice) << std::endl;
        }
    }
}

void VRTrackerReader::setTrackingResult(TrackingData& data, vr::ETrackingResult result) {
    switch (result) {
    case   vr::TrackingResult_Running_OK:
        data.trackingResult = TrackingResult_Running_OK;
        break;
    case   vr::TrackingResult_Fallback_RotationOnly:
        data.trackingResult = TrackingResult_Fallback_RotationOnly;
        break;
    case   vr::TrackingResult_Running_OutOfRange:
        data.trackingResult = TrackingResult_Running_OutOfRange;
        break;
    case    vr::TrackingResult_Uninitialized:
        data.trackingResult = TrackingResult_Uninitialized;
        break;
    case   vr::TrackingResult_Calibrating_InProgress:
        data.trackingResult = TrackingResult_Calibrating_InProgress;
        break;
    case   vr::TrackingResult_Calibrating_OutOfRange:
        data.trackingResult = TrackingResult_Calibrating_OutOfRange;
        break;
    }
}

std::string VRTrackerReader::getManufacturerInformation(vr::TrackedDeviceIndex_t device) {
    char buf[150];
    char manufacturer[50];
    char modelnumber[50];
    char serialnumber[50];
    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ManufacturerName_String, manufacturer, sizeof(manufacturer));
    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_ModelNumber_String, modelnumber, sizeof(modelnumber));
    vr::VRSystem()->GetStringTrackedDeviceProperty(device, vr::ETrackedDeviceProperty::Prop_SerialNumber_String, serialnumber, sizeof(serialnumber));
    sprintf_s(buf, sizeof(buf), "%s - %s [%s]", manufacturer, modelnumber, serialnumber);
    return std::string(buf);
}

void VRTrackerReader::createConfigurationFile() {
    std::ofstream configurationFile;
    configurationFile.open(FILENAME);
    uint8_t index = 0;
    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
            std::string line;
            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
            case vr::TrackedDeviceClass_Controller:
            case vr::TrackedDeviceClass_GenericTracker:
            case vr::TrackedDeviceClass_HMD:
                line = getManufacturerInformation(unDevice) + "=" + (index == MAXTRACKERS ? "-1" : std::to_string(index++));
                configurationFile << line << std::endl;
                std::cout << line << std::endl;
            }
        }
    }
    configurationFile.close();
    std::cout << "Configuration file created: " << FILENAME << std::endl;
}

void VRTrackerReader::readConfigurationFile() {
    std::ifstream configurationFile;
    configurationFile.open(FILENAME);
    std::string line;
    while (std::getline(configurationFile, line)) {
        size_t pos = 0;
        pos = line.find(DELIMITER);
        std::string name = line.substr(0, pos);
        line.erase(0, pos + DELIMITER.length());
        int id = std::stoi(line);
        if (id != -1) {
            mapDeviceNameToUnrealID.emplace(name, id);
        }
    }
    configurationFile.close();
    std::cout << "Read configuration file." << std::endl;
}

void VRTrackerReader::applyConfiguration() {
    uint8_t index = 0;
    for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++) {
        if (vrSystem->IsTrackedDeviceConnected(unDevice)) {
            switch (vrSystem->GetTrackedDeviceClass(unDevice)) {
            case vr::TrackedDeviceClass_Controller:
            case vr::TrackedDeviceClass_GenericTracker:
            case vr::TrackedDeviceClass_HMD:
                std::string deviceName = getManufacturerInformation(unDevice);
                if (mapDeviceNameToUnrealID.find(deviceName) != mapDeviceNameToUnrealID.end()) {
                    OpenVRIDAndUnrealID openVRAndUnrealId;
                    openVRAndUnrealId.openVRID = unDevice;
                    openVRAndUnrealId.unrealID = mapDeviceNameToUnrealID[deviceName];
                    openVRIDAndUnrealIDs.push_back(openVRAndUnrealId);
                }
            }
        }
    }
}

std::string VRTrackerReader::FileCreate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream filename;
    filename << std::put_time(std::localtime(&in_time_t), "%d%H%M");

    std::string fileName = "data_" + filename.str() + ".txt";

    return fileName;
}

void VRTrackerReader::run() {

    VRTrackerReader v;

    std::string fileName = v.FileCreate();
    std::ofstream outputFile("C:/Users/jh813" + fileName);

    float ini_x = 0;
    float ini_y = 0;
    float ini_z = 0;

    vr::EVRInitError eError = vr::VRInitError_None;
    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
    {
        vrSystem = NULL;
        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
        exit(0);
    }

    printInformationAboutConnectedDevices();

    if (configurationMode) {
        createConfigurationFile();
        exit(0);
    }

    readConfigurationFile();
    applyConfiguration();

    bool a = FALSE;

    UDPSender sender;
    sender.initUDPSender();

    while (true) {
        if (GetKeyState('C') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
            sender.close();
            std::cout << "End of program" << std::endl;
            outputFile.close();
            exit(0);
        }
        if (GetKeyState('Q') & 0x8000 && a == FALSE) {
            std::cout << "initilization!" << std::endl;
            float ini_x = 0;
            float ini_y = 0;
            float ini_z = 0;

            a = TRUE;
        }

        for (auto& openVRIdAndUnrealId : openVRIDAndUnrealIDs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            TrackingData data;
            getData(data, openVRIdAndUnrealId.openVRID, outputFile, &ini_x, &ini_y, &ini_z, a );
            sender.sendData(openVRIdAndUnrealId.unrealID, data);
        }
    }
}

int main(int argc, char* argv[])
{
    std::cout << "Welcome to OpenVR Tracker Reader!" << std::endl;
    VRTrackerReader v;
    if (argc == 2 && std::string(argv[1]) == "-c") {
        v.configurationMode = true;
        std::cout << "Will create configuration based of currently connected devices." << std::endl;
    }

    v.run();
}