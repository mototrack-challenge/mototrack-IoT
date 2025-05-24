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

// === Configuração do endpoint da API via ngrok (HTTP, não HTTPS!) ===
const char* API_ENDPOINT = "http://7f0b-2804-7f0-b980-dcb9-5d74-5dc9-68bf-4bf3.ngrok-free.app/movimentacoes/salvar";

// === Pinos ===
const int buttonPin = 12; // Botão verde
const int ledPin = 2;     // LED vermelho

// === Estado ===
int idDepartamento = 1;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("MotoTrack iniciado.");
  Serial.println("Digite o ID da moto no monitor serial.");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando ao WiFi: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(SECRET_SSID, SECRET_PW);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConectado!");
    Serial.println("Endereço IP: " + WiFi.localIP().toString());
  }

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

    // Envio real para a API
    String jsonPayload = "{\"moto\":{\"id_moto\":" + String(idMoto) + "},\"departamento\":{\"id_departamento\":" + String(idDepartamento) + "}}";
    
    HTTPClient http;
    http.begin(client, API_ENDPOINT); // HTTPS support
    http.addHeader("Content-Type", "application/json");

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

    delay(1000);  // Feedback visual
    digitalWrite(ledPin, LOW);
    delay(20000); // Delay de 20s para respeitar ThingSpeak
  }

  lastButtonState = buttonState;
}
