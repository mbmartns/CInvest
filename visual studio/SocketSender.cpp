#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

// Socket global e controle de estado
static SOCKET sock = INVALID_SOCKET;
static bool initialized = false;

extern "C" {

    __declspec(dllexport)
    bool InitConnection()
    {
        if (initialized) return true;

        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            return false;
        }

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5050);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            return false;
        }

        initialized = true;
        return true;
    }

    __declspec(dllexport)
    void SendCandleData(double open, double high, double low, double close)
    {
        if (!initialized) {
            if (!InitConnection()) return; // Tenta inicializar se ainda não foi
        }

        std::ostringstream oss;
        oss.precision(5);
        oss << std::fixed << open << "," << high << "," << low << "," << close;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport)
    const char* ReceiveCommand()
    {
        static char buffer[512];
        if (!initialized) return "NONE";

        // Define modo não bloqueante
        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);

        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

        // Volta ao modo bloqueante (opcional, pode deixar não bloqueante se quiser)
        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            return buffer;
        }

        return "NONE";
    }

    __declspec(dllexport)
    void CloseConnection()
    {
        if (initialized) {
            closesocket(sock);
            WSACleanup();
            initialized = false;
        }
    }

}
