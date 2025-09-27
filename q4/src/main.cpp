#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include <ArduinoJson.h>  // Biblioteca para JSON

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

WiFiClient client;
unsigned long channelID = 2950926;
const char* writeAPIKey = "I4Y0E8MYP7ECSPR8";

// === Endpoints da API (Spring Boot via ngrok/IP local - Trocar com base na URL gerada pelo ngrok) ===
const char* API_ENDPOINT   = "http://fd702f3433a2.ngrok-free.app/movimentacoes";
const char* LOGIN_ENDPOINT = "http://fd702f3433a2.ngrok-free.app/auth/login";

// === Usuário para login ( TROCAR DE ACORDO COM O USUÁRIO CRIADO NO POSTMAN ) ===
const char* USER_EMAIL    = "augustolyra@email.com";
const char* USER_PASSWORD = "augusto123";

const int buttonPin = 12;
const int ledPin    = 2;

int idDepartamento = 1;
bool lastButtonState = HIGH;
String jwtToken = "";

// === Função de login ===
void autenticarAPI() {
  HTTPClient http;
  http.begin(LOGIN_ENDPOINT); 
  http.addHeader("Content-Type", "application/json");

  String loginPayload = "{\"email\":\"" + String(USER_EMAIL) + "\",\"senha\":\"" + String(USER_PASSWORD) + "\"}";

  int httpResponseCode = http.POST(loginPayload);

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Login bem-sucedido: " + response);

    // Usa ArduinoJson para extrair o token
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, response);

    if (!error && doc.containsKey("token")) {
      jwtToken = doc["token"].as<String>();
      Serial.println("Token JWT obtido: " + jwtToken);
    } else {
      Serial.println("⚠️ Erro ao processar JSON: " + String(error.c_str()));
    }
  } else {
    Serial.println("Erro ao autenticar. Código HTTP: " + String(httpResponseCode));
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Resposta da API: " + response);
    }
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("MotoTrack iniciado.");
  Serial.println("Conectando WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi conectado!");
  Serial.println("IP: " + WiFi.localIP().toString());

  // Faz login e pega token
  autenticarAPI();

  Serial.println("Digite o ID da moto no monitor serial.");
}

void loop() {
  static int idMoto = 0;

  // Lê ID da moto via Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0 && input.toInt() > 0) {
      idMoto = input.toInt();
      Serial.println("ID da moto recebido: " + String(idMoto));
      Serial.println("Pressione o botão verde para enviar.");
    }
  }

  bool buttonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && buttonState == LOW && idMoto > 0) {
    int bateria = random(10, 100);
    float temperatura = random(200, 350) / 10.0;

    ThingSpeak.setField(1, idMoto);
    ThingSpeak.setField(2, idDepartamento);
    ThingSpeak.setField(3, bateria);
    ThingSpeak.setField(4, temperatura);

    int status = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (status == 200) {
      Serial.println("📡 Dados enviados ao ThingSpeak com sucesso.");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("❌ Erro ao enviar para ThingSpeak: " + String(status));
      digitalWrite(ledPin, LOW);
    }

    // === Envia para API protegida com JWT ===
    if (jwtToken != "") {
      String jsonPayload = "{\"moto_id\":" + String(idMoto) + ",\"departamento_id\":" + String(idDepartamento) + "}";

      HTTPClient http;
      http.begin(API_ENDPOINT);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Authorization", "Bearer " + jwtToken);

      int httpResponseCode = http.POST(jsonPayload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("✅ Requisição enviada para API.");
        Serial.println("Código HTTP: " + String(httpResponseCode));
        Serial.println("Resposta: " + response);
      } else {
        Serial.println("❌ Falha na requisição HTTP.");
        Serial.println("Código de erro: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("⚠️ Token JWT não disponível. Verifique autenticação.");
    }

    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(20000); // Delay de 20s p/ respeitar ThingSpeak
  }

  lastButtonState = buttonState;
}
