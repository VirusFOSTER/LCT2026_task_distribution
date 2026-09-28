FROM node:20-alpine AS builder
WORKDIR /app
RUN apk add --no-cache --virtual .build-deps make gcc g++ python3
COPY package*.json ./
RUN npm ci --omit=dev
COPY . .

FROM node:20-alpine
WORKDIR /app
# Устанавливаем рантайм-зависимость для zeromq
RUN apk add --no-cache zeromq
COPY --from=builder /app/node_modules ./node_modules
COPY --from=builder /app/bridge.js ./
CMD ["node", "bridge.js"]
