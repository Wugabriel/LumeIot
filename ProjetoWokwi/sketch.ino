#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

String apiKey = "1RUJVTEZ4NECR8GC"; 
const char* server = "http://api.thingspeak.com/update";

enum NivelDeEstresse {
  LEVE,
  MEDIO,
  ALTO,
  MUITO_ALTO
};

NivelDeEstresse calcularNivelDeEstresse(int bpm, float temperatura) {
  if (bpm <= 0 || temperatura <= 0) return MEDIO;
  if (bpm < 80 && temperatura < 37.0) return LEVE;
  if (bpm < 100 && temperatura < 37.5) return MEDIO;
  if (bpm < 120 && temperatura < 38.0) return ALTO;
  return MUITO_ALTO;
}

String estresseToString(NivelDeEstresse n) {
  switch (n) {
    case LEVE: return "LEVE";
    case MEDIO: return "MEDIO";
    case ALTO: return "ALTO";
    case MUITO_ALTO: return "MUITO ALTO";
  }
  return "DESCONHECIDO";
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Conectado ao WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    int batimentos = random(60, 120);
    float temperatura = random(360, 390) / 10.0;

    NivelDeEstresse nivel = calcularNivelDeEstresse(batimentos, temperatura);
    String nivelStr = estresseToString(nivel);

    String url = String(server) +
                 "?api_key=" + apiKey +
                 "&field1=" + String(batimentos) +
                 "&field2=" + String(temperatura);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print(" Enviado → BPM=");
      Serial.print(batimentos);
      Serial.print(" | Temp=");
      Serial.print(temperatura);
      Serial.print(" | Estresse=");
      Serial.print(nivelStr);
      Serial.print(" | Código=");
      Serial.println(httpResponseCode);
    } else {
      Serial.print(" Erro HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();

  } else {
    Serial.println(" WiFi desconectado!");
  }

  delay(15000); // envia a cada 15s
}