Comandos para rodar o Projeto : 

Em um terminal
cd backend
g++ -std=c++17 -o cinvest.exe src/main.cpp src/TcpServer.cpp src/PatternDetector.cpp src/AppController.cpp src/utils/CandleParser.cpp -lws2_32
.\cinvest.exe
.\CandleTCP.exe

Apos isso execute baixe o necessario para rodar a API em python, com
pip install flask flask-cors
Apos isso :
python tcp_api.py

Tem que ter o node js instalado
npm run dev

Ordem de rodar
1 - C++
2 - Python
3 - Front