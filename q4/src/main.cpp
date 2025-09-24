#include <WiFi.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>

// Wi-Fi
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

// ThingSpeak
WiFiClient client;
unsigned long channelID = 2972280;
const char* writeAPIKey = "ZEX7LVYWNW05LGY6";

// API local
const char* API_ENDPOINT = "http://localhost/api"; // <- substitua

// Pinos
const int buttonPin = 12;
const int ledPin = 2;
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

bool lastButtonState = HIGH;
int idSensor = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi!");

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  ThingSpeak.begin(client);

  Serial.println("Sensor iniciado. Digite o ID do sensor:");
}

// Função leitura do ultrassônico
long readUltrasonicDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
  if (duration == 0) return -1; // falha
  return duration * 0.034 / 2;  // distância em cm
}

void loop() {
  // Leitura do ID via Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0 && input.toInt() > 0) {
      idSensor = input.toInt();
      Serial.println("ID do sensor: " + String(idSensor));
    }
  }

  // Botão pressionado
  bool buttonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && buttonState == LOW && idSensor > 0) {
    // Simulação
    int nivelBateria = random(60, 100); // %
    float temperatura = random(20, 35); // °C simulado
    float umidade = random(40, 70);     // % simulado

    // Ultrassônico
    long distancia = readUltrasonicDistance(TRIG_PIN, ECHO_PIN);

    Serial.println("Leituras:");
    Serial.println("Bateria: " + String(nivelBateria) + "%");
    Serial.println("Temperatura: " + String(temperatura) + " C");
    Serial.println("Umidade: " + String(umidade) + " %");
    Serial.println("Distância: " + String(distancia) + " cm");

    // Envia ao ThingSpeak
    ThingSpeak.setField(1, idSensor);
    ThingSpeak.setField(2, nivelBateria);
    ThingSpeak.setField(3, temperatura);
    ThingSpeak.setField(4, umidade);
    ThingSpeak.setField(5, distancia);

    int responseCode = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (responseCode == 200) {
      Serial.println("Dados enviados ao ThingSpeak com sucesso.");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("Erro ThingSpeak. Código: " + String(responseCode));
      digitalWrite(ledPin, LOW);
    }

    // Envia para API local
    HTTPClient http;
    http.begin(API_ENDPOINT);
    http.addHeader("Content-Type", "application/json");

    String jsonBody = "{\"idSensor\":" + String(idSensor) +
                      ",\"nivelBateria\":" + String(nivelBateria) +
                      ",\"temperatura\":" + String(temperatura, 1) +
                      ",\"umidade\":" + String(umidade, 1) +
                      ",\"distancia\":" + String(distancia) + "}";

    int httpResponseCode = http.POST(jsonBody);
    if (httpResponseCode > 0) {
      Serial.println("Dados enviados à API local.");
    } else {
      Serial.println("Falha ao enviar à API local: " + String(httpResponseCode));
    }
    http.end();

    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(20000); // intervalo mínimo do ThingSpeak
  }

  lastButtonState = buttonState;
}
