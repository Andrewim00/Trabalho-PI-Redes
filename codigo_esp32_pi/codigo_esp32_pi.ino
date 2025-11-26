#include <WiFi.h>
#include <HTTPClient.h>

// ======== CONFIGURAÇÕES ========
const char* ssid = "Liliane"; // Nome da rede
const char* password = "34966369"; // Senha da rede
String apiKey = "KUTQOPRXL0OIAVOJ"; // API Key do canal no speakthing

// ======== SENSORES E CONEXÓES ========
const int echo = 0;
const int trigger = 1;
const int limite_presenca = 90;

int presenca_anterior = 0;

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
  sensor_read();
  int presenca = (distance <= limite_presenca) ? 1 : 0;
  
  if (presenca != presenca_anterior)
  {
    presenca_anterior = presenca;
    enviarThingSpeak(presenca_anterior);
  }

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
  Serial.println(" cm");
  delay(100);

}

void enviarThingSpeak(int valor) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = "http://api.thingspeak.com/update?api_key=" 
                 + apiKey + "&field1=" + String(valor);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.print("ThingSpeak resposta: ");
      Serial.println(httpCode);
    } else {
      Serial.print("Erro envio: ");
      Serial.println(httpCode);
    }

    http.end();
  }
}
