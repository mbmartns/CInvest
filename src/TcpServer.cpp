#include "..\include\TcpServer.hpp"
#include <iostream>

TcpServer::TcpServer(int port) : port(port) {}

TcpServer::~TcpServer() {
    stop();
}

bool TcpServer::initWinSock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

bool TcpServer::start() {
    if (!initWinSock()) return false;

    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) return false;


    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Falha no bind\n";
        closesocket(listenSock);
        return false;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Falha no listen\n";
        closesocket(listenSock);
        return false;
    }

    std::cout << "Servidor TCP aguardando conexao na porta " << port << "...\n";

    clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "Falha no accept\n";
        closesocket(listenSock);
        return false;
    }

    std::cout << "Cliente conectado!\n";
    return true;
}

std::string TcpServer::receiveMessage() {
    char buffer[512];
    int bytesReceived = recv(clientSock, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        return std::string(buffer);
    }
    
    return ""; // Retorna string vazia se não houver dados
}

bool TcpServer::sendMessage(const std::string& message) {
    if (clientSock == INVALID_SOCKET) return false;

    // Se a mensagem for "0" ou "1"
    if (message == "0" || message == "1") {
        return send(clientSock, message.c_str(), 1, 0) == 1;
    }

    // Envia mensagens normais (para debug ou outros usos)
    return send(clientSock, message.c_str(), message.size(), 0) != SOCKET_ERROR;
}

void TcpServer::stop() {
    if (clientSock != INVALID_SOCKET) closesocket(clientSock);
    if (listenSock != INVALID_SOCKET) closesocket(listenSock);
    WSACleanup();
}

void TcpServer::cleanup() {
    stop();
}