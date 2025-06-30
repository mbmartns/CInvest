#pragma once

#include <deque>
#include <memory>
#include <string>
#include "../Pattern.hpp"

class HammerPattern : public Pattern {
public:
    std::string getName() const override {
        return "Hammer";
    }

    
    std::string getDescription() const override {
        return "O padrão Martelo sugere que os compradores estão retomando o controle após uma pressão vendedora, indicando possível reversão de baixa para alta.";
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
        bool longLowerShadow = lowerShadow > (2 * body);
        bool smallUpperShadow = upperShadow <= (body * 0.2);

        return smallBody && longLowerShadow && smallUpperShadow;
    }
};
