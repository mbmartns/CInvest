// include/patterns/DojiPattern.hpp
#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class DojiPattern : public Pattern {
public:
    std::string getName() const override {
        return "Doji";
    }

    std::string getDescription() const override {
        return "O Doji representa um ponto de equilíbrio, onde o preço de abertura e fechamento são praticamente iguais. Pode indicar uma possível reversão de tendência.";
    }

    std::string getStatus() const override {
        return "Neutro";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back();
        double body = candle->bodySize();
        double range = candle->totalRange();

        // Considera como Doji se o corpo for muito pequeno comparado ao range total
        return range > 0 && (body / range) <= 0.1;
    }
};
