## MotoTrack IoT 🚀🏍️
Projeto desenvolvido para o desafio MotoTrack, com o objetivo de monitorar e rastrear motos em pátios da empresa utilizando ESP32, sensores embarcados e ThingSpeak para visualização dos dados em tempo real, além de uma integração direta com uma API Java Spring Boot exposta pela internet via ngrok.

## 📡 Descrição
Este projeto utiliza um microcontrolador ESP32 para simular o envio de dados (como temperatura e nível de bateria) para a nuvem via ThingSpeak, além de comunicar-se com uma API REST Java responsável por registrar movimentações de motos em diferentes departamentos da empresa.

A aplicação integra as seguintes tecnologias:

ESP32 + C++

ThingSpeak para monitoramento em tempo real

API REST em Java com Spring Boot

Exposição da API local usando ngrok (HTTP)

Simulador Wokwi para testes online


## 📁 Estrutura do Projeto
codigo-esp32/
├── main.ino       # Código principal do ESP32
├── README.md      # Este arquivo


## 🔧 Tecnologias Utilizadas

- ✅ ESP32 DevKit
- 🧰 Arduino IDE ou VSCode + PlatformIO
- 📊 ThingSpeak (canal público para dashboard)
- 🌐 WiFi para conexão com internet
- 🔗 ngrok para expor o backend Java
- 🧪 Wokwi (simulador online)
- ☕ Spring Boot API com Oracle DB

## 📲 Funcionalidades

-🔌 Conexão automática do ESP32 com rede Wi-Fi
- 📤 Envio de dados aleatórios para ThingSpeak:
- ID da moto
- Nível de bateria (%)
- Temperatura (°C)

🔁 Envio de requisição HTTP POST para a API Java com o seguinte payload:
{
  "moto": {
    "id_moto": 1
  },
  "departamento": {
    "id_departamento": 1
  }
}

⏱ Atualização a cada 20 segundos para respeitar o limite gratuito da ThingSpeak

## 🛠️ Como usar

1. **Clone o repositório:**

   ```bash
   git clone https://github.com/mototrack-challenge/mototrack-IoT.git

2. Abra no VSCode ou Arduino IDE.

3. Configure suas credenciais:

No main.ino, altere:

const char* SECRET_SSID = "SUA_REDE_WIFI"; // altere para sua rede <br/>
const char* SECRET_PW = "SUA_SENHA_WIFI"; // altere para sua senha

unsigned long channelID = 1234567;<br/>
const char* writeAPIKey = "SUA_WRITE_API_KEY";

4. Carregue no ESP32 e monitore a saída serial.

🌐 Painel ThingSpeak
Acompanhe os dados em tempo real acessando o canal público (https://thingspeak.mathworks.com/channels/2950926/private_show).

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
