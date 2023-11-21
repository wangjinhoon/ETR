#include <rtabmap/core/ICPTransmit.h>

Transmitter::Transmitter() : wsaData_{}, clientSocket_(INVALID_SOCKET), serverSocket_(INVALID_SOCKET) {
 
    struct sockaddr_in server_;
}

Transmitter::~Transmitter() {
    closesocket(clientSocket_);
    closesocket(serverSocket_);
    WSACleanup();
}

void Transmitter::initialize() {

    // Winsock �ʱ�ȭ
    WSAStartup(MAKEWORD(2, 2), &wsaData_);
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);

    // ���� �ּ� ����
    server_.sin_family = AF_INET;
    server_.sin_addr.s_addr = INADDR_ANY;
    server_.sin_port = htons(8888);

    // ���� ���ε� �� ������
    bind(serverSocket_, (struct sockaddr*)&server_, sizeof(server_));
    listen(serverSocket_, 3);

    std::cout << "Waiting for connections..." << std::endl;
    clientSocket_ = accept(serverSocket_, NULL, NULL);
}

void Transmitter::transmit(const std::vector<float>& v) {
    std::string message;
    for (float value : v) {
        message += std::to_string(value) + ",";
    }

    std::cout << "message : " << message << std::endl;

    send(clientSocket_, message.c_str(), message.length(), 0);
}
