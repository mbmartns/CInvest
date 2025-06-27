#pragma once

#include "TcpServer.hpp"
#include "Candlestick.hpp"
#include "PatternDetector.hpp"
#include "patterns/HangingManPattern.hpp"
#include "patterns/BearishHaramiPattern.hpp"
#include "patterns/DojiPattern.hpp"
#include "patterns/LongDayPattern.hpp"
#include "patterns/ShortDayPattern.hpp"
#include "patterns/MarubozuPattern.hpp"
#include "patterns/PawnPattern.hpp"

#include <deque>
#include <memory>
#include <string>
#include <vector>

class AppController {
private:
    TcpServer server;
    std::deque<std::unique_ptr<Candlestick>> candles;
    PatternDetector detector;

public:
    AppController();
    void run();
};
