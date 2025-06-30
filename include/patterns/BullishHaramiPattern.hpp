#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class BullishHaramiPattern : public Pattern {
public:
    std::string getName() const override {
        return "Harami de Fundo";
    }

    std::string getDescription() const override {
        return "Um dos padrões de candle de reversão de alta, ou seja, indica um bom momento de compra.";
    }

    std::string getStatus() const override {
        return "Comprar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 3) return false;

        const auto& c1 = candles[candles.size() - 3];  // Candle 1 (mais antigo)
        const auto& c2 = candles[candles.size() - 2];  // Candle 2 (intermediário)
        const auto& c3 = candles[candles.size() - 1];  // Candle 3 (mais recente)

        double body1 = c1->bodySize();
        double body2 = c2->bodySize();
        double body3 = c3->bodySize();

        // Candle 1: corpo médio, baixa (vermelho)
        bool c1_ok = body1 > 0.5 && c1->isBearish();

        // Candle 2: corpo pequeno, alta (verde)
        bool c2_ok = body2 < 0.5 && c2->isBullish();

        // Candle 3: corpo grande, alta (verde)
        bool c3_ok = body3 > 0.7 && c3->isBullish();

        return c1_ok && c2_ok && c3_ok;
    }
};
