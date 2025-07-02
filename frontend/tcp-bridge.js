// tcp-bridge.js
const net = require('net');
const express = require('express');
const cors = require('cors');

const app = express();
app.use(cors());

let lastCandle = null;

// TCP Server para receber do C++
const tcpServer = net.createServer((socket) => {
  socket.on('data', (data) => {
    // Espera string tipo: "1.234,1.250,1.220,1.240"
    const [open, high, low, close] = data.toString().split(',').map(Number);
    lastCandle = { open, high, low, close, time: new Date().toLocaleTimeString(), volume: Math.floor(Math.random() * 1000) + 100 };
    console.log('Novo candle recebido:', lastCandle);
  });
});

tcpServer.listen(5050, () => {
  console.log('TCP Bridge escutando na porta 5050');
});

// API HTTP para o React buscar o último candle
app.get('/api/candle', (req, res) => {
  if (lastCandle) {
    res.json(lastCandle);
  } else {
    res.status(204).send();
  }
});

app.listen(3001, () => {
  console.log('API HTTP rodando em http://localhost:3001');
});