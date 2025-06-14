#define _WIN32_WINNT 0x0600  
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <iomanip>


#pragma comment(lib, "ws2_32.lib")  // Linka Winsock

int main()
{
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
    serverAddr.sin_port = htons(5050); // Mesma porta da DLL
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

    std::cout << "Servidor TCP rodando e aguardando conexão na porta 5050...\n";

    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    if(clientSock == INVALID_SOCKET) {
        std::cout << "Accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Cliente conectado!\n";

    // Marca o tempo que a conexão começou (milissegundos desde que o sistema iniciou)
    ULONGLONG startTime = GetTickCount();


    char buffer[512];
    int bytesReceived;

    while (true)
    {
        bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0'; // Termina a string
            std::cout << "Mensagem recebida: " << buffer << "\n";
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Cliente desconectou.\n";
            break;
        }
        else
        {
            std::cout << "Erro no recv: " << WSAGetLastError() << "\n";
            break;
        }
    }

    // Marca o tempo que a conexão terminou
    ULONGLONG endTime = GetTickCount();

    // Calcula tempo decorrido em segundos
    double durationSeconds = (endTime - startTime) / 1000.0;

    std::cout << std::fixed << std::setprecision(3);

    std::cout << "Tempo de conexão: " << durationSeconds << " segundos.\n";

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
