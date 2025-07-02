//+------------------------------------------------------------------+
//|                                                  SendCandleEA.mq5|
//|   Envia dados do candle via DLL e executa BUY ou SELL           |
//+------------------------------------------------------------------+
#property strict

#include <Trade/Trade.mqh>

input ENUM_TIMEFRAMES Timeframe = PERIOD_CURRENT;
input double Volume = 0.1;

datetime lastReportedCandleTime = 0;

CTrade trade;  // ✅ Classe para executar ordens

#import "SocketSender.dll"
   bool InitConnection();
   void SendCandleData(double open, double high, double low, double close);
   string ReceiveCommand();
   void CloseConnection();
#import

//+------------------------------------------------------------------+
int OnInit()
{
   if (!InitConnection())
   {
      Print("❌ Erro ao iniciar conexão socket.");
      return INIT_FAILED;
   }

   Print("✅ Conexão socket iniciada com sucesso.");
   return INIT_SUCCEEDED;
}

//+------------------------------------------------------------------+
void OnTick()
{
   datetime closedCandleTime = iTime(_Symbol, Timeframe, 1);

   if (closedCandleTime != lastReportedCandleTime)
   {
      lastReportedCandleTime = closedCandleTime;

      double openPrice  = iOpen(_Symbol, Timeframe, 1);
      double highPrice  = iHigh(_Symbol, Timeframe, 1);
      double lowPrice   = iLow(_Symbol, Timeframe, 1);
      double closePrice = iClose(_Symbol, Timeframe, 1);

      PrintFormat("📨 Candle Fechado [%s] - O: %.5f | H: %.5f | L: %.5f | C: %.5f",
                  TimeToString(closedCandleTime, TIME_DATE | TIME_MINUTES),
                  openPrice, highPrice, lowPrice, closePrice);

      SendCandleData(openPrice, highPrice, lowPrice, closePrice);

      // Tenta receber comando por até 2 segundos
      string cmd = "NONE";
      int attempts = 10;
      while (attempts-- > 0)
      {
         cmd = ReceiveCommand();
         if (cmd == "0" || cmd == "1")
            break;
         Sleep(200);
      }

      PrintFormat("📩 Comando recebido do DLL: %s", cmd);

      if (cmd == "0")
         ExecuteTrade("SELL");
      else if (cmd == "1")
         ExecuteTrade("BUY");
   }
}

//+------------------------------------------------------------------+
void ExecuteTrade(string direction)
{
   bool success = false;

   if (direction == "BUY")
      success = trade.Buy(Volume, _Symbol);
   else if (direction == "SELL")
      success = trade.Sell(Volume, _Symbol);

   if (!success)
   {
      PrintFormat("❌ Erro ao enviar ordem %s: %d - %s",
                  direction, GetLastError(), trade.ResultRetcodeDescription());
   }
   else
   {
      PrintFormat("✅ Ordem de %s enviada com sucesso!", direction);
   }
}

//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
   CloseConnection();
   Print("🔌 Conexão socket encerrada.");
}