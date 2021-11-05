#include <ESP8266WiFi.h>
#include "DHT.h"
#include "UbidotsMicroESP8266.h"

#define DHTPIN 13
#define DHTTYPE DHT11

#define ID_TEMPERATURA "5d35fd131d84725c0dfbd9da"
#define ID_UMIDADE "5d35fd3a1d84725cbea6715f"
#define TOKEN "BBFF-KDipF7tE4GwIqlFInbn1A1M48UP8ri"


DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "GVT-3AB3";
const char* senha = "9003000533";

Ubidots cliente(TOKEN);

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);
    
  Serial.print("Conectando a rede:");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");

}

void loop() {

  float u  = dht.readHumidity();
  float t = dht.readTemperature();

  if( isnan(t) || isnan(u) ){
    Serial.println("Falha de leitura");
  }

  cliente.add(ID_TEMPERATURA, t);
  cliente.add(ID_UMIDADE, u);
  cliente.sendAll();
  
  Serial.print("Umidade:");
  Serial.print(u);
  Serial.print(",");
  Serial.print("temperatura:");
  Serial.print(t);
  
  delay(100); 
}
