#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include "..\include\TcpServer.hpp"
#include "..\include\Candlestick.hpp"
#include "..\include\PatternDetector.hpp"
#include "..\include\patterns\HangingManPattern.hpp"

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

    PatternDetector detector;
    detector.addPattern(std::make_unique<HangingManPattern>());
    // Adicione outros padrões aqui conforme desejar

    while (true) {
        std::string msg = server.receiveMessage();
        if (msg.empty()) {
            std::cout << "Cliente desconectou ou erro na leitura.\n";
            break;
        }

        try {
            Candlestick candle = parseCandle(msg);
            candles.push_back(std::make_unique<Candlestick>(candle));

            Candlestick::printList(candles);

            auto detectedPatterns = detector.detect(candles);

            if (!detectedPatterns.empty()) {
                std::cout << "Padrões detectados:\n";
                for (const auto& patternName : detectedPatterns) {
                    std::cout << " - " << patternName << "\n";
                }
            } else {
                std::cout << "Nenhum padrão detectado.\n";
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar candle: " << e.what() << "\n";
        }
    }

    server.stop();
    return 0;
}
