#include <iostream>
#include <sstream>
#include "C:\Users\Joao_\Desktop\Programação\Cinvest\CInvest\include\TcpServer.hpp"
#include "C:\Users\Joao_\Desktop\Programação\Cinvest\CInvest\include\Candlestick.hpp"

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
    TcpServer server(5051);

    server.setMessageCallback([](const std::string& msg) {
        try {
            Candlestick candle = parseCandle(msg);
            std::cout << candle.describe();
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar candle: " << e.what() << "\n";
        }
    });

    if (!server.start()) {
        std::cerr << "Erro ao iniciar o servidor\n";
        return 1;
    }

    std::cout << "Servidor ativo. Pressione Enter para encerrar...\n";
    std::cin.get();
    server.stop();
    return 0;
}