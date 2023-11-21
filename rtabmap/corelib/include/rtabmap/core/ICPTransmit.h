// #pragma once

// #pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

class Transmitter {
public:
    WSADATA wsaData_;
    SOCKET serverSocket_;
    SOCKET clientSocket_;
    struct sockaddr_in server_;
    Transmitter();
    ~Transmitter();

    void initialize();
    void transmit(const std::vector<float>& v);

};