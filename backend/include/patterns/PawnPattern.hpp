// include/patterns/PeaoPattern.hpp
#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class PawnPattern : public Pattern {
public:
    std::string getName() const override {
        return "Peão";
    }

    std::string getDescription() const override {
        return "Padrão com pouca variação entre abertura e fechamento em dois candles consecutivos, indicando indecisão no mercado.";
    }

    std::string getStatus() const override {
        return "Neutro";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        // Precisamos de pelo menos 2 candles para detectar esse padrão
        if (candles.size() < 2) return false;

        // Pega os dois últimos candles
        const auto& candle1 = candles[candles.size() - 2];
        const auto& candle2 = candles.back();

        // Define corpo pequeno: corpo <= 30% do total do candle
        auto isSmallBody = [](const std::unique_ptr<Candlestick>& candle) {
            return candle->bodySize() > 0 && candle->bodySize() <= (candle->totalRange() * 0.3);
        };

        return isSmallBody(candle1) && isSmallBody(candle2);
    }
};
