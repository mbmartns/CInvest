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

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back(); // Último candle recebido

        double body = candle->bodySize();
        double lowerShadow = candle->lowerShadow();
        double upperShadow = candle->upperShadow();

        bool smallBody = body > 0 && body <= (candle->totalRange() * 0.2);
        bool longLowerShadow = lowerShadow > (2 * body);
        bool smallUpperShadow = upperShadow <= body * 0.20;

        return smallBody && longLowerShadow && smallUpperShadow;
    }

    std::string getDescription() const {
        return "O candle Martelo é caracterizado por um corpo pequeno (de qualquer cor), sombra superior pequena ou inexistente "
               "e uma longa sombra inferior (cerca de duas vezes o tamanho do corpo).";
    }

    std::string getJustification() const {
        return "O padrão Martelo pode ser um sinal de compra, ele sugere que os compradores estão retomando o controle após um período de vendas, "
               "indicando uma possível reversão de baixa para alta.";
    }
};