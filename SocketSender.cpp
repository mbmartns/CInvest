// SocketSender.cpp
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")  // Linka a biblioteca Winsock

extern "C" __declspec(dllexport)
void SendCandleData(double open, double high, double low, double close)
{
    // Inicializa o Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    // Define IP e porta do servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5051); // Porta do servidor
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor (localhost)

    // Conecta ao servidor
    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Monta a mensagem: Ex: "1.2345,1.2500,1.2200,1.2400"
    std::ostringstream oss;
    oss.precision(5);
    oss << std::fixed << open << "," << high << "," << low << "," << close;
    std::string message = oss.str();

    // Envia a mensagem
    send(sock, message.c_str(), static_cast<int>(message.length()), 0);

    // Fecha o socket
    closesocket(sock);
    WSACleanup();
}
