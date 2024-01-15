#include "VRTrackerReader.h"
#include <stdio.h>
#include <windows.h>
#include <Eigen>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>

float cam_ini_x = 0.0;
float cam_ini_y = 0.0;
float cam_ini_z = 0.0; 
Eigen::Matrix4f ini_rotationMatrix;

// Function to create a 4x4 matrix from rotation and translation
Eigen::Matrix4f composeMatrix(const Eigen::Matrix3f& rotation, const Eigen::Vector3f& translation) {
    Eigen::Matrix4f resultMatrix = Eigen::Matrix4f::Identity();
    resultMatrix.block<3, 3>(0, 0) = rotation;
    resultMatrix.block<3, 1>(0, 3) = translation;
    return resultMatrix;
}


// 트래커를 카메라 좌표계와 일치시키기 위한 로데이텬 매트릭스 생성
Eigen::Matrix3f createRotationMatrix() {

    Eigen::Matrix3f rotationMatrixY;

    rotationMatrixY << 1, 0, 0,
                       0, 0, 1,
                       0, -1, 0;

    Eigen::Matrix3f combinedRotationMatrix = rotationMatrixY;

    return combinedRotationMatrix;
}

// 현재 트래커를 카메라 좌표계로 변환하기 위한 매트릭스 생성
Eigen::Matrix4f T2C_coordinate(const Matrix4X4& matrix) {
    Eigen::Vector3f delta_displacement(0.0f, 0.0f, -0.05f);//트래커를 카메라 위치로 이동, z축으로 -5cm
    Eigen::Matrix3f delta_rotation = createRotationMatrix();//카메라 좌표계와 회전축을 맞추기 위한 행렬 생성
                                                            //1, 0, 0
                                                            //0, 0, 1
                                                            //0, -1, 0 트래커의 로테이션 행렬이 이와 같은 값을 가지고 있어 역행렬을 곱해주어 카메라 좌표계와 일치하게끔 변환
    

    // Convert Matrix4X4 to Eigen::Matrix4f, 계산을 위해  Eigen 형태로 변환
    Eigen::Matrix4f eigenMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            eigenMatrix(i, j) = matrix.M[j][i];
        }
    }
    

    Eigen::Matrix3f tracker_rotation = eigenMatrix.block<3, 3>(0, 0);

    Eigen::Vector3f tracker_position = eigenMatrix.block<3, 1>(0, 3);

    Eigen::Matrix3f rotation = tracker_rotation * delta_rotation;
    
    Eigen::Vector3f position = tracker_position + ((rotation) * delta_displacement);// 카메라 좌표계로 변환

    Eigen::Matrix4f resultMatrix = composeMatrix(rotation, position);

    return resultMatrix;
}

// 현재 트래커를 카메라 좌표계로 변환하기 위한 매트릭스 생성, 위 함수와 input 형식이 다름
Eigen::Matrix4f T2C_coordinate(const Eigen::Matrix4f& matrix) {
     
    Eigen::Vector3f delta_displacement(0.0f, 0.0f, -0.05f);//트래커를 카메라 위치로 이동, z축으로 -5cm
    Eigen::Matrix3f delta_rotation = createRotationMatrix();//카메라 좌표계와 회전축을 맞추기 위한 행렬 생성 
                                                            //1, 0, 0
                                                            //0, 0, 1
                                                            //0, -1, 0 트래커의 로테이션 행렬이 이와 같은 값을 가지고 있어 역행렬을 곱해주어 카메라 좌표계와 일치하게끔 변환

    Eigen::Matrix3f tracker_rotation = matrix.block<3, 3>(0, 0);

    Eigen::Vector3f tracker_position = matrix.block<3, 1>(0, 3);

    Eigen::Matrix3f rotation = tracker_rotation * delta_rotation;

    Eigen::Vector3f position = tracker_position + ((rotation)*delta_displacement);// 카메라 좌표계로 변환

    Eigen::Matrix4f resultMatrix = composeMatrix(rotation, position);

    return resultMatrix;
}

