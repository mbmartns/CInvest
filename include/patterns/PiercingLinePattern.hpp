#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class PiercingLinePattern : public Pattern {
public:
    std::string getName() const override {
        return "Piercing Line";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de alta após tendência de baixa. Indica possível momento de compra.";
    }

    std::string getStatus() const override {
        return "Comprar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& c1 = candles[candles.size() - 2];  // Candle 1 (vermelho)
        const auto& c2 = candles[candles.size() - 1];  // Candle 2 (verde)

        double body1 = c1->bodySize();
        double body2 = c2->bodySize();

        // Primeiro candle: vermelho, corpo razoável
        bool c1_ok = c1->isBearish() && body1 > 0.4;

        // Segundo candle: verde, corpo razoável
        bool c2_ok = c2->isBullish() && body2 > 0.4;

        // Sombra superior e inferior pequenas no segundo candle
        bool smallShadows =
            c2->upperShadow() <= 0.1 * body2 &&
            c2->lowerShadow() <= 0.1 * body2;

        // Abertura do segundo abaixo do fechamento do primeiro,
        // fechamento do segundo acima da metade do corpo do primeiro
        bool piercingCriteria =
            c2->getOpen() < c1->getClose() &&
            c2->getClose() > (c1->getOpen() + c1->getClose()) / 2;

        return c1_ok && c2_ok && piercingCriteria && smallShadows;
    }
};
