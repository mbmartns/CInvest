#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class DarkCloudCoverPattern : public Pattern {
public:
    std::string getName() const override {
        return "Nuvem Negra";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de alta, composto por dois candles: um de alta (verde) seguido por um de baixa (vermelho) "
               "com abertura acima e fechamento abaixo da metade do corpo anterior. Sinais ainda fracos — recomenda-se cautela.";
    }

    std::string getStatus() const override {
        return "Observar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& c1 = candles[candles.size() - 2];  // Primeiro candle (alta)
        const auto& c2 = candles[candles.size() - 1];  // Segundo candle (baixa)

        double c1_open = c1->getOpen();
        double c1_close = c1->getClose();
        double c2_open = c2->getOpen();
        double c2_close = c2->getClose();

        double c1_body_midpoint = c1_open + (c1_close - c1_open) / 2;

        bool c1_bullish = c1->isBullish();
        bool c2_bearish = c2->isBearish();

        bool gap_up = c2_open > c1_close;
        bool closes_below_half = c2_close < c1_body_midpoint;

        bool upper_shadow_small = c2->upperShadow() <= c2->bodySize() * 0.1;
        bool lower_shadow_small = c2->lowerShadow() <= c2->bodySize() * 0.1;

        return c1_bullish && c2_bearish &&
               gap_up && closes_below_half &&
               upper_shadow_small && lower_shadow_small;
    }
};
