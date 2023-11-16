#pragma once

#include <iostream>
#include <vector>
#include "Ws2tcpip.h"
#include <winsock2.h>
#include <Windows.h>

#include "Structs.h"

enum TrackingResult {
	TrackingResult_Uninitialized = 1,
	TrackingResult_Calibrating_InProgress = 100,
	TrackingResult_Calibrating_OutOfRange = 101,
	TrackingResult_Running_OK = 200,
	TrackingResult_Running_OutOfRange = 201,
	TrackingResult_Fallback_RotationOnly = 300,
};

struct TrackingData {
	std::vector<float> position;
	RPY rotation;
	//float velocity[3];				//velocity in tracker space in m / s
	//float angularVelocity[3];		//angular velocity in radians / s(? )
	TrackingResult trackingResult;	//(Calibrating, Running Ok, Uninitialized, Fallback_RotationOnly, ...)
};

class UDPSender
{
private:
	const int port = 1236;
	SOCKET clientSocket;
	sockaddr_in serverAddress;
public:
	void sendData(uint8_t id, TrackingData& data);
	void initUDPSender();
	void close();
};

