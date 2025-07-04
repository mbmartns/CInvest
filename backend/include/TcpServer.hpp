#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class TcpServer {
public:
    TcpServer(int port);
    ~TcpServer();

    bool start();
    std::string receiveMessage();
    bool sendMessage(const std::string& message);
    void stop();

private:
    int port;
    SOCKET listenSock = INVALID_SOCKET;
    SOCKET clientSock = INVALID_SOCKET;

    bool initWinSock();
    void cleanup();
};
