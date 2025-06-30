// include/Pattern.hpp
#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Candlestick.hpp"

class Pattern {
public:
    virtual ~Pattern() = default;

    // Nome do padrão (ex: "Hammer", "Engulfing")
    virtual std::string getName() const = 0;

    // Nome da descrição (todo padrão deve ter uma descrição)
    virtual std::string getDescription() const = 0;

    // Nome da descrição (todo padrão deve ter uma descrição)
    virtual std::string getStatus() const = 0;

    // Detecta se o padrão está presente nos últimos candles
    virtual bool detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const = 0;
};