void VRTrackerReader::getData(TrackingData& data, uint32_t identifier, std::ofstream& outputFile, float* ini_x, float* ini_y, float* ini_z, bool a, int cam_input, int ini_input) {
    vr::TrackedDevicePose_t devicePose;
    vr::VRControllerState_t controllerState;

    //현재 장치의 Pose 값 받아옴
    vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, identifier, &controllerState, sizeof(controllerState), &devicePose);

    data.position = getPosition(devicePose.mDeviceToAbsoluteTracking);
    //setTrackingResult(data, devicePose.eTrackingResult);

    // Matrix4X4 형으로 변환
    Matrix4X4 pose(devicePose.mDeviceToAbsoluteTracking);

    int cam = cam_input;
    int ini = ini_input;

    // pose 변수로 부터 쿼터니언 값 추출
    Quaternion orientation = toQuaternion(pose);
    //초기점 좌표로 부터 초기화용 트래커 까지의 거리를 출력  
    if (identifier == ini && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0 && !a) {
        //Eigen::Quaterniond q(orientation.w, orientation.x, orientation.y, orientation.z);

        std::cout << identifier << "ini_before => x : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS
            << ", y : " << std::left << std::setw(10) << devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS
            << ", z : " << std::left << std::setw(10) << -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS
            << ", q.x : " << std::left << std::setw(10) << orientation.x
            << ", q.y : " << std::left << std::setw(10) << orientation.y
            << ", q.z : " << std::left << std::setw(10) << orientation.z
            << ", q.w : " << std::left << std::setw(10) << orientation.w << std::endl;
    }
    //초기화용 트래커를 원점으로 만들기 위해 현재 초기화용 트래커의 x, y, z, 4x4 매트릭스 값 저장 -> 한번만 작동
    if (identifier == ini && *ini_x == 0.0 && *ini_y == 0.0 && *ini_z == 0.0 && a) {
        *ini_x = devicePose.mDeviceToAbsoluteTracking.m[0][3] * METERTOUNREALUNITS;
        *ini_y = devicePose.mDeviceToAbsoluteTracking.m[1][3] * METERTOUNREALUNITS;
        *ini_z = -devicePose.mDeviceToAbsoluteTracking.m[2][3] * METERTOUNREALUNITS;
        // 초기화용 매트릭스의 R, T를 ini_rotationMatrix에 저장
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                ::ini_rotationMatrix(i, j) = pose.M[j][i];
            }
        }
        std::cout << " inilization!!!!! " << " *ini_x : " << *ini_x
            << " *ini_y : " << *ini_y
            << " *ini_z : " << *ini_z << std::endl;
    }

    //카메라의 시작 위치 저장용 트래커의 원점 기준의 x, y, z 저장 -> 한번만 작동
    else if (identifier == cam && ::cam_ini_x == 0.0 && ::cam_ini_y == 0.0 && ::cam_ini_z == 0.0 && a && *ini_x != 0.0) {
        Eigen::Matrix4f convert_eigenMatrix;
        //매트릭스 형태 변환
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                convert_eigenMatrix(i, j) = pose.M[j][i];
            }
        }


        Eigen::Matrix4f T_new;
        T_new = ::ini_rotationMatrix.inverse() * convert_eigenMatrix;//이니셜 트래커를 기준으로하기 위해 이니셜 트래커의 역행렬을 곱해줌
        
        Eigen::Matrix4f pose1 = T2C_coordinate(T_new);
        //변환된 원점으로 부터 카메라 초기 위치 저장
        ::cam_ini_x = pose1(0, 3) * METERTOUNREALUNITS;
        ::cam_ini_y = pose1(1, 3) * METERTOUNREALUNITS;
        ::cam_ini_z = -pose1(2, 3) * METERTOUNREALUNITS;

        outputFile << " *cam_x : " << ::cam_ini_x/100 //유니티와 단위를 맞추기 위해 100을 나눠줌
                    << " *cam_y : " << ::cam_ini_y/100
                    << " *cam_z : " << ::cam_ini_z/100 << std::endl;
    }

    //카메라 추적용 트래커의 위치 데이터를 받아옴
    if (identifier == cam && a && *ini_x != 0.0) {
        auto currentTimePoint = std::chrono::system_clock::now();
        auto microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(currentTimePoint);

        //매트릭스 형태 변환
        Eigen::Matrix4f convert_eigenMatrix;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                convert_eigenMatrix(i, j) = pose.M[j][i];
            }
        }


        Eigen::Matrix4f T_new;
        T_new = ::ini_rotationMatrix.inverse() * convert_eigenMatrix;//이니셜 트래커를 기준으로하기 위해 이니셜 트래커의 역행렬을 곱해줌
        Eigen::Matrix4f tracker_to_cam_pose = T2C_coordinate(T_new);

        Quaternion cam_orientation = toQuaternion(tracker_to_cam_pose);
        std::cout << "cam_tracker : " << identifier << " x : " << std::left << std::setw(10) << tracker_to_cam_pose(0, 3) * METERTOUNREALUNITS
            << ", y : " << std::left << std::setw(10) << tracker_to_cam_pose(1, 3) * METERTOUNREALUNITS
            << ", z : " << std::left << std::setw(10) << -tracker_to_cam_pose(2, 3) * METERTOUNREALUNITS
            << ", q.x : " << std::left << std::setw(10) << cam_orientation.x
            << ", q.y : " << std::left << std::setw(10) << cam_orientation.y
            << ", q.z : " << std::left << std::setw(10) << cam_orientation.z
            << ", q.w : " << std::left << std::setw(10) << cam_orientation.w << std::endl;
            

        //rtabmap의 타임스탬프와 같은 형태로 변환, 1970년 1월 1일 0시 0분 0초를 기준으로 경과한 시간을 초 단위로 표현한 값, Unix timestamp
        std::string number = std::to_string(microseconds.time_since_epoch().count());
        number.pop_back();

        number.insert(number.length() - 5, 1, '.');

        double originalValue = std::stod(number);

        //error turm 2seconed
        double modifiedValue = originalValue - 2.0;

        std::string modifiedString = std::to_string(modifiedValue);


        //data save, coordinate adjustment, 카메라의 회전축과 부호가 반대인 부분이 있어 맞춰줌
        // 0, 0, 0 부터 시작해야함으로 카메라 초기위치값 만큼 빼줌
        outputFile << modifiedString
            << " " << (-tracker_to_cam_pose(2, 3) * METERTOUNREALUNITS - ::cam_ini_z) / 100
            << " " << -(tracker_to_cam_pose(0, 3) * METERTOUNREALUNITS - ::cam_ini_x) / 100
            << " " << (tracker_to_cam_pose(1, 3) * METERTOUNREALUNITS - ::cam_ini_y) / 100
            << " " << cam_orientation.x
            << " " << -cam_orientation.y
            << " " << cam_orientation.z
            << " " << cam_orientation.w << std::endl;
    }

    //카메라용 트래커가 아닌 다른 트래커의 위치 및 거리값을 받아옴 
    else if (identifier != 0 && a && *ini_x != 0.0) {
        // Convert Matrix4X4 to Eigen::Matrix4f
        Eigen::Matrix4f convert_eigenMatrix;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                convert_eigenMatrix(i, j) = pose.M[j][i];
            }
        }


        Eigen::Matrix4f T_new;
        T_new = ::ini_rotationMatrix.inverse() * convert_eigenMatrix;//이니셜 트래커를 기준으로하기 위해 이니셜 트래커의 역행렬을 곱해줌
        Quaternion orientation = toQuaternion(T_new);

        //원점으로 부터 현재 트래커 까지의 거리를 계산 
        double distance = sqrt(((T_new.coeff(0,3) * METERTOUNREALUNITS) * (T_new.coeff(0, 3) * METERTOUNREALUNITS)) + ((T_new.coeff(1, 3) * METERTOUNREALUNITS) * (T_new.coeff(1, 3) * METERTOUNREALUNITS)) + ((-(T_new.coeff(2, 3)) * METERTOUNREALUNITS) * (-(T_new.coeff(2, 3)) * METERTOUNREALUNITS)));
        //트래커의 x, y, z, q.x, q.y, q.z, q.w, 거리 값 출력
        std::cout << "other_tracker : " << identifier << " x' : " << std::left << std::setw(10) << T_new.coeff(0, 3) * METERTOUNREALUNITS
            << " y' : " << std::left << std::setw(10) << T_new.coeff(1, 3) * METERTOUNREALUNITS
            << " z' : " << std::left << std::setw(10) << -T_new.coeff(2, 3) * METERTOUNREALUNITS
            << ", q.x : " << std::left << std::setw(10) << orientation.x
            << ", q.y : " << std::left << std::setw(10) << orientation.y
            << ", q.z : " << std::left << std::setw(10) << orientation.z
            << ", q.w : " << std::left << std::setw(10) << orientation.w
            << " distance : " << distance << std::endl;
    }
};

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

