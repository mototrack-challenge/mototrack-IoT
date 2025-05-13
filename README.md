# MotoTrack IoT 🚀🏍️

Projeto desenvolvido para o desafio **MotoTrack**, com o objetivo de monitorar e rastrear motos em pátios da empresa utilizando **ESP32**, sensores embarcados e **ThingSpeak** para visualização dos dados em tempo real.

## 📡 Descrição

Este projeto utiliza um microcontrolador **ESP32** para simular o envio de dados (como temperatura, umidade, localização ou outros sensores) para a nuvem, usando o serviço **ThingSpeak**. Ele faz parte de uma arquitetura IoT pensada para rastrear motos de forma inteligente.

## 📁 Estrutura do Projeto
codigo-esp32/
├── main.ino # Código principal do ESP32
├── README.md # Este arquivo

## 🔧 Tecnologias Utilizadas

- ESP32 DevKit
- Arduino IDE / VSCode com extensões de IoT
- ThingSpeak (canal de nuvem para dados)
- WiFi para comunicação com a internet
- Wokwi (simulador)

## 📲 Funcionalidades

- Conexão automática do ESP32 com rede WiFi
- Envio de múltiplos dados para o ThingSpeak
- Leitura/simulação de sensores (valores aleatórios por enquanto)
- Atualização a cada 20 segundos para respeitar a limitação gratuita

## 🛠️ Como usar

1. **Clone o repositório:**

   ```bash
   git clone https://github.com/mototrack-challenge/mototrack-IoT.git

2. Abra no VSCode ou Arduino IDE.

3. Configure suas credenciais:

  No main.ino, altere:
cpp
Copiar
Editar
const char* SECRET_SSID = "SUA_REDE_WIFI";
const char* SECRET_PW = "SUA_SENHA_WIFI";
unsigned long channelID = 1234567;
const char* writeAPIKey = "SUA_WRITE_API_KEY";

4. Carregue no ESP32 e monitore a saída serial.

🌐 Painel ThingSpeak
Acompanhe os dados em tempo real acessando o canal público (link será adicionado aqui quando disponível).

✍️ Autores
Grupo MotoTrack - FIAP

Participantes:

Augusto Lopes Lyra (RM: 558209)

Felipe Ulson Sora (RM: 555462)

Vinícius Ribeiro Nery Costa (RM: 559165)

📅 Status do Projeto
🟢 Sprint 1: Protótipo com envio de dados funcionando.

📌 Licença
Este projeto é de uso educacional, desenvolvido para fins acadêmicos.
---
