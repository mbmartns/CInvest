@echo off
REM Inicia o Frontend
start cmd /k "cd /d %~dp0frontend\trading-dashboard && npm run dev"

REM Inicia o backend Python (API Flask)
start cmd /k "cd /d %~dp0backend\src && python tcp_api.py"

REM Compila e executa o backend C++
start cmd /k "cd /d %~dp0backend && g++ -std=c++17 -o cinvest.exe src\main.cpp src\TcpServer.cpp src\PatternDetector.cpp src\AppController.cpp src\utils\CandleParser.cpp -lws2_32 && .\cinvest.exe"