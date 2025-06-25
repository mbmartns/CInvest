// include/patterns/HangingManPattern.hpp
#pragma once

#include <deque>  // necessário para std::deque
#include <memory> // necessário para std::unique_ptr
#include "../Pattern.hpp"

class HangingManPattern : public Pattern {
public:
    std::string getName() const override {
        return "Hanging Man";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back(); // Último candle recebido

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
