#include "../include/AppController.hpp"
#include "../include/utils/CandleParser.hpp"
#include <iostream>
#include <vector>
#include <string>

static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}


AppController::AppController() : server(5050) {
    // Padrões de candlestick
    detector.addPattern(std::make_unique<BearishEngulfingPattern>());
    detector.addPattern(std::make_unique<BearishHaramiPattern>());
    detector.addPattern(std::make_unique<BearishKickPattern>());
    detector.addPattern(std::make_unique<BullishEngulfingPattern>());
    detector.addPattern(std::make_unique<BullishHaramiPattern>());
    detector.addPattern(std::make_unique<BullishKickPattern>());
    detector.addPattern(std::make_unique<DarkCloudCoverPattern>());
    detector.addPattern(std::make_unique<DojiPattern>());
    detector.addPattern(std::make_unique<HammerPattern>());
    detector.addPattern(std::make_unique<HangingManPattern>());
    detector.addPattern(std::make_unique<InvertedHammerPattern>());
    detector.addPattern(std::make_unique<LongDayPattern>());
    detector.addPattern(std::make_unique<MarubozuPattern>());
    detector.addPattern(std::make_unique<PawnPattern>());
    detector.addPattern(std::make_unique<PiercingLinePattern>());
    detector.addPattern(std::make_unique<ShootingStarPattern>());
    detector.addPattern(std::make_unique<ShortDayPattern>());
    detector.addPattern(std::make_unique<ThreeBlackCrowsPattern>());
}

void AppController::run() {
    if (!server.start()) {
        std::cerr << "Erro ao iniciar o servidor.\n";
        return;
    }

    std::cout << "Servidor iniciado. Aguardando dados do MetaTrader...\n";

    while (true) {
        std::string msg = server.receiveMessage();

        if (msg.empty()) {
            std::cout << "Cliente desconectou ou erro na leitura.\n";
            break;
        }

        // === Trata mensagens informativas ===
        size_t sepPos = msg.find('|');
        if (sepPos != std::string::npos) {
            std::string msgType = msg.substr(0, sepPos);
            std::string payload = msg.substr(sepPos + 1);

            if (msgType == "SYMBOL") {
                auto parts = split(payload, '|');
                if (parts.size() == 2) {
                    std::cout << "[SYMBOL] Símbolo: " << parts[0]
                              << ", Timeframe: " << parts[1] << "\n";
                } else {
                    std::cout << "[SYMBOL] Payload inválido: " << payload << "\n";
                }
                continue;
            } else if (msgType == "OPEN") {
                auto parts = split(payload, '|');
                if (parts.size() == 4) {
                    std::cout << "[ORDER OPENED] Tipo: " << parts[0]
                              << ", Volume: " << parts[1]
                              << ", Preço: " << parts[2]
                              << ", Ticket: " << parts[3] << "\n";
                } else {
                    std::cout << "[ORDER OPENED] Payload inválido: " << payload << "\n";
                }
                continue;
            } else if (msgType == "CLOSE") {
                auto parts = split(payload, '|');
                if (parts.size() == 4) {
                    std::cout << "[ORDER CLOSED] Tipo: " << parts[0]
                              << ", Preço: " << parts[1]
                              << ", Lucro: " << parts[2]
                              << ", Ticket: " << parts[3] << "\n";
                } else {
                    std::cout << "[ORDER CLOSED] Payload inválido: " << payload << "\n";
                }
                continue;
            } else if (msgType == "PROFIT") {
                std::cout << "[SESSION PROFIT] Lucro total da sessão: " << payload << "\n";
                continue;
            } else if (msgType == "CANDLE") {
                msg = payload; 
            }
        }

        // === Trata Candle (continua como antes) ===
        try {
            auto candle = CandleParser::fromString(msg);
            candles.push_back(std::make_unique<Candlestick>(candle));

            if (candles.size() > 5) {
                candles.pop_front();
            }

            Candlestick::printList(candles);

            auto detectedPatterns = detector.detect(candles);
            std::string decision = "";

            if (!detectedPatterns.empty()) {
                std::cout << "\n=== Padroes Detectados ===\n";
                for (const auto& pattern : detectedPatterns) {
                    std::cout << "Padrao: " << pattern->getName()
                              << " | Status: " << pattern->getStatus() << "\n";

                    if (pattern->getStatus() == "Vender") {
                        decision = "0";
                        break;
                    } else if (pattern->getStatus() == "Comprar") {
                        decision = "1";
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
            server.sendMessage("2");
        }
    }

    server.stop();
}