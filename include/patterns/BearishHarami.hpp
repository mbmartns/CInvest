#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class HaramiDeTopoPattern : public Pattern {
public:
    std::string getName() const override {
        return "Harami de Topo";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de baixa: três candles consecutivos - médio de alta, pequeno de baixa, grande de baixa.";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 3) return false;

        const auto& c1 = candles[candles.size() - 3];  // Candle 1 (mais antigo)
        const auto& c2 = candles[candles.size() - 2];  // Candle 2 (intermediário)
        const auto& c3 = candles[candles.size() - 1];  // Candle 3 (mais recente)

        double body1 = c1->bodySize();
        double body2 = c2->bodySize();
        double body3 = c3->bodySize();

        // Candle 1: corpo médio, alta (verde)
        bool c1_ok = body1 > 0.5 && c1->isBullish();

        // Candle 2: corpo pequeno, baixa (vermelho)
        bool c2_ok = body2 < 0.5 && c2->isBearish();

        // Candle 3: corpo grande, baixa (vermelho)
        bool c3_ok = body3 > 0.7 && c3->isBearish();

        return c1_ok && c2_ok && c3_ok;
    }
};
