#include "UDPSender.h"

void UDPSender::initUDPSender()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Failed to initialize winsock" << std::endl;
		exit(0);
	}

	if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket" << std::endl;
		WSACleanup();
		exit(0);
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(1236);
	inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr));
}

void UDPSender::sendData(uint8_t id, TrackingData& data) {
	float dataToSend[] = { (float)id, data.position[0], data.position[1], data.position[2], data.rotation.roll, data.rotation.pitch, data.rotation.yaw, (float)data.trackingResult };
	sendto(clientSocket, (char*)dataToSend, sizeof(dataToSend), 0, (sockaddr*)&serverAddress, sizeof(serverAddress));
}

void UDPSender::close() {
	closesocket(clientSocket);
	WSACleanup();
}
