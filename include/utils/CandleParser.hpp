#pragma once

#include <string>
#include "../Candlestick.hpp"

class CandleParser {
public:
    static Candlestick fromString(const std::string& message);
};
