Ordem de rodar
1 - Frontend
2 - Python
3 - C++

Comandos para rodar o Projeto : 

.\start_project.bat
Que faz com que :

Tem que ter o node js instalado
cd frontend
cd trading-dashboard
npm run dev

Apos isso execute baixe o necessario para rodar a API em python, com
pip install flask flask-cors
Apos isso :
cd backend
cd src
python tcp_api.py

Em um terminal
cd backend
g++ -std=c++17 -o cinvest.exe src/main.cpp src/TcpServer.cpp src/PatternDetector.cpp src/AppController.cpp src/utils/CandleParser.cpp -lws2_32
.\cinvest.exe
.\CandleTCP.exe
