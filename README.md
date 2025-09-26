## MotoTrack IoT 🚀🏍️
Projeto desenvolvido para o desafio MotoTrack, com o objetivo de monitorar e rastrear motos em pátios da empresa utilizando ESP32, sensores embarcados e ThingSpeak para visualização dos dados em tempo real, além de uma integração direta com uma API Java Spring Boot exposta pela internet via ngrok.

## 📡 Descrição
Este projeto utiliza um microcontrolador ESP32 para simular o envio de dados (como temperatura e nível de bateria) para a nuvem via ThingSpeak, além de comunicar-se com uma API REST Java responsável por registrar movimentações de motos em diferentes departamentos da empresa.

- A aplicação integra as seguintes tecnologias:
- ESP32 + C++
- ThingSpeak para monitoramento em tempo real
- API REST em Java com Spring Boot
- Exposição da API local usando ngrok (HTTP)
- Simulador Wokwi para testes online
  
#🔑 Novas implementações (Sprint 3):

- Autenticação JWT para comunicação segura com a API Java.
- Backend REST Java com Spring Boot para autenticação e movimentações.
- Backend MVC Java com painel administrativo para gestão de motos e usuários.
- Backend .NET REST para funcionalidades complementares.
- Aplicativo mobile em React Native consumindo ambas as APIs.
- Banco de dados Oracle com versionamento automático via Flyway.

## 📁 Estrutura do Projeto
```bash
codigo-esp32/
├── main.ino       # Código principal do ESP32
├── diagram.json   # diagrama
├── README.md      # Este arquivo
```

## 🔧 Tecnologias Utilizadas

- ✅ ESP32 DevKit
- 🧰 Arduino IDE ou VSCode + PlatformIO
- 📊 ThingSpeak (canal público para dashboard)
- 🌐 WiFi para conexão com internet
- 🔗 ngrok para expor o backend Java
- 🧪 Wokwi (simulador online)
- ☕ Spring Boot API com Oracle DB
- 🔐 JWT (JSON Web Token)
- NGROK

## 📲 Funcionalidades

-🔌 Conexão automática do ESP32 com rede Wi-Fi
- 📤 Envio de dados aleatórios para ThingSpeak:
- ID da moto
- Nível de bateria (%)
- Temperatura (°C)

🔁 Envio de requisição HTTP POST para a API Java com o seguinte payload:
```JSON
{
  "moto": {
    "id_moto": 1
  },
  "departamento": {
    "id_departamento": 1
  }
}
```
⏱ Atualização a cada 20 segundos para respeitar o limite gratuito da ThingSpeak

## 🛠️ Como usar

1. **Clone o repositório:**

   ```bash
   git clone https://github.com/mototrack-challenge/mototrack-IoT.git

2. Abra no VSCode ou Arduino IDE.

3. Configure suas credenciais:

No main.ino, altere:

```cpp
const char* SECRET_SSID = "SUA_REDE_WIFI";<br/>
const char* SECRET_PW   = "SUA_SENHA_WIFI";

unsigned long channelID = 1234567;<br/>
const char* writeAPIKey = "SUA_WRITE_API_KEY";

// Endereço da API Java (usando ngrok HTTP!)<br/>
const char* API_ENDPOINT = "http://SEU_NGROK_URL/movimentacoes/salvar";
```

4. Inicie o servidor backend Java localmente

   
Certifique-se de que a aplicação Spring Boot está rodando na porta 8080:
```bash
./mvnw spring-boot:run
```

5. Exponha a API com ngrok:
```bash
ngrok http 8080
```
Copie a URL gerada (ex: http://abcd1234.ngrok-free.app) e substitua no API_ENDPOINT do código do ESP32.


6. 🔑 Autenticação JWT (Postman + ESP32)

1️⃣ Criar um usuário no Postman
   
 #### 🔐 Usuário

- `POST - /usuarios`  
  Cadastra um novo usuário.

```jsonc
{
  "nome": "Augusto",
  "email": "augustolyra@email.com",
  "senha": "augusto123",
  "perfil": "ADMIN"
}
```

2️⃣ Alterar usuário e senha no código:

```cpp
// === Usuário para login ( TROCAR DE ACORDO COM O USUÁRIO CRIADO NO POSTMAN ) ===
const char* USER_EMAIL    = "augustolyra@email.com";
const char* USER_PASSWORD = "augusto123";
```


## 7. Faça o upload para o ESP32
Use a IDE para carregar o código e abrir o Monitor Serial.

## 8. Interaja via Monitor Serial
Digite o ID da moto, pressione Enter, e depois pressione o botão físico (GPIO 12) para simular o envio dos dados e movimentação.

## 🌐 Painel ThingSpeak <br/>
Acompanhe os dados em tempo real acessando o canal público (https://thingspeak.mathworks.com/channels/2950926/private_show).

## Resultados Parciais Canal ThingSpeak:
![image](https://github.com/user-attachments/assets/72c07521-2ee2-4e87-8d59-7c0a324abc81)

## Resultados Parciais Canal ThingSpeak (Sprint 3):
<img width="1600" height="819" alt="image" src="https://github.com/user-attachments/assets/451e02f0-3d9b-4723-87f6-4c85a19df963" />



## ✍️ Autores
Grupo MotoTrack - FIAP

Participantes:

Augusto Lopes Lyra (RM: 558209)

Felipe Ulson Sora (RM: 555462)

Vinícius Ribeiro Nery Costa (RM: 559165)

📅 Status do Projeto
🟢 Sprint 1/2: Protótipo com envio de dados funcionando.
🟢 Sprint 3: Integração com API Java + JWT e persistência no Banco de Dados.

📌 Licença
Este projeto é de uso educacional, desenvolvido para fins acadêmicos.
---
