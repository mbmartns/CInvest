# CInvest

## Descrição
O **CInvest** é uma aplicação de apoio a investimentos automatizados que se conecta ao MetaTrader 5 para receber dados de candles em tempo real e, com base em técnicas de análise técnica, identifica padrões gráficos clássicos, como martelo, engolfo e outros. Utilizando programação orientada a objetos em C++, o sistema realiza decisões automatizadas de compra e venda, funcionando como um robô de investimentos. Além disso, o CInvest também possui um papel educacional: ao detectar um padrão, ele fornece uma explicação clara sobre o significado daquele candle, que é exibida no frontend, auxiliando o usuário a aprender análise técnica enquanto opera.

![cinvest](https://github.com/user-attachments/assets/68b567d6-8586-42d6-b2d4-e3f5a99fc32b)
## Link Para vídeo explicativo
https://youtu.be/ktDbGtygCtM

## Tecnologias Escolhidas
| Tecnologia | Descrição | Justificativa para o Uso |
@@ -14,22 +14,17 @@ O projeto foi pensado como um exercício completo de desenvolvimento em equipe,
## Funcionalidades
Abaixo está uma lista não extensiva das principais funcionalidades suportadas na versão 1.0.0 do CInvest.

- Extração de dados OHLC (Open, High, Low, Close) dos candles
- Identificação de padrões técnicos (Hammer, Doji, Engulfing)
- Envio de ordens de compra e venda com base nos padrões detectados
- Exibição dos resultados das análises e decisões de trading
- Arquitetura modular com aplicação de Programação Orientada a Objetos

## Estrutura Organizacional

📁 Estrutura Organizacional do Projeto Este projeto é dividido em duas grandes camadas: Frontend e Backend, cada uma com responsabilidades bem definidas. Essa separação permite maior organização, escalabilidade e facilidade de manutenção.

🖥️ Frontend
A camada de frontend é responsável pela interface gráfica e pela interação do usuário com o sistema. Utiliza React.js, uma biblioteca JavaScript moderna e eficiente para criação de interfaces dinâmicas.

Diretório: frontend/

Estrutura do frontend: frontend/ ├── trading-dashboard/ # Projeto React.js contendo o dashboard de trading │ ├── .gitignore # Arquivos e pastas ignoradas pelo Git │ ├── package.json # Dependências e scripts de execução do React │ └── tcp-bridge.js # Módulo de comunicação TCP entre frontend e backend

tcp-bridge.js: Permite que o frontend envie e receba dados via TCP, facilitando a integração em tempo real com o backend.
package.json: Gerencia as bibliotecas e scripts do React.
trading-dashboard/: Pasta principal do projeto React.
⚙️ Backend (C++) A parte principal do projeto é escrita em C++ e implementa toda a lógica de negócio, análise de padrões e integração com a plataforma MetaTrader.

Diretórios principais do backend:

📁 include/ — Cabeçalhos Contém os arquivos de definições de classes e funções, organizados por responsabilidade.

Subpastas:

patterns/: Definições relacionadas à identificação de padrões gráficos.
utils/: Utilitários e classes auxiliares.
Arquivos principais:

AppController.hpp: Controlador principal da aplicação.
Candlestick.hpp: Estrutura de dados para velas do gráfico.
Pattern.hpp: Representa um padrão de análise técnica.
PatternDetector.hpp: Algoritmos de detecção de padrões.
TcpServer.hpp: Definição do servidor TCP.
📁 src/ — Implementações Contém os arquivos .cpp que implementam as funcionalidades declaradas nos headers.

Subpastas:

utils/: Implementações auxiliares.
Arquivos principais:
AppController.cpp: Implementação da lógica central da aplicação.
main.cpp: Ponto de entrada do programa.
PatternDetector.cpp: Implementação dos algoritmos de análise técnica.
TcpServer.cpp: Implementação do servidor TCP.
tcp_api.py: Script Python que possivelmente atua como conector ou ferramenta complementar para testes.
📁 metatrader/ Contém a lógica para comunicação e envio de dados entre o backend e a plataforma MetaTrader.

SendCandle.cpp: Responsável pelo envio das velas (candlesticks) para o MetaTrader.
📁 visual studio/ Arquivos de configuração ou compatibilidade com o ambiente do Visual Studio.

pch.h: Header de pré-compilação.
SocketSender.cpp: Implementação de envio de dados via sockets.
📦 Outros arquivos

vscode/: Configurações do Visual Studio Code (atalhos, lint, etc).
.gitignore: Arquivos e pastas ignoradas pelo sistema de controle de versão Git.
CMakeLists.txt: Script de build com CMake, usado para compilar o projeto multiplataforma.
BUILD.md: Documentação sobre o processo de build.
README.md: Este arquivo de documentação.
📌 Resumo das Tecnologias

Camada	Tecnologia	Função
Frontend	React.js	Interface do usuário
Backend	C++	Lógica de negócio e integração
Plataforma	MetaTrader	Análise e execução de trades

## Guia para build local do sistema
Para ajudar a fazer a build e setup local do sistema, o CInvest tem uma [BUILD.md]

## Autores do Projeto
- Itallo Augusto
- João Antônio
- Maria Beatriz
- Lucas Américo

## Licença

Este projeto está licenciado sob a Licença GNU General Public License v3.0 - veja o arquivo [LICENSE.md](LICENSE) para detalhes.
