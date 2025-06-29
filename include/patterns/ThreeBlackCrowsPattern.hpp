#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class ThreeBlackCrowsPattern : public Pattern {
public:
    std::string getName() const override {
        return "Três Corvos Pretos";
    }

    std::string getDescription() const override {
        return "Padrão de reversão de baixa composto por três candles vermelhos consecutivos, cada um fechando abaixo do corpo anterior "
               "e abrindo próximo à abertura do candle anterior. Indica perda de força dos compradores e possível início de tendência de baixa.";
    }

    std::string getStatus() const override {
        return "Vender";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 3) return false;

        const auto& c1 = candles[candles.size() - 3];
        const auto& c2 = candles[candles.size() - 2];
        const auto& c3 = candles[candles.size() - 1];

        // Todos devem ser candles de baixa significativos
        bool allBearish = c1->isBearish() && c2->isBearish() && c3->isBearish();

        // O corpo do candle seguinte deve começar próximo da abertura do anterior e fechar abaixo
        bool c2_aligns = std::abs(c2->getOpen() - c1->getOpen()) <= (c1->bodySize() * 0.3);
        bool c3_aligns = std::abs(c3->getOpen() - c2->getOpen()) <= (c2->bodySize() * 0.3);

        bool c2_closes_lower = c2->getClose() < c1->getClose();
        bool c3_closes_lower = c3->getClose() < c2->getClose();

        return allBearish && c2_aligns && c3_aligns && c2_closes_lower && c3_closes_lower;
    }
};
