#include "../../include/utils/CandleParser.hpp"
#include <sstream>
#include <stdexcept>

Candlestick CandleParser::fromString(const std::string& message) {
    std::istringstream iss(message);
    double open, high, low, close;
    char comma;

    if (!(iss >> open >> comma >> high >> comma >> low >> comma >> close)) {
        throw std::runtime_error("Falha ao parsear a mensagem");
    }

    return Candlestick(open, high, low, close);
}
