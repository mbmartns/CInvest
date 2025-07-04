#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class BullishKickPattern : public Pattern {
public:
    std::string getName() const override {
        return "Chute de Alta";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de alta caracterizado por dois candles com corpos médios ou longos e um gap de alta entre eles.";
    }

    std::string getStatus() const override {
        return "Comprar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& c1 = candles[candles.size() - 2]; // Primeiro candle (vermelho)
        const auto& c2 = candles[candles.size() - 1]; // Segundo candle (verde)

        double body1 = c1->bodySize();
        double body2 = c2->bodySize();

        // Condições:
        bool firstRed = c1->isBearish() && body1 > 0.5;
        bool secondGreen = c2->isBullish() && body2 > 0.5;
        bool hasGapUp = c2->getOpen() > c1->getClose();

        return firstRed && secondGreen && hasGapUp;
    }
};
