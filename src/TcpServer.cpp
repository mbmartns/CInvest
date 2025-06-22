#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <thread> // Adicionado

#include "C:\Users\Joao_\Desktop\Programação\Cinvest\CInvest\include\TcpServer.hpp"
#include <iostream>

TcpServer::TcpServer(int port) : port(port), listenSock_(INVALID_SOCKET), clientSock(INVALID_SOCKET), running(false) {}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::initWinSock() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    return result == 0;
}

void TcpServer::cleanup() {
    if (clientSock != INVALID_SOCKET) closesocket(clientSock);
    if (listenSock_ != INVALID_SOCKET) closesocket(listenSock_);
    WSACleanup();
}

bool TcpServer::start() {
    if (!initWinSock()) return false;

    listenSock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock_ == INVALID_SOCKET) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;
    if (listen(listenSock_, SOMAXCONN) == SOCKET_ERROR) return false;

    running = true;
    serverThread = std::thread(&TcpServer::runServerLoop, this);
    return true;
}

void TcpServer::runServerLoop() {
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(listenSock_, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock == INVALID_SOCKET) return;

    char buffer[512];
    int bytesReceived;

    while (running) {
        bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            if (messageCallback) messageCallback(std::string(buffer));
        } else {
            break;
        }
    }

    running = false;
    cleanup();
}

void TcpServer::stop() {
    running = false;
    if (serverThread.joinable()) serverThread.join();
    cleanup();
}

void TcpServer::setMessageCallback(MessageCallback cb) {
    messageCallback = cb;
}