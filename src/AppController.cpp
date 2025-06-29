#include "../include/AppController.hpp"
#include "../include/utils/CandleParser.hpp"
#include <iostream>

AppController::AppController() : server(5050) {
    // Padrões de candlestick
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

    std::cout << "Servidor iniciado. Aguardando dados do MetaTrader...\n";

    while (true) {
        std::string msg = server.receiveMessage();
        
        // Verifica se cliente desconectou
        if (msg.empty()) {
            std::cout << "Cliente desconectou ou erro na leitura.\n";
            break;
        }

        try {
            // Processa o candle recebido
            auto candle = CandleParser::fromString(msg);
            candles.push_back(std::make_unique<Candlestick>(candle));
            
            // Mantém apenas os últimos 5 candles
            if (candles.size() > 5) {
                candles.pop_front();
            }

            // Exibe candles atuais
            Candlestick::printList(candles);

            // Detecta padrões
            auto detectedPatterns = detector.detect(candles);
            std::string decision = ""; 

            if (!detectedPatterns.empty()) {
                std::cout << "\n=== Padroes Detectados ===\n";
                for (const auto& pattern : detectedPatterns) {
                    std::cout << "Padrao: " << pattern->getName() 
                              << " | Status: " << pattern->getStatus() << "\n";

                    // Prioriza sinais de VENDA
                    if (pattern->getStatus() == "Vender") {
                        decision = "0"; // SELL
                        break;
                    }
                    else if (pattern->getStatus() == "Comprar") {
                        decision = "1"; // BUY
                        break;
                    }
                }
            } else {
                std::cout << "Nenhum padrao detectado.\n";
            }

            if (decision == "0" || decision == "1") {
                if (!server.sendMessage(decision)) {
                    std::cerr << "Erro ao enviar sinal para o MetaTrader.\n";
                } else {
                    std::cout << "\n[SINAL ENVIADO] "
                              << (decision == "0" ? "SELL (0)" : "BUY (1)")
                              << "\n------------------------\n";
                }
            } else {
                std::cout << "\n[SINAL ENVIADO] NENHUM (vazio)\n------------------------\n";
            }

        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar candle: " << e.what() << "\n";
            server.sendMessage("2"); // Código de erro
        }
    }

    server.stop();
}
