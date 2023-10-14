#include "VRTrackerReader.h"
#include <stdio.h>
#include <windows.h>

void VRTrackerReader::getData(TrackingData& data, uint32_t identifier) {
	vr::TrackedDevicePose_t devicePose;
	vr::VRControllerState_t controllerState;
	//std::cout << identifier << std::endl;
	vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);
		
	//memcpy(data->angularVelocity, devicePose.vAngularVelocity.v, sizeof(data->angularVelocity));
	//memcpy(data->velocity, devicePose.vVelocity.v, sizeof(data->velocity));
	data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
	setTrackingResult(data, devicePose.eTrackingResult);

	Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);

	if (identifier == 3) {
		std::cout << "z : " << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS << std::endl;
		std::cout << "x : " << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS << std::endl;
		std::cout << "y : " << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS << std::endl;
		Sleep(1000); //1ÃÊÁ¤Áö
	}

	Quaternion orientation = toQuaternion(pose);


	//std::cout << "q.x : " << orientation.x << std::endl;
	//std::cout << "q.y : " << orientation.y << std::endl;
	//std::cout << "q.z : " << orientation.z << std::endl;
	//std::cout << "q.w : " << orientation.w << std::endl;

	data.rotation = calculateYPR(orientation);
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
	case	vr::TrackingResult_Running_OK:
		data.trackingResult = TrackingResult_Running_OK;
		break;
	case	vr::TrackingResult_Fallback_RotationOnly:
		data.trackingResult = TrackingResult_Fallback_RotationOnly;
		break;
	case	vr::TrackingResult_Running_OutOfRange:
		data.trackingResult = TrackingResult_Running_OutOfRange;
		break;
	case 	vr::TrackingResult_Uninitialized:
		data.trackingResult = TrackingResult_Uninitialized;
		break;
	case	vr::TrackingResult_Calibrating_InProgress:
		data.trackingResult = TrackingResult_Calibrating_InProgress;
		break;
	case	vr::TrackingResult_Calibrating_OutOfRange:
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
	std::cout << "Read configuration file."<< std::endl;
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

void VRTrackerReader::run() {

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

	UDPSender sender;
	sender.initUDPSender();

	while (true) {
		if (GetKeyState('C') & 0x8000 || GetKeyState('Q') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
			sender.close();
			std::cout << "End of program" << std::endl;
			exit(0);
		}
		for (auto& openVRIdAndUnrealId : openVRIDAndUnrealIDs) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			TrackingData data;
			getData(data, openVRIdAndUnrealId.openVRID);
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
