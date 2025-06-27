#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class BullishEngulfingPattern : public Pattern {
public:
    std::string getName() const override {
        return "Engolfo de Alta";
    }

    std::string getDescription() const override {
        return "Formado após uma tendência de baixa, o Engolfo de Alta sinaliza possível reversão de tendência para alta.";
    }

    std::string getStatus() const override {
        return "Comprar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& c1 = candles[candles.size() - 2];  // Candle 1 (vermelho)
        const auto& c2 = candles[candles.size() - 1];  // Candle 2 (verde)

        // Candle 1: corpo pequeno e de baixa
        bool c1_ok = c1->isBearish() && c1->bodySize() < 0.5;

        // Candle 2: corpo maior e de alta
        bool c2_ok = c2->isBullish() && c2->bodySize() > c1->bodySize();

        // Verifica se o segundo candle "engolfa" o primeiro
        bool engulfing =
            c2->getOpen() < c1->getClose() &&
            c2->getClose() > c1->getOpen();

        return c1_ok && c2_ok && engulfing;
    }
};
