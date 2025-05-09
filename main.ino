#include <WiFi.h>
#include <ThingSpeak.h>

// === Configurações da rede Wi-Fi ===
const char* SECRET_SSID = "Redmi 12 5G";       // Substitua pela sua rede
const char* SECRET_PW = "toddynho";        // Substitua pela senha da rede

// === Configurações do ThingSpeak ===
WiFiClient client;
unsigned long channelID = 2950926;               // Substitua pelo seu Channel ID
const char* writeAPIKey = "I4Y0E8MYP7ECSPR8";   // Substitua pela sua chave

// Variáveis
int idMoto = 0;
int idDepartamento = 3;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

  Serial.println("MotoTrack iniciado.");
  Serial.println("Digite o ID da moto e pressione Enter:");
}

void loop() {
  // Reconexão WiFi, se necessário
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando ao WiFi: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(SECRET_SSID, SECRET_PW);
      Serial.print(".");
      delay(2000);
    }
    Serial.println("\nConectado!");
    Serial.println("Endereço IP: " + WiFi.localIP().toString());
  }

  // Verifica entrada no monitor serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0 && input.toInt() > 0) {
      idMoto = input.toInt();

      // Envia para os campos do ThingSpeak
      ThingSpeak.setField(1, idMoto);
      ThingSpeak.setField(2, idDepartamento); // pode usar campo 2 para departamento

      int status = ThingSpeak.writeFields(channelID, writeAPIKey);

      if (status == 200) {
        Serial.println("Movimentação registrada com sucesso!");
        Serial.println("ID Moto: " + String(idMoto) + ", Departamento: " + String(idDepartamento));
      } else {
        Serial.print("Erro ao enviar dados: ");
        Serial.println(status);
      }

      delay(20000); // respeita limite do ThingSpeak
    }
  }
}