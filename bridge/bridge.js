`javascript
import { WebSocketServer } from 'ws';
import * as zmq from 'zeromq';

// Создаём REQ-сокет ZeroMQ для связи с C++ бэкендом
const zmqSocket = new zmq.Request();
zmqSocket.connect('tcp://localhost:5555');

// WebSocket-сервер для браузера
const wss = new WebSocketServer({ port: 8080 });
console.log('WebSocket мост запущен на ws://localhost:8080');

wss.on('connection', (ws) => {
  console.log('Клиент подключился');

  ws.on('message', async (data) => {
    const requestStr = data.toString();
    console.log('От браузера:', requestStr);

try {
      // Отправляем JSON в ZeroMQ
      await zmqSocket.send(requestStr);
      // Ждём ответ
      const [reply] = await zmqSocket.receive();
      const replyStr = reply.toString();
      console.log('От C++:', replyStr);
      // Отправляем ответ обратно в браузер
      ws.send(replyStr);
    } catch (err) {
      console.error('Ошибка:', err);
      ws.send(JSON.stringify({ status: 'error', error: err.message }));
    }
  });

  ws.on('close', () => console.log('Клиент отключился'));
});
