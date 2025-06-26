// include/patterns/HangingManPattern.hpp
#pragma once

#include <deque> 
#include <memory> 
#include "../Pattern.hpp"

class HangingManPattern : public Pattern {
public:
    std::string getName() const override {
        return "Hanging Man";
    }

    std::string getDescription() const override {
        return "É um candlestick com padrão de reversão de baixa, ou seja, um bom momento para vender. ";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        // Pegar exatamente o ultimo candle recebido
        const auto& candle = candles.back();

        double body = candle->bodySize();
        double lowerShadow = candle->lowerShadow();
        double upperShadow = candle->upperShadow();

        // Critérios do padrão Enforcado
        bool corpoPequeno = body > 0 && body <= (candle->totalRange() * 0.3);
        bool sombraInferiorLonga = lowerShadow > (2 * body);
        bool sombraSuperiorPequena = upperShadow <= body * 0.25;

        return corpoPequeno && sombraInferiorLonga && sombraSuperiorPequena;
    }
};
