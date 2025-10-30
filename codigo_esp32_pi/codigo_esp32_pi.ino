#include <WiFi.h>
#include <HTTPClient.h>

// ======== CONFIGURAÇÕES ========
const char* ssid = "SEU_WIFI_SSID"; // Nome da rede
const char* password = "SUA_SENHA_WIFI"; // Senha da rede
String apiKey = "SUA_API_KEY_DO_THINGSPEAK"; // API Key do canal no speakthing

// ======== SENSORES E CONEXÓES ========
const int echo = 0;
const int trigger = 1;

float duration, distance;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  Serial.begin(115200);
  
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  // Garantindo conexáo
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  // put your main code here, to run repeatedly:

}


void sensor_read()
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);
}
