// include/patterns/LongDayPattern.hpp
#pragma once

#include <deque>
#include <memory>
#include "../Pattern.hpp"

class LongDayPattern : public Pattern {
public:
    std::string getName() const override {
        return "Long Day";
    }

    std::string getDescription() const override {
        return "Padrão com dois corpos grandes consecutivos; pode indicar alta volatilidade, porém a tendência futura é incerta.";
    }

    std::string getStatus() const override {
        return "Neutro";
    }

    bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const override {
        if (candles.size() < 2) return false;

        const auto& candle1 = candles[candles.size() - 2];
        const auto& candle2 = candles[candles.size() - 1];

        // Critério para corpo "grande" pode ser, por exemplo, maior que 50% do range do candle (high - low)
        auto isLargeBody = [](const std::unique_ptr<Candlestick>& c) {
            return c->bodySize() >= 0.5 * c->totalRange();
        };

        bool candle1Large = isLargeBody(candle1);
        bool candle2Large = isLargeBody(candle2);

        // Cores: vermelho (fechamento < abertura) e verde (fechamento > abertura)
        bool firstRedSecondGreen = (candle1->getClose() < candle1->getOpen()) && (candle2->getClose() > candle2->getOpen());
        bool firstGreenSecondRed = (candle1->getClose() > candle1->getOpen()) && (candle2->getClose() < candle2->getOpen());

        return candle1Large && candle2Large && (firstRedSecondGreen || firstGreenSecondRed);
    }
};
