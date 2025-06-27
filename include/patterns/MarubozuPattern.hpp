// include/patterns/MarubozuPattern.hpp
#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class MarubozuPattern : public Pattern {
public:
    std::string getName() const override {
        return "Marubozu";
    }

    std::string getDescription() const override {
        return "Candlestick com corpo médio ou grande, sem sombra superior nem inferior, indicando abertura na máxima e fechamento na mínima (ou vice-versa).";
    }

    std::string getStatus() const override {
        return "Neutro";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.empty()) return false;

        const auto& candle = candles.back();

        double body = candle->bodySize();
        double totalRange = candle->totalRange();
        double upperShadow = candle->upperShadow();
        double lowerShadow = candle->lowerShadow();

        // Corpo médio ou grande: corpo >= 50% do range total
        bool corpoMedioOuGrande = body >= (0.5 * totalRange);

        // Sem sombras superior e inferior (considera pequenas tolerâncias para evitar erros numéricos)
        bool semSombraSuperior = upperShadow <= 0.01 * totalRange;
        bool semSombraInferior = lowerShadow <= 0.01 * totalRange;

        return corpoMedioOuGrande && semSombraSuperior && semSombraInferior;
    }
};
