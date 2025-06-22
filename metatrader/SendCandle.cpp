//+------------------------------------------------------------------+
//|                                                  SendCandleEA.mq5|
//|       Envia dados do candle fechado via DLL (com socket)        |
//+------------------------------------------------------------------+
#property version   "1.04"
#property strict

input ENUM_TIMEFRAMES Timeframe = PERIOD_CURRENT;

datetime lastReportedCandleTime = 0;

#import "SocketSender.dll"
   bool InitConnection();
   void SendCandleData(double open, double high, double low, double close);
   void CloseConnection();
#import

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

        PrintFormat("Fechado [%s] - O: %.5f | H: %.5f | L: %.5f | C: %.5f",
                     TimeToString(closedCandleTime, TIME_DATE | TIME_MINUTES),
                     openPrice, highPrice, lowPrice, closePrice);

        SendCandleData(openPrice, highPrice, lowPrice, closePrice);
    }
}

void OnDeinit(const int reason)
{
   CloseConnection();
   Print("🔌 Conexão socket encerrada.");
}