//카메라 추적용 데이터의 정보를 저장하기 위한 파일을 생성하는 함수
std::string VRTrackerReader::FileCreate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream filename;
    filename << std::put_time(std::localtime(&in_time_t), "%d%H%M");

    std::string fileName = "data_" + filename.str() + ".txt";

    return fileName;
}

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

//config 파일에 현재 연결된 트래커들을 등록하는 함수
void VRTrackerReader::run() {

    vr::EVRInitError eError = vr::VRInitError_None;
    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
    {
        vrSystem = NULL;
        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
        exit(0);
    }

    if (configurationMode) {
        createConfigurationFile();
        exit(0);
    }
}


//트래커 정보를 받아 올 수 있는 메인 함수
void VRTrackerReader::run(int cam_input_, int ini_input_) {

    VRTrackerReader v;

    //트래커 정보 저장용 파일 생성
    std::string fileName = v.FileCreate();
    std::ofstream outputFile("C:/Users/jh813" + fileName);

    //트래커 초기화 때 사용할 변수 생성
    int cam_input = cam_input_;
    int ini_input = ini_input_;
    float ini_x = 0;
    float ini_y = 0;
    float ini_z = 0;


    //VR 시스템 초기화 및 발생하는 오류 처리
    vr::EVRInitError eError = vr::VRInitError_None;
    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
    {
        vrSystem = NULL;
        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
        exit(0);
    }

    //Config 파일로 현재 연결된 장비 구성
    readConfigurationFile();
    applyConfiguration();

    bool a = FALSE;


    while (true) {
        //프로그램 종료 용 
        if (GetKeyState('C') & 0x8000 || GetKeyState(VK_ESCAPE) & 0x8000) {
            std::cout << "End of program" << std::endl;
            outputFile.close();
            exit(0);
        }
        //초기화 용 
        if (GetKeyState('Q') & 0x8000 && a == FALSE) {
            std::cout << "initilization!" << std::endl;
            float ini_x = 0;
            float ini_y = 0;
            float ini_z = 0;

            a = TRUE;
        }

        //현재 연결된 장치를 하나씩 getData에 입력으로 넣어줌
        for (auto& openVRIdAndUnrealId : openVRIDAndUnrealIDs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            
            TrackingData data;
            getData(data, openVRIdAndUnrealId.openVRID, outputFile, &ini_x, &ini_y, &ini_z, a, cam_input, ini_input);
        }
    }
}


//현재 연결된 디바이스들의 정보를 출력하는 함수
void VRTrackerReader::printInformationAboutConnectedDevice(){
    VRTrackerReader v;

    vr::EVRInitError eError = vr::VRInitError_None;
    vrSystem = vr::VR_Init(&eError, vr::VRApplication_Background);
    if (eError != vr::VRInitError_None)
    {
        vrSystem = NULL;
        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
        exit(0);
    }

    printInformationAboutConnectedDevices();
}


int main(int argc, char* argv[])
{
    std::cout << "Welcome to OpenVR Tracker Reader!" << std::endl;
    VRTrackerReader v;
    if (argc == 2 && std::string(argv[1]) == "-c") {
        v.configurationMode = true;
        std::cout << "Will create configuration based of currently connected devices." << std::endl;

        v.run();
    }
    else if (argc == 2 && std::string(argv[1]) == "-a") {
        v.printInformationAboutConnectedDevice();
        return 1;
    }
    else if (argc == 2) {
        std::cout << "exe, cam_input, ini_input" << std::endl;
        return 1;
    }
    else if (argc == 3){
        int cam_input = std::atoi(argv[1]);
        int ini_input = std::atoi(argv[2]);

        v.run(cam_input, ini_input);
    }
}