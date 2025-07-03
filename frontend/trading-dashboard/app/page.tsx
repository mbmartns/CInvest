"use client"

import { useState, useEffect } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "../components/ui/card"
import { Badge } from "../components/ui/badge"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "../components/ui/tabs"
import { ScrollArea } from "../components/ui/scroll-area"
import { TrendingUp, Activity, DollarSign, Target, Clock } from "lucide-react"
import './globals.css'



// Tipos de dados
interface Candlestick {
  time: string
  open: number
  high: number
  low: number
  close: number
  volume: number
}

interface Pattern {
  id: string
  name: string
  type: "bullish" | "bearish" | "neutral"
  confidence: number
  time: string
  description: string
}

interface Order {
  id: string
  type: "buy" | "sell"
  price: number
  time: string
  justification: string
  status: "pending" | "profit" | "loss"
  pnl?: number
}

interface DailyStats {
  totalTrades: number
  winRate: number
  totalPnL: number
  bestTrade: number
  worstTrade: number
}

// Padrões de candlestick
const PATTERNS = [
  {
    name: "Martelo",
    type: "bullish" as const,
    description: "Padrão de reversão de alta com corpo pequeno e sombra inferior longa",
  },
  {
    name: "Enforcado",
    type: "bearish" as const,
    description: "Padrão de reversão de baixa com corpo pequeno e sombra inferior longa",
  },
  { name: "Doji", type: "neutral" as const, description: "Padrão de indecisão com abertura e fechamento próximos" },
  {
    name: "Engolfo de Alta",
    type: "bullish" as const,
    description: "Candle de alta que engole completamente o candle anterior de baixa",
  },
  {
    name: "Engolfo de Baixa",
    type: "bearish" as const,
    description: "Candle de baixa que engole completamente o candle anterior de alta",
  },
  {
    name: "Estrela Cadente",
    type: "bearish" as const,
    description: "Padrão de reversão de baixa com sombra superior longa",
  },
  { name: "Estrela da Manhã", type: "bullish" as const, description: "Padrão de reversão de alta de três candles" },
]

