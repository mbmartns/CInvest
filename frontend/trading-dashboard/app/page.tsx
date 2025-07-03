"use client"

import { useState } from "react"
import { Card, CardContent, CardHeader, CardTitle } from "@/components/ui/card"
import { Badge } from "@/components/ui/badge"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { ScrollArea } from "@/components/ui/scroll-area"
import { TrendingUp, Activity, DollarSign, Target, Clock } from "lucide-react"
import { useEffect } from "react"

// Tipos de dados
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

export default function TradingDashboard() {
  const [currentPrice] = useState(108800)
  const [patterns, setPatterns] = useState<Pattern[]>([])
  const [orders] = useState<Order[]>([])
  const [dailyStats] = useState<DailyStats>({
    totalTrades: 0,
    winRate: 0,
    totalPnL: 0,
    bestTrade: 0,
    worstTrade: 0,
  })
  const [isConnected] = useState(true)

  const formatCurrency = (value: number) => {
    return new Intl.NumberFormat("pt-BR", {
      style: "currency",
      currency: "USD",
      minimumFractionDigits: 2,
    }).format(value)
  }

  useEffect(() => {
    const interval = setInterval(() => {
      fetch("http://localhost:3001/api/pattern")
        .then(res => res.json())
        .then(data => {
          if (data && data.name) {
            const newPattern: Pattern = {
              id: Date.now().toString(),
              name: data.name,
              type: data.status === "Comprar" ? "bullish" : data.status === "Vender" ? "bearish" : "neutral",
              confidence: 100,
              time: new Date().toLocaleTimeString(),
              description: data.description,
            }
            setPatterns(prev => {
              // Evita duplicatas consecutivas
              if (prev.length > 0 && prev[0].name === newPattern.name && prev[0].description === newPattern.description) {
                return prev
              }
              return [newPattern, ...prev.slice(0, 19)] // mantém só os 20 mais recentes
            })
          }
        })
        .catch(() => {})
    }, 2000) // a cada 2 segundos

    return () => clearInterval(interval)
  }, [])

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
              <div className="text-2xl font-bold">BTC/USD</div>
              <p className="text-sm text-gray-500">Bitcoin / Dólar Americano</p>
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
              <div className="text-2xl font-bold">{currentPrice.toFixed(2)}</div>
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

        {/* Estatísticas do Dia */}
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

        {/* Tabs para diferentes seções */}
        <Tabs defaultValue="orders" className="space-y-4">
          <TabsList className="grid w-full grid-cols-2">
            <TabsTrigger value="orders">Ordens Executadas</TabsTrigger>
            <TabsTrigger value="patterns">Padrões Detectados</TabsTrigger>
          </TabsList>

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
                            <strong>Preço:</strong> {order.price.toFixed(2)}
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
        </Tabs>
      </div>
    </div>
  )
}