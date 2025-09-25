#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>

// === Configurações da rede Wi-Fi ===
const char* SECRET_SSID = "Wokwi-GUEST";
const char* SECRET_PW = "";

// === Configurações do ThingSpeak ===
WiFiClient client;
unsigned long channelID = 2950926;
const char* writeAPIKey = "I4Y0E8MYP7ECSPR8";

// === Configuração do endpoint da API via ngrok ===
const char* API_ENDPOINT = "http://127.0.0.1:8080/movimentacoes";
const char* LOGIN_ENDPOINT = "http://127.0.0.1:8080/auth/login";

// === Usuário para login ===
const char* USER_EMAIL = "augusto@email.com";
const char* USER_PASSWORD = "augusto123";

// Variável global para armazenar o token
String jwtToken = "";

const int buttonPin = 12;
const int ledPin = 2;

int idDepartamento = 1;
bool lastButtonState = HIGH;

void autenticarAPI() {
  HTTPClient http;
  http.begin(client, LOGIN_ENDPOINT);
  http.addHeader("Content-Type", "application/json");

  String loginPayload = "{\"email\":\"" + String(USER_EMAIL) + "\",\"senha\":\"" + String(USER_PASSWORD) + "\"}";

  int httpResponseCode = http.POST(loginPayload);

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Login bem-sucedido: " + response);

    // O token normalmente vem no JSON, tipo: {"token":"xxx"}
    int tokenStart = response.indexOf(":\"") + 2;
    int tokenEnd = response.indexOf("\"", tokenStart);
    jwtToken = response.substring(tokenStart, tokenEnd);

    Serial.println("Token JWT obtido: " + jwtToken);
  } else {
    Serial.println("Erro ao autenticar. Código HTTP: " + String(httpResponseCode));
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

  WiFi.begin(SECRET_SSID, SECRET_PW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.println("IP: " + WiFi.localIP().toString());

  // Autentica na API e pega o token
  autenticarAPI();

  Serial.println("Digite o ID da moto no monitor serial.");
}

void loop() {
  static int idMoto = 0;

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
      Serial.println("Dados enviados ao ThingSpeak com sucesso.");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("Erro ao enviar para ThingSpeak: " + String(status));
      digitalWrite(ledPin, LOW);
    }

    // === Envio real para a API com JWT ===
    if (jwtToken != "") {
      String jsonPayload = "{\"moto\":{\"id_moto\":" + String(idMoto) + "},\"departamento\":{\"id_departamento\":" + String(idDepartamento) + "}}";

      HTTPClient http;
      http.begin(client, API_ENDPOINT);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Authorization", "Bearer " + jwtToken); // Token JWT no header

      int httpResponseCode = http.POST(jsonPayload);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Requisição HTTP feita com sucesso.");
        Serial.println("Código de resposta: " + String(httpResponseCode));
        Serial.println("Resposta: " + response);
      } else {
        Serial.println("Falha na requisição HTTP.");
        Serial.println("Código de erro: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("⚠️ Token JWT não disponível. Verifique autenticação.");
    }

    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(20000); // Delay de 20s para respeitar ThingSpeak
  }

  lastButtonState = buttonState;
}
