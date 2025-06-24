#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <string>
#include <functional>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

class TcpServer {
public:
    using MessageCallback = std::function<void(const std::string&)>;

    TcpServer(int port);
    ~TcpServer();

    bool start();
    void stop();

    void setMessageCallback(MessageCallback cb);

private:
    int port;
    SOCKET listenSock = INVALID_SOCKET;
    SOCKET clientSock = INVALID_SOCKET;
    std::thread serverThread;
    bool running = false;

    MessageCallback messageCallback;

    bool initWinSock();
    void cleanup();
    void runServerLoop();
};
