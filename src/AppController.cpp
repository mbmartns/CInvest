#include "../include/AppController.hpp"
#include "../include/utils/CandleParser.hpp"
#include <iostream>

AppController::AppController() : server(5050) {
    detector.addPattern(std::make_unique<HangingManPattern>());
    detector.addPattern(std::make_unique<BearishHaramiPattern>());
    detector.addPattern(std::make_unique<DojiPattern>());
    detector.addPattern(std::make_unique<LongDayPattern>());
    detector.addPattern(std::make_unique<ShortDayPattern>());
    detector.addPattern(std::make_unique<MarubozuPattern>());
    detector.addPattern(std::make_unique<PawnPattern>());
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

            auto detectedPatterns = detector.detect(candles);

            if (!detectedPatterns.empty()) {
                std::cout << "Padroes detectados:\n";
                for (const auto& pattern : detectedPatterns) {
                    std::cout << " - Nome: " << pattern->getName() << "\n";
                    std::cout << "   Descrição: " << pattern->getDescription() << "\n";
                    std::cout << "   Status: " << pattern->getStatus() << "\n\n";
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
