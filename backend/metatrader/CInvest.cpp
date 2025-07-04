//+------------------------------------------------------------------+
//|                                                  SendCandleEA.mq5|
//|   Envia dados do candle via DLL e executa BUY ou SELL           |
//+------------------------------------------------------------------+
#property strict

#include <Trade/Trade.mqh>
#include <Trade/PositionInfo.mqh>
#include <Trade/OrderInfo.mqh>
#include <Trade/DealInfo.mqh>


input ENUM_TIMEFRAMES Timeframe = PERIOD_M1;
input double Volume = 0.1;
input int StopLossPoints = 3000;
input int TakeProfitPoints = 3000;
input int TrailingStopPoints = 600;
input int TrailingTriggerPoints = 100;
input ulong MagicNumber = 123456;

datetime lastReportedCandleTime = 0;
datetime sessionStartTime = 0;
double sessionProfit = 0;

CTrade trade;
CPositionInfo positionInfo;
COrderInfo orderInfo;
CDealInfo dealInfo;

ulong openTickets[];

#import "SocketSender.dll"
   bool InitConnection();
   void SendCandleData(double open, double high, double low, double close);
   string ReceiveCommand();
   void CloseConnection();
   void SendSymbolInfo(const uchar &symbol[], const uchar &timeframe[]);
   void SendOrderOpened(string type, double volume, double price, ulong ticket);
   void SendOrderClosed(string type, double price, double profit, ulong ticket);
   void SendSessionProfit(double profit);
#import

//+------------------------------------------------------------------+
//| Expert initialization function                                   |
//+------------------------------------------------------------------+

int OnInit()
{
   trade.SetExpertMagicNumber(MagicNumber);
   sessionProfit = 0;
   sessionStartTime = TimeCurrent();


   if (!InitConnection()) {
      Print("❌ Erro ao iniciar conexão socket.");
      return INIT_FAILED;
   }

   uchar symbolBuff[];
   uchar timeframeBuff[];

   StringToCharArray(_Symbol, symbolBuff);
   StringToCharArray(EnumToString(Timeframe), timeframeBuff);

   SendSymbolInfo(symbolBuff, timeframeBuff);

   Print("✅ Conexão socket iniciada com sucesso.");
   return INIT_SUCCEEDED;
}


//+------------------------------------------------------------------+
//| Expert tick function                                             |
//+------------------------------------------------------------------+
void OnTick()
{
   CheckTrailingStop();
   CheckClosedOrders();

   datetime closedCandleTime = iTime(_Symbol, Timeframe, 1);

   if (closedCandleTime != lastReportedCandleTime)
   {
      lastReportedCandleTime = closedCandleTime;

      double openPrice  = iOpen(_Symbol, Timeframe, 1);
      double highPrice  = iHigh(_Symbol, Timeframe, 1);
      double lowPrice   = iLow(_Symbol, Timeframe, 1);
      double closePrice = iClose(_Symbol, Timeframe, 1);

      PrintFormat("📨 Candle Fechado [%s] - O: %.5f | H: %.5f | L: %.5f | C: %.5f",
                  TimeToString(closedCandleTime, TIME_DATE|TIME_MINUTES),
                  openPrice, highPrice, lowPrice, closePrice);

      SendCandleData(openPrice, highPrice, lowPrice, closePrice);

      string cmd = "NONE";
      int attempts = 10;
      while(attempts-- > 0)
      {
         cmd = ReceiveCommand();
         if(cmd == "0" || cmd == "1")
            break;
         Sleep(100);
      }

      PrintFormat("📩 Comando recebido do DLL: %s", cmd);

      if(cmd == "0")
         ExecuteTrade("SELL");
      else if(cmd == "1")
         ExecuteTrade("BUY");
   }
}

//+------------------------------------------------------------------+
//| Executa uma operação de compra ou venda                          |
//+------------------------------------------------------------------+
void ExecuteTrade(string direction)
{
   double price = direction == "BUY" ? SymbolInfoDouble(_Symbol, SYMBOL_ASK) : SymbolInfoDouble(_Symbol, SYMBOL_BID);
   double sl = 0, tp = 0;

   if(StopLossPoints > 0)
      sl = direction == "BUY" ? price - StopLossPoints * _Point : price + StopLossPoints * _Point;

   if(TakeProfitPoints > 0)
      tp = direction == "BUY" ? price + TakeProfitPoints * _Point : price - TakeProfitPoints * _Point;

   if(!CheckStops(direction, price, sl, tp))
   {
      Print("❌ Níveis de stop inválidos. Ajustando...");
      double minDist = SymbolInfoInteger(_Symbol, SYMBOL_TRADE_STOPS_LEVEL) * _Point;

      if(direction == "BUY") {
         sl = sl > 0 ? MathMax(sl, price - minDist * 10) : 0;
         tp = tp > 0 ? MathMin(tp, price + minDist * 10) : 0;
      } else {
         sl = sl > 0 ? MathMin(sl, price + minDist * 10) : 0;
         tp = tp > 0 ? MathMax(tp, price - minDist * 10) : 0;
      }
   }

   bool success = false;
   ulong ticket;

   if(direction == "BUY") {
      success = trade.Buy(Volume, _Symbol, price, sl, tp);
      ticket = trade.ResultOrder();
   } else {
      success = trade.Sell(Volume, _Symbol, price, sl, tp);
      ticket = trade.ResultOrder();
   }

   if(success) {
      ArrayResize(openTickets, ArraySize(openTickets)+1);
      openTickets[ArraySize(openTickets)-1] = ticket;
      SendOrderOpened(direction, Volume, price, ticket);
      PrintFormat("✅ Ordem de %s enviada com sucesso! SL: %.5f | TP: %.5f", direction, sl, tp);
   } else {
      PrintFormat("❌ Erro ao enviar ordem %s: %d - %s", direction, GetLastError(), trade.ResultRetcodeDescription());
   }
}

