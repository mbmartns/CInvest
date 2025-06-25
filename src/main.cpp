#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include "..\include\TcpServer.hpp"
#include "..\include\Candlestick.hpp"

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
    TcpServer server(5050);
    if (!server.start()) {
        std::cerr << "Erro ao iniciar o servidor.\n";
        return 1;
    }

    std::vector<std::unique_ptr<Candlestick>> candles;

    while (true) {
        std::string msg = server.receiveMessage();
        if (msg.empty()) {
            std::cout << "Cliente desconectou ou erro na leitura.\n";
            break;
        }

        try {
            Candlestick candle = parseCandle(msg);
            candles.push_back(std::make_unique<Candlestick>(candle));

            // Agora usamos o método estático para imprimir
            Candlestick::printList(candles);

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar candle: " << e.what() << "\n";
        }
    }

    server.stop();
    return 0;
}
