// include/Candlestick.hpp
#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <deque>
#include <memory>

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

    // Getters para acessar os valores do candle
    double getOpen() const { return open; }
    double getClose() const { return close; }
    double getLow() const { return low; }
    double getHigh() const { return close; }
    
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

    // Novo método estático para imprimir lista de candles
    static void printList(const std::deque<std::unique_ptr<Candlestick>>& candles) {
        std::cout << "=== Lista de Candles armazenados ===\n";
        for (size_t i = 0; i < candles.size(); ++i) {
            std::cout << "Candlestick #" << (i + 1) << candles[i]->describe() << "\n";
        }
    }

    // Retorna uma string com todas as informações do candle formatadas
    std::string describe() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(5);
        oss << "\nCandlestick Info:\n";
        oss << "  - Abertura:        " << open << "\n";
        oss << "  - Fechamento:      " << close << "\n";
        oss << "  - Maxima:          " << high << "\n";
        oss << "  - Minima:          " << low << "\n";
        oss << "  - Corpo:           " << bodySize() << "\n";
        oss << "  - Sombra Superior: " << upperShadow() << " (" << upperShadowPercent() << "%)\n";
        oss << "  - Sombra Inferior: " << lowerShadow() << " (" << lowerShadowPercent() << "%)\n";
        oss << "  - Amplitude Total: " << totalRange() << "\n";
        oss << "  - Tipo:            " << (isBullish() ? "Alta (Bullish)" : (isBearish() ? "Baixa (Bearish)" : "Neutro")) << "\n";
        return oss.str();
    }
};
