#include "../include/AppController.hpp"
#include "../include/utils/CandleParser.hpp"
#include <iostream>

AppController::AppController() : server(5050) {
    detector.addPattern(std::make_unique<HangingManPattern>());
    detector.addPattern(std::make_unique<BearishHaramiPattern>());
}

void AppController::run() {
    if (!server.start()) {
        std::cerr << "Erro ao iniciar o servidor.\n";
        return;
    }

    while (true) {
        std::string msg = server.receiveMessage();
        if (msg.empty()) {
            std::cout << "Cliente desconectou ou erro na leitura.\n";
            break;
        }

        try {
            auto candle = CandleParser::fromString(msg);
            candles.push_back(std::make_unique<Candlestick>(candle));
            if (candles.size() > 5) {
                candles.pop_front();
            }

            Candlestick::printList(candles);

            auto detected = detector.detect(candles);

            if (!detected.empty()) {
                std::cout << "Padroes detectados:\n";
                for (const auto& p : detected) {
                    std::cout << " - " << p << "\n";
                }
            } else {
                std::cout << "Nenhum padrao detectado.\n";
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar candle: " << e.what() << "\n";
        }
    }

    server.stop();
}
