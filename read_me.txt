write: g++ -o TcpServer.exe TcpServer.cpp -lws2_32
then: .\TcpServer.exe

g++ -o cinvest.exe src/main.cpp src/TcpServer.cpp src/PatternDetector.cpp -lws2_32
then: .\cinvest.exe
