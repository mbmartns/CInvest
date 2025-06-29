#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class ShootingStarPattern : public Pattern {
public:
    std::string getName() const override {
        return "Estrela Cadente";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de baixa, com corpo pequeno (vermelho), sombra superior longa e sombra inferior pequena ou inexistente.";
    }

    std::string getStatus() const override {
        return "Vender";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back();

        double body = candle->bodySize();
        double upperShadow = candle->upperShadow();
        double lowerShadow = candle->lowerShadow();
        double total = candle->totalRange();

        bool smallBody = body > 0 && body <= (total * 0.2);
        bool longUpperShadow = upperShadow > (2 * body);
        bool smallLowerShadow = lowerShadow <= (0.1 * total);
        bool isRed = candle->isBearish();

        return smallBody && longUpperShadow && smallLowerShadow && isRed;
    }
};
