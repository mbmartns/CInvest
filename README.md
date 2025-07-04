# CInvest

## Descrição
O **CInvest** é uma aplicação de apoio a investimentos automatizados que se conecta ao MetaTrader 5 para receber dados de candles em tempo real e, com base em técnicas de análise técnica, identifica padrões gráficos clássicos, como martelo, engolfo e outros. Utilizando programação orientada a objetos em C++, o sistema realiza decisões automatizadas de compra e venda, funcionando como um robô de investimentos. Além disso, o CInvest também possui um papel educacional: ao detectar um padrão, ele fornece uma explicação clara sobre o significado daquele candle, que é exibida no frontend, auxiliando o usuário a aprender análise técnica enquanto opera.

![cinvest](https://github.com/user-attachments/assets/68b567d6-8586-42d6-b2d4-e3f5a99fc32b)


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
(Adicionar aqui a estrutura de pastas do Código)




## Guia para build local do sistema
Para ajudar a fazer a build e setup local do sistema, o CInvest tem uma [BUILD.md]