//+------------------------------------------------------------------+
//| Verifica ordens fechadas e calcula o lucro                       |
//+------------------------------------------------------------------+
//+------------------------------------------------------------------+
//| Verifica ordens fechadas e calcula o lucro                       |
//+------------------------------------------------------------------+
void CheckClosedOrders()
{
   datetime start = sessionStartTime;
   datetime end = TimeCurrent();
   
   if(!HistorySelect(start, end))
   {
      Print("Erro ao selecionar histórico: ", GetLastError());
      return;
   }

   for(int i = ArraySize(openTickets)-1; i >= 0; i--)
   {
      ulong ticket = openTickets[i];

      if(!PositionSelectByTicket(ticket))
      {
         double totalProfit = 0.0;
         string direction = "";
         double closePrice = 0.0;
         bool orderClosed = false;

         int totalDeals = HistoryDealsTotal();
         for(int j = 0; j < totalDeals; j++)
         {
            ulong dealTicket = HistoryDealGetTicket(j);
            if(dealTicket > 0 && HistoryDealSelect(dealTicket)) // CORREÇÃO AQUI (linha 198)
            {
               if(dealInfo.Ticket() == dealTicket && dealInfo.Order() == ticket)
               {
                  totalProfit += dealInfo.Profit() + dealInfo.Swap() + dealInfo.Commission();

                  if(dealInfo.Entry() == DEAL_ENTRY_OUT)
                  {
                     closePrice = dealInfo.Price();
                     direction = (dealInfo.DealType() == DEAL_TYPE_BUY) ? "BUY" : "SELL";
                     orderClosed = true;
                  }
               }
            }
         }

         if(orderClosed && direction != "" && closePrice > 0)
         {
            sessionProfit += totalProfit;
            SendOrderClosed(direction, closePrice, totalProfit, ticket);
            PrintFormat("Ordem #%d fechada: %s @ %.5f, Lucro: %.2f", 
                       ticket, direction, closePrice, totalProfit);
         }

         if(ArrayRemove(openTickets, i, 1) != 1)
         {
            Print("Erro ao remover ticket #", ticket);
         }
      }
   }
}

//+------------------------------------------------------------------+
//| Verifica se os níveis de stop são válidos                        |
//+------------------------------------------------------------------+
bool CheckStops(string direction, double price, double sl, double tp)
{
   double minDist = SymbolInfoInteger(_Symbol, SYMBOL_TRADE_STOPS_LEVEL) * _Point;

   if(direction == "BUY") {
      if(sl > 0 && (price - sl) < minDist) return false;
      if(tp > 0 && (tp - price) < minDist) return false;
   } else {
      if(sl > 0 && (sl - price) < minDist) return false;
      if(tp > 0 && (price - tp) < minDist) return false;
   }
   return true;
}

//+------------------------------------------------------------------+
//| Verifica e ajusta trailing stop                                  |
//+------------------------------------------------------------------+
void CheckTrailingStop()
{
   if(TrailingStopPoints <= 0) return;

   for(int i = PositionsTotal()-1; i >= 0; i--)
   {
      if(positionInfo.SelectByIndex(i))
      {
         if(positionInfo.Symbol() == _Symbol && positionInfo.Magic() == MagicNumber)
         {
            ulong ticket = positionInfo.Ticket();
            double currentSl = positionInfo.StopLoss();
            double currentTp = positionInfo.TakeProfit();
            double newSl = currentSl;

            if(positionInfo.PositionType() == POSITION_TYPE_BUY)
            {
               double currentPrice = SymbolInfoDouble(_Symbol, SYMBOL_BID);
               double activationPrice = positionInfo.PriceOpen() + TrailingTriggerPoints * _Point;

               if(currentPrice > activationPrice)
               {
                  newSl = currentPrice - TrailingStopPoints * _Point;
                  if(newSl > currentSl && newSl > positionInfo.PriceOpen())
                  {
                     trade.PositionModify(ticket, newSl, currentTp);
                  }
               }
            }
            else if(positionInfo.PositionType() == POSITION_TYPE_SELL)
            {
               double currentPrice = SymbolInfoDouble(_Symbol, SYMBOL_ASK);
               double activationPrice = positionInfo.PriceOpen() - TrailingTriggerPoints * _Point;

               if(currentPrice < activationPrice)
               {
                  newSl = currentPrice + TrailingStopPoints * _Point;
                  if((newSl < currentSl || currentSl == 0) && newSl < positionInfo.PriceOpen())
                  {
                     trade.PositionModify(ticket, newSl, currentTp);
                  }
               }
            }
         }
      }
   }
}

//+------------------------------------------------------------------+
//| Expert deinitialization function                                 |
//+------------------------------------------------------------------+
void OnDeinit(const int reason)
{
   SendSessionProfit(sessionProfit);
   CloseConnection();
   Print("🔌 Conexão socket encerrada.");
}