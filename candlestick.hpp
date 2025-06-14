// Candlestick.hpp
#pragma once

#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>


// Classe base que representa um candle com todas as suas propriedades fundamentais
class Candlestick {
protected:
    double open;      // Preço de abertura
    double high;      // Preço máximo
    double low;       // Preço mínimo
    double close;     // Preço de fechamento

public:
    // Construtor
    Candlestick(double open, double high, double low, double close)
        : open(open), high(high), low(low), close(close) {}

    // Retorna o tipo do candle: true para alta, false para baixa
    bool isBullish() const {
        return close > open;
    }

    bool isBearish() const {
        return open > close;
    }

    // Amplitude total entre a máxima e mínima
    double totalRange() const {
        return high - low;
    }

    // Tamanho do corpo (abertura vs fechamento)
    double bodySize() const {
        return std::abs(close - open);
    }

    // Sombra superior: parte acima do corpo
    double upperShadow() const {
        double upper = isBullish() ? high - close : high - open;
        return std::max(0.0, upper);
    }

    // Sombra inferior: parte abaixo do corpo
    double lowerShadow() const {
        double lower = isBullish() ? open - low : close - low;
        return std::max(0.0, lower);
    }

    // Porcentagem da sombra superior em relação ao corpo
    double upperShadowPercent() const {
        double body = bodySize();
        return (body > 0) ? (upperShadow() / body) * 100.0 : 0.0;
    }

    // Porcentagem da sombra inferior em relação ao corpo
    double lowerShadowPercent() const {
        double body = bodySize();
        return (body > 0) ? (lowerShadow() / body) * 100.0 : 0.0;
    }

    // Retorna uma string com todas as informações do candle formatadas
    std::string describe() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(5);
        oss << "\n📊 Candlestick Info:\n";
        oss << "  - Abertura:        " << open << "\n";
        oss << "  - Fechamento:      " << close << "\n";
        oss << "  - Máxima:          " << high << "\n";
        oss << "  - Mínima:          " << low << "\n";
        oss << "  - Corpo:           " << bodySize() << "\n";
        oss << "  - Sombra Superior: " << upperShadow() << " (" << upperShadowPercent() << "%)\n";
        oss << "  - Sombra Inferior: " << lowerShadow() << " (" << lowerShadowPercent() << "%)\n";
        oss << "  - Amplitude Total: " << totalRange() << "\n";
        oss << "  - Tipo:            " << (isBullish() ? "Alta (Bullish)" : (isBearish() ? "Baixa (Bearish)" : "Neutro")) << "\n";
        return oss.str();
    }
};
