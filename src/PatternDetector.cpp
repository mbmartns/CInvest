#include "../include/PatternDetector.hpp"

PatternDetector::PatternDetector() = default;

void PatternDetector::addPattern(std::unique_ptr<Pattern> pattern) {
    patterns.push_back(std::move(pattern));
}

std::vector<std::string> PatternDetector::detect(const std::vector<std::unique_ptr<Candlestick>>& candles) const {
    std::vector<std::string> detected;

    for (const auto& pattern : patterns) {
        if (pattern->detect(candles)) {
            detected.push_back(pattern->getName());
        }
    }

    return detected;
}