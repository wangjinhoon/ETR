#include "VRTrackerReader.h"
#include <stdio.h>
#include <windows.h>
#include <Eigen>
#include <cmath>
#include <iostream>
#include <string>
const double PI = std::acos(-1);

float cam_ini_x = 0.0;
float cam_ini_y = 0.0;
float cam_ini_z = 0.0;

// Function to create a 4x4 matrix from rotation and translation
Eigen::Matrix4f composeMatrix(const Eigen::Matrix3f& rotation, const Eigen::Vector3f& translation) {
    Eigen::Matrix4f resultMatrix = Eigen::Matrix4f::Identity();
    resultMatrix.block<3, 3>(0, 0) = rotation;
    resultMatrix.block<3, 1>(0, 3) = translation;
    return resultMatrix;
}

Eigen::Matrix3f createRotationMatrix(float angleY) {
    angleY = angleY * PI / 180.0f;

    Eigen::Matrix3f rotationMatrixY;

    rotationMatrixY << 1, 0, 0,
        0, 0, 1,
        0, -1, 0;

    // Combine the rotation matrices
    Eigen::Matrix3f combinedRotationMatrix = rotationMatrixY;

    return combinedRotationMatrix;
}

Eigen::Matrix4f printSymmetricMatrix(const Matrix4X4& matrix) {
    Eigen::Vector3f delta_displacement(0.0f, 0.0f, -0.05f);
    Eigen::Matrix3f delta_rotation = createRotationMatrix(-90.0f);

    // Convert Matrix4X4 to Eigen::Matrix4f
    Eigen::Matrix4f eigenMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            eigenMatrix(i, j) = matrix.M[j][i];
        }
    }
    

    Eigen::Matrix3f tracker_rotation = eigenMatrix.block<3, 3>(0, 0);

    Eigen::Vector3f tracker_position = eigenMatrix.block<3, 1>(0, 3);

    Eigen::Matrix3f rotation = tracker_rotation * delta_rotation;
    
    Eigen::Vector3f position = tracker_position + ((rotation) * delta_displacement);

    Eigen::Matrix4f resultMatrix = composeMatrix(rotation, position);

    return resultMatrix;
}

void VRTrackerReader::getData(TrackingData& data, uint32_t identifier, std::ofstream& outputFile, float* ini_x, float* ini_y, float* ini_z, bool a) {
    vr::TrackedDevicePose_t devicePose;
    vr::VRControllerState_t controllerState;

    vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);

    data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
    setTrackingResult(data, devicePose.eTrackingResult);

    Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);

    int cam = 6; //here is cam's tracker number
    int ini = 0;

    Quaternion orientation = toQuaternion(pose);
    //basic
    if (identifier == ini && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0 && !a) {
        Eigen::Quaterniond q(orientation.w, orientation.x, orientation.y, orientation.z);

        std::cout << identifier << " x : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS
            << ", y : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS
            << ", z : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS
            << ", q.x : " << std::left << std::setw(10) << orientation.x
            << ", q.y : " << std::left << std::setw(10) << orientation.y
            << ", q.z : " << std::left << std::setw(10) << orientation.z
            << ", q.w : " << std::left << std::setw(10) << orientation.w << std::endl;
    }
    //initilization
    if (identifier == ini && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0 && a) {
        *ini_x = devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS;
        *ini_y = devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS;
        *ini_z = -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS;

        std::cout << " inilization!!!!! " << std::endl;
    }

    //cam_initilization
    else if (identifier == cam && ::cam_ini_x == 0.0 && ::cam_ini_y == 0.0 && ::cam_ini_z == 0.0 && a && *ini_x != 0.0) {
        Eigen::Matrix4f pose1 = printSymmetricMatrix(pose);
        ::cam_ini_x = pose1(0, 3) * METERTOUNREALUNITS - *ini_x;
        ::cam_ini_y = pose1(1, 3) * METERTOUNREALUNITS - *ini_y;
        ::cam_ini_z = -pose1(2, 3) * METERTOUNREALUNITS - *ini_z;

        outputFile << " *cam_x : " << ::cam_ini_x
            << " *cam_y : " << ::cam_ini_y
            << " *cam_z : " << ::cam_ini_z << std::endl;
    }

    //get_cam_data
    if (identifier == cam && a && *ini_x != 0.0) {
        auto currentTimePoint = std::chrono::system_clock::now();
        auto microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(currentTimePoint);

        Eigen::Matrix4f tracker_to_cam_pose = printSymmetricMatrix(pose);

        Quaternion cam_orientation = toQuaternion(tracker_to_cam_pose);
        std::cout << identifier << " x : " << std::left << std::setw(10) << tracker_to_cam_pose(0, 3) * METERTOUNREALUNITS - *ini_x
            << ", y : " << std::left << std::setw(10) << tracker_to_cam_pose(1, 3) * METERTOUNREALUNITS - *ini_y
            << ", z : " << std::left << std::setw(10) << -tracker_to_cam_pose(2, 3) * METERTOUNREALUNITS - *ini_z
            << ", q.x : " << std::left << std::setw(10) << cam_orientation.x
            << ", q.y : " << std::left << std::setw(10) << cam_orientation.y
            << ", q.z : " << std::left << std::setw(10) << cam_orientation.z
            << ", q.w : " << std::left << std::setw(10) << cam_orientation.w << std::endl;
            

        //current time
        std::string number = std::to_string(microseconds.time_since_epoch().count());
        number.pop_back();

        number.insert(number.length() - 5, 1, '.');

        double originalValue = std::stod(number);

        //error turm 2seconed
        double modifiedValue = originalValue - 2.0;

        std::string modifiedString = std::to_string(modifiedValue);


        //data save, coordinate adjustment
        outputFile << modifiedString
            << " " << (-tracker_to_cam_pose(2, 3) * METERTOUNREALUNITS - *ini_z - ::cam_ini_z) / 100
            << " " << -(tracker_to_cam_pose(0, 3) * METERTOUNREALUNITS - *ini_x - ::cam_ini_x) / 100
            << " " << (tracker_to_cam_pose(1, 3) * METERTOUNREALUNITS - *ini_y - ::cam_ini_y) / 100
            << " " << cam_orientation.x
            << " " << -cam_orientation.y
            << " " << cam_orientation.z
            << " " << cam_orientation.w << std::endl;
    }

    //other tracker data position and distance 
    else if (identifier != 0 && a && *ini_x != 0.0) {
        double distance = sqrt(((devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x) * (devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x)) + ((devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y) * (devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y)) + ((-devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z) * (-devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z)));
        std::cout << identifier << " x' : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS - *ini_x
            << " y' : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS - *ini_y
            << " z' : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS - *ini_z
            << ", q.x : " << std::left << std::setw(10) << orientation.x
            << ", q.y : " << std::left << std::setw(10) << orientation.y
            << ", q.z : " << std::left << std::setw(10) << orientation.z
            << ", q.w : " << std::left << std::setw(10) << orientation.w
            << " distance : " << distance << std::endl;
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


    while (true) {
        if (GetKeyState('C') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
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
            getData(data, openVRIdAndUnrealId.openVRID, outputFile, &ini_x, &ini_y, &ini_z, a);
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