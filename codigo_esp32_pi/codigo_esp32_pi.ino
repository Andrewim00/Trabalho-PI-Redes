#include <WiFi.h>
#include <HTTPClient.h>

// ======== CONFIGURAÇÕES ========
const char* ssid = "Liliane"; // Nome da rede
const char* password = "34966369"; // Senha da rede
String apiKey = "KUTQOPRXL0OIAVOJ"; // API Key do canal no speakthing

// ======== SENSORES E CONEXÓES ========
const int buzzer = 21;

const int led_verde = 16;
const int led_amarelo = 17;
const int led_vermelho = 5;

const int echo1 = 25;
const int trigger1 = 26;

const int echo2 = 32;
const int trigger2 = 33;

const int limite_presenca = 50;

int presenca_anterior = 0;

float duracao1, duracao2, distancia1, distancia2;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);

  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(led_verde, OUTPUT);
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_vermelho, OUTPUT);

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

  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(buzzer, LOW);
}


void loop() {
  // put your main code here, to run repeatedly:
  sensor_read();
  int presenca1 = (distancia1 <= limite_presenca) ? 1 : 0;
  int presenca2 = (distancia2 <= limite_presenca) ? 1 : 0;
  
  int presenca_atual = presenca2;
  if (presenca_atual != presenca_anterior)
    {
      presenca_anterior = presenca_atual;
      enviarThingSpeak(presenca_atual);
    }

  if (presenca1 && !presenca2)
  {
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    digitalWrite(led_vermelho, LOW);
    digitalWrite(buzzer, LOW);
    return;
  }

  if (presenca2)
  {
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    digitalWrite(buzzer, HIGH);
    return;
  }

  digitalWrite(led_verde, HIGH);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_vermelho, LOW);
  digitalWrite(buzzer, LOW);

}


void sensor_read()
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);

  duracao1 = pulseIn(echo1, HIGH);
  distancia1 = (duracao1*.0343)/2;

  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);

  duracao2 = pulseIn(echo2, HIGH);
  distancia2 = (duracao2*.0343)/2;
  
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
