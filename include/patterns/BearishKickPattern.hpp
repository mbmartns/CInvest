#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class BearishKickPattern : public Pattern {
public:
    std::string getName() const override {
        return "Chute de Baixa";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de baixa caracterizado por dois candles com corpos médios ou longos e um gap de baixa entre eles.";
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
        bool firstGreen = c1->isBullish() && body1 > 0.5;
        bool secondRed = c2->isBearish() && body2 > 0.5;
        bool hasGapDown = c2->getOpen() < c1->getClose();

        return firstGreen && secondRed && hasGapDown;
    }
};
