#pragma once

#include <deque>
#include <memory>
#include <string>
#include "../Pattern.hpp"

class InvertedHammerPattern : public Pattern {
public:
    std::string getName() const override {
        return "Inverted Hammer";
    }
    
    std::string getDescription() const override {
        return "O padrão Martelo Invertido sugere uma possível reversão de baixa para alta. "
               "Ele indica que os compradores estão começando a ganhar força após uma sequência de quedas.";
    }

    std::string getStatus() const override {
        return "Comprar";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back();

        double body = candle->bodySize();
        double lowerShadow = candle->lowerShadow();
        double upperShadow = candle->upperShadow();
        double total = candle->totalRange();

        bool smallBody = body > 0 && body <= (total * 0.2);
        bool longUpperShadow = upperShadow > (2 * body);
        bool smallLowerShadow = lowerShadow <= (body * 0.2);

        return smallBody && longUpperShadow && smallLowerShadow;
    }
};
