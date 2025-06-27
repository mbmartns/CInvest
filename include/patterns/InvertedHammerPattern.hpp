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

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back();

        double body = candle->bodySize();
        double lowerShadow = candle->lowerShadow();
        double upperShadow = candle->upperShadow();

        // Critérios do Martelo Invertido
        bool smallBody = body > 0 && body <= (candle->totalRange() * 0.2);
        bool longUpperShadow = upperShadow > (2 * body);
        bool smallLowerShadow = lowerShadow <= body * 0.20;

        return smallBody && longUpperShadow && smallLowerShadow;    }

    std::string getDescription() const {
        return "O Martelo Invertido tem corpo pequeno, sombra superior longa (cerca de 2x o corpo) e sombra inferior pequena ou inexistente.";
    }

    std::string getJustification() const {
        return "O padrão Martelo Invertido sugere possível reversão de baixa para alta. Indica que compradores começam a pressionar após quedas, "
               "podendo ser um sinal de compra no início da transição para alta.";
    }
};