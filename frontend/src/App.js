javascript
import React, { useState, useRef, useEffect } from 'react';

function App() {
  const [name, setName] = useState('');
  const [message, setMessage] = useState('');
  const [response, setResponse] = useState(null);
  const [connected, setConnected] = useState(false);
  const wsRef = useRef(null);

  useEffect(() => {
    const ws = new WebSocket('ws://localhost:8080');
    wsRef.current = ws;

    ws.onopen = () => setConnected(true);
    ws.onclose = () => setConnected(false);
    ws.onmessage = (event) => {
      try {
        setResponse(JSON.parse(event.data));
      } catch (e) {
        setResponse({ status: 'error', error: 'Невалидный JSON от сервера' });
      }
    };

    return () => ws.close();
  }, []);

  const sendData = () => {
    if (!wsRef.current || wsRef.current.readyState !== WebSocket.OPEN) return;
    const payload = { name, message };
    wsRef.current.send(JSON.stringify(payload));
  };

  return (
    <div className="container">
      <h1>ZeroMQ JSON Demo</h1>
      <p>Статус: {connected ? '🟢 Подключено' : '🔴 Отключено'}</p>

      <div className="form">
        <input placeholder="Имя" value={name} onChange={(e) => setName(e.target.value)} />
        <input placeholder="Сообщение" value={message} onChange={(e) => setMessage(e.target.value)} />
        <button onClick={sendData} disabled={!connected}>Отправить JSON</button>
      </div>

      {response && (
        <div className="response">
          <h3>Ответ от C++ сервера:</h3>
          <pre>{JSON.stringify(response, null, 2)}</pre>
        </div>
      )}
    </div>
  );
}

export default App;