export default function TradingDashboard() {
  const [currentPrice, setCurrentPrice] = useState(1.085)
  const [candlesticks, setCandlesticks] = useState<Candlestick[]>([])
  const [patterns, setPatterns] = useState<Pattern[]>([])
  const [orders, setOrders] = useState<Order[]>([])
  const [dailyStats, setDailyStats] = useState<DailyStats>({
    totalTrades: 0,
    winRate: 0,
    totalPnL: 0,
    bestTrade: 0,
    worstTrade: 0,
  })
  const [isConnected, setIsConnected] = useState(true)


  // Detectar padrão aleatoriamente
  const detectPattern = () => {
    if (Math.random() < 0.15) {
      // 15% chance de detectar padrão
      const pattern = PATTERNS[Math.floor(Math.random() * PATTERNS.length)]
      const newPattern: Pattern = {
        id: Date.now().toString(),
        name: pattern.name,
        type: pattern.type,
        confidence: Math.floor(Math.random() * 30) + 70, // 70-100%
        time: new Date().toLocaleTimeString(),
        description: pattern.description,
      }

      setPatterns((prev) => [newPattern, ...prev.slice(0, 19)]) // Manter apenas 20 padrões

      // Chance de executar ordem baseada no padrão
      if (Math.random() < 0.6) {
        // 60% chance de executar ordem
        executeOrder(newPattern)
      }
    }
  }

  // Executar ordem
  const executeOrder = (pattern: Pattern) => {
    const orderType =
      pattern.type === "bullish" ? "buy" : pattern.type === "bearish" ? "sell" : Math.random() > 0.5 ? "buy" : "sell"
    const price = currentPrice + (Math.random() - 0.5) * 0.0005

    const justifications = {
      Martelo:
        "Padrão Martelo detectado com forte sombra inferior, indicando pressão compradora. Reversão de alta provável.",
      Enforcado: "Padrão Enforcado identificado após movimento de alta, sinalizando possível reversão de baixa.",
      Doji: "Padrão Doji detectado, indicando indecisão do mercado. Aguardando confirmação direcional.",
      "Engolfo de Alta": "Padrão Engolfo de Alta confirmado, candle atual engoliu completamente o anterior.",
      "Engolfo de Baixa": "Padrão Engolfo de Baixa detectado, forte pressão vendedora identificada.",
      "Estrela Cadente": "Estrela Cadente formada no topo, sinal de reversão de baixa com alta probabilidade.",
      "Estrela da Manhã": "Padrão Estrela da Manhã completado, forte sinal de reversão de alta.",
    }

    const newOrder: Order = {
      id: Date.now().toString(),
      type: orderType,
      price: Number(price.toFixed(5)),
      time: new Date().toLocaleTimeString(),
      justification: `Ordem de ${orderType === "buy" ? "compra" : "venda"} executada devido ao padrão ${pattern.name} detectado com ${pattern.confidence}% de confiança. ${justifications[pattern.name as keyof typeof justifications]}`,
      status: "pending",
    }

    setOrders((prev) => [newOrder, ...prev])

    // Simular resultado da ordem após alguns segundos
    setTimeout(
      () => {
        const isProfit = Math.random() > 0.4 // 60% chance de lucro
        const pnl = isProfit
          ? Math.random() * 50 + 10 // Lucro entre 10-60
          : -(Math.random() * 30) - 5 // Prejuízo entre -5 a -35

        setOrders((prev) =>
          prev.map((order) =>
            order.id === newOrder.id
              ? { ...order, status: isProfit ? "profit" : "loss", pnl: Number(pnl.toFixed(2)) }
              : order,
          ),
        )

        // Atualizar estatísticas
        setDailyStats((prev) => {
          const newTotalTrades = prev.totalTrades + 1
          const newTotalPnL = prev.totalPnL + pnl
          const wins = orders.filter((o) => o.status === "profit").length + (isProfit ? 1 : 0)
          const newWinRate = (wins / newTotalTrades) * 100

          return {
            totalTrades: newTotalTrades,
            winRate: Number(newWinRate.toFixed(1)),
            totalPnL: Number(newTotalPnL.toFixed(2)),
            bestTrade: Math.max(prev.bestTrade, pnl),
            worstTrade: Math.min(prev.worstTrade, pnl),
          }
        })
      },
      Math.random() * 10000 + 5000,
    ) // 5-15 segundos
  }
  function generateCandlestick(prevClose: number): Candlestick {
    const open = prevClose + (Math.random() - 0.5) * 0.001
    const volatility = 0.0015
    const high = open + Math.random() * volatility
    const low = open - Math.random() * volatility
    const close = low + Math.random() * (high - low)

    return {
      time: new Date().toLocaleTimeString(),
      open: Number(open.toFixed(5)),
      high: Number(high.toFixed(5)),
      low: Number(low.toFixed(5)),
      close: Number(close.toFixed(5)),
      volume: Math.floor(Math.random() * 1000) + 100,
    }
  }

  // Atualizar dados em tempo real
  useEffect(() => {
    const interval = setInterval(() => {
      // Atualizar preço atual
      setCurrentPrice((prev) => {
        const change = (Math.random() - 0.5) * 0.002
        return Number((prev + change).toFixed(5))
      })

      // Adicionar novo candlestick
      setCandlesticks((prev) => {
        const lastClose = prev.length > 0 ? prev[prev.length - 1].close : 1.085
        const newCandle = generateCandlestick(lastClose)
        return [...prev.slice(-29), newCandle] // Manter apenas 30 candles
      })

      // Detectar padrões
      detectPattern()

      // Simular desconexão ocasional
      if (Math.random() < 0.02) {
        // 2% chance
        setIsConnected(false)
        setTimeout(() => setIsConnected(true), 2000)
      }
    }, 2000) // Atualizar a cada 2 segundos

    return () => clearInterval(interval)
  }, [currentPrice, orders])

  const formatCurrency = (value: number) => {
    return new Intl.NumberFormat("pt-BR", {
      style: "currency",
      currency: "USD",
      minimumFractionDigits: 2,
    }).format(value)
  }

  return (
    <div className="min-h-screen bg-gray-50 p-4">
      <div className="max-w-7xl mx-auto space-y-6">
        {/* Header */}
        <div className="flex items-center justify-between">
          <div>
            <h1 className="text-3xl font-bold text-gray-900">Trading Bot Dashboard</h1>
            <p className="text-gray-600">Monitoramento em tempo real do robô automatizado</p>
          </div>
          <div className="flex items-center space-x-4">
            <Badge variant={isConnected ? "default" : "destructive"} className="px-3 py-1">
              <Activity className="w-4 h-4 mr-1" />
              {isConnected ? "Conectado" : "Desconectado"}
            </Badge>
          </div>
        </div>

        {/* Informações do Mercado */}
        <div className="grid grid-cols-1 md:grid-cols-4 gap-4">
          <Card>
            <CardHeader className="pb-2">
              <CardTitle className="text-sm font-medium text-gray-600">Par de Moedas</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="text-2xl font-bold">EUR/USD</div>
              <p className="text-sm text-gray-500">Euro / Dólar Americano</p>
            </CardContent>
          </Card>

          <Card>
            <CardHeader className="pb-2">
              <CardTitle className="text-sm font-medium text-gray-600">Timeframe</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="text-2xl font-bold">M1</div>
              <p className="text-sm text-gray-500">1 Minuto</p>
            </CardContent>
          </Card>

          <Card>
            <CardHeader className="pb-2">
              <CardTitle className="text-sm font-medium text-gray-600">Preço Atual</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="text-2xl font-bold">{currentPrice.toFixed(5)}</div>
              <p className="text-sm text-green-600 flex items-center">
                <TrendingUp className="w-4 h-4 mr-1" />
                Em tempo real
              </p>
            </CardContent>
          </Card>

          <Card>
            <CardHeader className="pb-2">
              <CardTitle className="text-sm font-medium text-gray-600">Status</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="text-2xl font-bold text-green-600">Ativo</div>
              <p className="text-sm text-gray-500">Robô operando</p>
            </CardContent>
          </Card>
        </div> 
        {/* Gráfico de Candlesticks Simulado */}
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center">
              <TrendingUp className="w-5 h-5 mr-2" />
              Gráfico de Candlesticks - EUR/USD M1
            </CardTitle>
          </CardHeader>
          <CardContent>
            <div className="h-64 bg-gray-900 rounded-lg p-4 overflow-x-auto">
              <div className="flex items-end space-x-1 h-full min-w-max">
                {candlesticks.map((candle, index) => {
                  const isGreen = candle.close > candle.open
                  const bodyHeight = Math.abs(candle.close - candle.open) * 10000
                  const wickTop = (candle.high - Math.max(candle.open, candle.close)) * 10000
                  const wickBottom = (Math.min(candle.open, candle.close) - candle.low) * 10000

                  return (
                    <div key={index} className="flex flex-col items-center justify-end h-full">
                      {/* Wick superior */}
                      <div
                        className={`w-0.5 ${isGreen ? "bg-green-400" : "bg-red-400"}`}
                        style={{ height: `${wickTop}px` }}
                      />
                      {/* Corpo */}
                      <div
                        className={`w-3 ${isGreen ? "bg-green-400" : "bg-red-400"}`}
                        style={{ height: `${Math.max(bodyHeight, 1)}px` }}
                      />
                      {/* Wick inferior */}
                      <div
                        className={`w-0.5 ${isGreen ? "bg-green-400" : "bg-red-400"}`}
                        style={{ height: `${wickBottom}px` }}
                      />
                    </div>
                  )
                })}
              </div>
            </div>
          </CardContent>
        </Card>

        {/* Tabs para diferentes seções */}
        <Tabs defaultValue="patterns" className="space-y-4">
          <TabsList className="grid w-full grid-cols-3">
            <TabsTrigger value="patterns">Padrões Detectados</TabsTrigger>
            <TabsTrigger value="orders">Ordens Executadas</TabsTrigger>
            <TabsTrigger value="stats">Estatísticas do Dia</TabsTrigger>
          </TabsList>

          {/* Padrões Detectados */}
          <TabsContent value="patterns">
            <Card>
              <CardHeader>
                <CardTitle className="flex items-center">
                  <Target className="w-5 h-5 mr-2" />
                  Padrões de Candlestick Detectados
                </CardTitle>
              </CardHeader>
              <CardContent>
                <ScrollArea className="h-96">
                  {patterns.length === 0 ? (
                    <p className="text-gray-500 text-center py-8">Aguardando detecção de padrões...</p>
                  ) : (
                    <div className="space-y-3">
                      {patterns.map((pattern) => (
                        <div key={pattern.id} className="border rounded-lg p-4">
                          <div className="flex items-center justify-between mb-2">
                            <div className="flex items-center space-x-2">
                              <Badge
                                variant={
                                  pattern.type === "bullish"
                                    ? "default"
                                    : pattern.type === "bearish"
                                      ? "destructive"
                                      : "secondary"
                                }
                              >
                                {pattern.name}
                              </Badge>
                              <Badge variant="outline">{pattern.confidence}% confiança</Badge>
                            </div>
                            <div className="flex items-center text-sm text-gray-500">
                              <Clock className="w-4 h-4 mr-1" />
                              {pattern.time}
                            </div>
                          </div>
                          <p className="text-sm text-gray-600">{pattern.description}</p>
                        </div>
                      ))}
                    </div>
                  )}
                </ScrollArea>
              </CardContent>
            </Card>
          </TabsContent>

          {/* Ordens Executadas */}
          <TabsContent value="orders">
            <Card>
              <CardHeader>
                <CardTitle className="flex items-center">
                  <DollarSign className="w-5 h-5 mr-2" />
                  Histórico de Ordens
                </CardTitle>
              </CardHeader>
              <CardContent>
                <ScrollArea className="h-96">
                  {orders.length === 0 ? (
                    <p className="text-gray-500 text-center py-8">Nenhuma ordem executada ainda...</p>
                  ) : (
                    <div className="space-y-3">
                      {orders.map((order) => (
                        <div key={order.id} className="border rounded-lg p-4">
                          <div className="flex items-center justify-between mb-2">
                            <div className="flex items-center space-x-2">
                              <Badge variant={order.type === "buy" ? "default" : "destructive"}>
                                {order.type === "buy" ? "COMPRA" : "VENDA"}
                              </Badge>
                              <Badge
                                variant={
                                  order.status === "pending"
                                    ? "secondary"
                                    : order.status === "profit"
                                      ? "default"
                                      : "destructive"
                                }
                              >
                                {order.status === "pending"
                                  ? "Pendente"
                                  : order.status === "profit"
                                    ? "Lucro"
                                    : "Prejuízo"}
                              </Badge>
                              {order.pnl && (
                                <span
                                  className={`text-sm font-medium ${order.pnl > 0 ? "text-green-600" : "text-red-600"}`}
                                >
                                  {formatCurrency(order.pnl)}
                                </span>
                              )}
                            </div>
                            <div className="flex items-center text-sm text-gray-500">
                              <Clock className="w-4 h-4 mr-1" />
                              {order.time}
                            </div>
                          </div>
                          <p className="text-sm text-gray-600 mb-1">
                            <strong>Preço:</strong> {order.price.toFixed(5)}
                          </p>
                          <p className="text-sm text-gray-600">{order.justification}</p>
                        </div>
                      ))}
                    </div>
                  )}
                </ScrollArea>
              </CardContent>
            </Card>
          </TabsContent>

          {/* Estatísticas do Dia */}
          <TabsContent value="stats">
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-5 gap-4">
              <Card>
                <CardHeader className="pb-2">
                  <CardTitle className="text-sm font-medium text-gray-600">Total de Trades</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className="text-2xl font-bold">{dailyStats.totalTrades}</div>
                  <p className="text-sm text-gray-500">Ordens executadas</p>
                </CardContent>
              </Card>

              <Card>
                <CardHeader className="pb-2">
                  <CardTitle className="text-sm font-medium text-gray-600">Taxa de Acerto</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className="text-2xl font-bold text-green-600">{dailyStats.winRate}%</div>
                  <p className="text-sm text-gray-500">Trades lucrativos</p>
                </CardContent>
              </Card>

              <Card>
                <CardHeader className="pb-2">
                  <CardTitle className="text-sm font-medium text-gray-600">P&L Total</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className={`text-2xl font-bold ${dailyStats.totalPnL >= 0 ? "text-green-600" : "text-red-600"}`}>
                    {formatCurrency(dailyStats.totalPnL)}
                  </div>
                  <p className="text-sm text-gray-500">Resultado do dia</p>
                </CardContent>
              </Card>

              <Card>
                <CardHeader className="pb-2">
                  <CardTitle className="text-sm font-medium text-gray-600">Melhor Trade</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className="text-2xl font-bold text-green-600">{formatCurrency(dailyStats.bestTrade)}</div>
                  <p className="text-sm text-gray-500">Maior lucro</p>
                </CardContent>
              </Card>

              <Card>
                <CardHeader className="pb-2">
                  <CardTitle className="text-sm font-medium text-gray-600">Pior Trade</CardTitle>
                </CardHeader>
                <CardContent>
                  <div className="text-2xl font-bold text-red-600">{formatCurrency(dailyStats.worstTrade)}</div>
                  <p className="text-sm text-gray-500">Maior prejuízo</p>
                </CardContent>
              </Card>
            </div>
          </TabsContent>
        </Tabs>
      </div>
    </div>
  )
}
