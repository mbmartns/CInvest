#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Pattern.hpp"

class PatternDetector {
private:
    std::vector<std::unique_ptr<Pattern>> patterns;

public:
    // Construtor
    PatternDetector();

    // Adiciona um novo padrão
    void addPattern(std::unique_ptr<Pattern> pattern);

    // Aplica todos os padrões à lista de candles e retorna os nomes detectados
    std::vector<std::string> detect(const std::vector<std::unique_ptr<Candlestick>>& candles) const;
};
