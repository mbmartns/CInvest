#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

inline bool sendHttpPostToFlask(const std::string& json) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    int port = 3001;
    const char* hostname = "127.0.0.1";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return false;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, hostname, &serverAddr.sin_addr);

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        closesocket(sock);
        WSACleanup();
        return false;
    }

    std::string request =
        "POST /api/candle HTTP/1.1\r\n"
        "Host: 127.0.0.1:3001\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(json.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        json;

    int sent = send(sock, request.c_str(), (int)request.size(), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "Send failed\n";
        closesocket(sock);
        WSACleanup();
        return false;
    }

    char buffer[1024];
    int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        // std::cout << "Resposta do Flask:\n" << buffer << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return true;
}