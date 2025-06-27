// include/patterns/ShortDayPattern.hpp
#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class ShortDayPattern : public Pattern {
public:
    std::string getName() const override {
        return "Short Day";
    }

    std::string getDescription() const override {
        return "Padrão com dois corpos pequenos consecutivos; indica indecisão e estabilidade no mercado, podendo sinalizar reversão ou continuidade.";
    }

    std::string getStatus() const override {
        return "Neutro";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& candle1 = candles[candles.size() - 2];
        const auto& candle2 = candles[candles.size() - 1];

        // Define corpo pequeno como corpo menor que 30% do total do range do candle
        auto isSmallBody = [](const std::unique_ptr<Candlestick>& c) {
            return c->bodySize() <= 0.3 * c->totalRange();
        };

        bool candle1Small = isSmallBody(candle1);
        bool candle2Small = isSmallBody(candle2);

        // Verifica cores vermelho-verde ou verde-vermelho
        bool firstRedSecondGreen = (candle1->getClose() < candle1->getOpen()) && (candle2->getClose() > candle2->getOpen());
        bool firstGreenSecondRed = (candle1->getClose() > candle1->getOpen()) && (candle2->getClose() < candle2->getOpen());

        return candle1Small && candle2Small && (firstRedSecondGreen || firstGreenSecondRed);
    }
};
