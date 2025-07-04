cmake_minimum_required(VERSION 3.10)
project(cinvest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Inclui os headers
include_directories(include)

# Adiciona os arquivos fonte
add_executable(CandleTCP
    src/main.cpp
    src/TcpServer.cpp
    src/PatternDetector.cpp
    src/AppController.cpp
    src/Candlestick.cpp 
)

# Biblioteca do Windows para sockets
target_link_libraries(CandleTCP ws2_32)

