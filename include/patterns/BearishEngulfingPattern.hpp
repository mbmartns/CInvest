#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class BearishEngulfingPattern : public Pattern {
public:
    std::string getName() const override {
        return "Engolfo de Baixa";
    }

    std::string getDescription() const override {
        return "Formado por dois candles consecutivos após uma tendência de alta. Indica possível reversão para baixa.";
    }

    std::string getStatus() const override {
        return "Vender";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& c1 = candles[candles.size() - 2]; // Primeiro candle (verde)
        const auto& c2 = candles[candles.size() - 1]; // Segundo candle (vermelho)

        double body1 = c1->bodySize();
        double body2 = c2->bodySize();

        // Condições:
        bool firstGreen = c1->isBullish() && body1 > 0.2;
        bool secondRed = c2->isBearish() && body2 > body1 * 1.5;

        bool engulfCondition =
            c2->getOpen() > c1->getClose() &&
            c2->getClose() < c1->getOpen();

        return firstGreen && secondRed && engulfCondition;
    }
};
