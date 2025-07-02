#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

static SOCKET sock = INVALID_SOCKET;
static bool initialized = false;

extern "C" {

    __declspec(dllexport)
        bool InitConnection() {
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
        void SendCandleData(double open, double high, double low, double close) {
        if (!initialized) {
            if (!InitConnection()) return;
        }

        std::ostringstream oss;
        oss.precision(5);
        oss << std::fixed << open << "," << high << "," << low << "," << close;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport)
        const char* ReceiveCommand() {
        static char buffer[16];  // você só espera "0", "1", ou "2"
        memset(buffer, 0, sizeof(buffer));

        if (!initialized) return "NONE";

        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);

        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // 🧹 Limpa qualquer caractere de quebra de linha ou lixo no final
            for (int i = 0; i < bytesReceived; ++i) {
                if (buffer[i] == '\r' || buffer[i] == '\n') {
                    buffer[i] = '\0';
                    break;
                }
            }

            // DEBUG opcional:
            std::cout << "📥 Comando recebido do C++: [" << buffer << "]\n";

            // Garante que o que chegou é 0 ou 1
            if (strcmp(buffer, "0") == 0 || strcmp(buffer, "1") == 0) {
                return buffer;
            }
            else {
                return "NONE";  // Ignora comando inválido
            }
        }

        return "NONE";
    }

    __declspec(dllexport)
        void CloseConnection() {
        if (initialized) {
            closesocket(sock);
            WSACleanup();
            initialized = false;
        }
    }

}
