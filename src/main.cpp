#define _WIN32_WINNT 0x0600  
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "..\include\Candlestick.hpp"

#pragma comment(lib, "ws2_32.lib")

Candlestick parseCandle(const std::string& message) {
    std::istringstream iss(message);
    double open, high, low, close;
    char comma;
    if (!(iss >> open >> comma >> high >> comma >> low >> comma >> close)) {
        throw std::runtime_error("Falha ao parsear a mensagem");
    }
    return Candlestick(open, high, low, close);
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(result != 0) {
        std::cout << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenSock == INVALID_SOCKET) {
        std::cout << "Error creating socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5050);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Bind failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    if(listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cout << "Listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Servidor TCP rodando na porta 5050, aguardando conexão...\n";

    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    if(clientSock == INVALID_SOCKET) {
        std::cout << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Cliente conectado!\n";

    char buffer[512];
    int bytesReceived;

    while(true) {
        bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if(bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            try {
                Candlestick candle = parseCandle(std::string(buffer));
                std::cout << candle.describe();
            } catch(const std::exception& e) {
                std::cerr << "Erro ao processar candle: " << e.what() << "\n";
            }

            // Opcional: responder algo para o cliente
            // const char* response = "Candle recebido e processado\n";
            // send(clientSock, response, strlen(response), 0);

        } else if (bytesReceived == 0) {
            std::cout << "Cliente desconectou.\n";
            break;
        } else {
            std::cout << "Erro no recv: " << WSAGetLastError() << "\n";
            break;
        }
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();

    return 0;
}
