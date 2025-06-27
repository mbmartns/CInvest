#include "../include/PatternDetector.hpp"

PatternDetector::PatternDetector() = default;

void PatternDetector::addPattern(std::unique_ptr<Pattern> pattern) {
    patterns.push_back(std::move(pattern));
}

// signature no PatternDetector.hpp
std::vector<const Pattern*> PatternDetector::detect(const std::deque<std::unique_ptr<Candlestick>>& candles) const {
    std::vector<const Pattern*> detectedPatterns;

    for (const auto& pattern : patterns) {
        if (pattern->detect(candles)) {
            detectedPatterns.push_back(pattern.get());
        }
    }

    return detectedPatterns;
}
