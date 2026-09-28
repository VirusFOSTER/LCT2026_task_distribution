const WS_URL = process.env.REACT_APP_WS_URL || 'ws://localhost:8080';

export class WsClient {
  constructor(url = WS_URL, options = {}) {
    this.url = url;
    this.ws = null;
    this.listeners = { open: [], close: [], message: [], error: [] };

    this.reconnectAttempts = 0;
    this.maxReconnectAttempts = options.maxReconnectAttempts ?? 10;
    this.reconnectDelay = options.reconnectDelay ?? 1000; // ms
    this.shouldReconnect = true;
    this.reconnectTimer = null;
  }

  on(event, callback) {
    if (!this.listeners[event]) this.listeners[event] = [];
    this.listeners[event].push(callback);
    return () => {
      this.listeners[event] = this.listeners[event].filter(cb => cb !== callback);
    };
  }

  emit(event, payload) {
    (this.listeners[event] || []).forEach(cb => cb(payload));
  }

  connect() {
    if (this.ws && (this.ws.readyState === WebSocket.OPEN ||
                    this.ws.readyState === WebSocket.CONNECTING)) {
      return;
    }
    this.shouldReconnect = true;

    this.ws = new WebSocket(this.url);

    this.ws.onopen = () => {
      this.reconnectAttempts = 0;
      this.emit('open');
    };

    this.ws.onmessage = (event) => {
      try {
        this.emit('message', JSON.parse(event.data));
      } catch (err) {
        this.emit('error', new Error('Невалидный JSON: ' + err.message));
      }
    };

    this.ws.onerror = (e) => this.emit('error', e);

    this.ws.onclose = () => {
      this.emit('close');
      if (this.shouldReconnect) this.scheduleReconnect();
    };
  }

  scheduleReconnect() {
    if (this.reconnectAttempts >= this.maxReconnectAttempts) {
      this.emit('error', new Error('Превышено число попыток переподключения'));
      return;
    }
    this.reconnectAttempts++;
    // Экспоненциальная задержка: 1s, 2s, 4s, 8s...
    const delay = this.reconnectDelay * Math.pow(2, this.reconnectAttempts - 1);
    this.reconnectTimer = setTimeout(() => this.connect(), delay);
  }

  send(payload) {
    if (!this.isOpen) throw new Error('WebSocket не подключён');
    this.ws.send(JSON.stringify(payload));
  }

  close() {
    this.shouldReconnect = false;
    if (this.reconnectTimer) clearTimeout(this.reconnectTimer);
    if (this.ws) {
      this.ws.close();
      this.ws = null;
    }
  }

  get isOpen() {
    return this.ws && this.ws.readyState === WebSocket.OPEN;
  }
}

export const wsClient = new WsClient();
