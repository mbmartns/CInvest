#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pch.h"
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

static SOCKET sock = INVALID_SOCKET;
static bool initialized = false;

extern "C" {

    __declspec(dllexport) bool InitConnection() {
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

    __declspec(dllexport) void SendCandleData(double open, double high, double low, double close) {
        if (!initialized && !InitConnection()) return;

        std::ostringstream oss;
        oss.precision(5);
        oss << "CANDLE|" << std::fixed << open << "," << high << "," << low << "," << close;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport) void SendSymbolInfo(const char* symbol, const char* timeframe) {
        if (!initialized && !InitConnection()) return;

        std::string message = "SYMBOL|" + std::string(symbol) + "|" + std::string(timeframe);
        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport) void SendOrderOpened(const char* type, double volume, double price, unsigned long ticket) {
        if (!initialized && !InitConnection()) return;

        std::ostringstream oss;
        oss.precision(5);
        oss << "OPEN|" << type << "|" << std::fixed << volume << "|" << price << "|" << ticket;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport) void SendOrderClosed(const char* type, double price, double profit, unsigned long ticket) {
        if (!initialized && !InitConnection()) return;

        std::ostringstream oss;
        oss.precision(5);
        oss << "CLOSE|" << type << "|" << std::fixed << price << "|" << profit << "|" << ticket;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport) void SendSessionProfit(double profit) {
        if (!initialized && !InitConnection()) return;

        std::ostringstream oss;
        oss.precision(2);
        oss << "PROFIT|" << std::fixed << profit;
        std::string message = oss.str();

        send(sock, message.c_str(), static_cast<int>(message.length()), 0);
    }

    __declspec(dllexport) const char* ReceiveCommand() {
        static char buffer[16];
        memset(buffer, 0, sizeof(buffer));

        if (!initialized) return "NONE";

        u_long mode = 1;
        ioctlsocket(sock, FIONBIO, &mode);

        int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);

        mode = 0;
        ioctlsocket(sock, FIONBIO, &mode);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Clean any trailing newline characters
            for (int i = 0; i < bytesReceived; ++i) {
                if (buffer[i] == '\r' || buffer[i] == '\n') {
                    buffer[i] = '\0';
                    break;
                }
            }

            // Validate command
            if (strcmp(buffer, "0") == 0 || strcmp(buffer, "1") == 0) {
                return buffer;
            }
        }

        return "NONE";
    }

    __declspec(dllexport) void CloseConnection() {
        if (initialized) {
            closesocket(sock);
            WSACleanup();
            initialized = false;
        }
    }
}