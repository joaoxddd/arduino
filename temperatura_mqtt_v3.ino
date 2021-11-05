#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#include "UbidotsESPMQTT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
#define TOKEN "BBFF-KDipF7tE4GwIqlFInbn1A1M48UP8ri"
#define MQTTCLIENTNAME "CLIENTESENSORTEMPERATURA"

DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "GVT-3AB3";
const char * senha = "9003000533";
Ubidots cliente (TOKEN, MQTTCLIENTNAME);
WiFiServer server(80);


void callback(char *topic, byte* payload, unsigned int length){
}

const int pinoLed = 12;
const int pinoSensorChuva = A0;
int val = 0; 

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(10);
  Serial.print("Conectando a rede:");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
  server.begin();
  Serial.println("Servidor iniciado");

  Serial.print("Use esta URL:");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  pinMode(pinoSensorChuva, INPUT); //DEFINE O PINO COMO ENTRADA
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
  digitalWrite(pinoLed, LOW); //LED INICIA DESLIGADO
  
  cliente.begin(callback);
}
void loop() {

  WiFiClient cliente1 = server.available();
  if(!cliente1){
    return;
  }
  Serial.println("Novo cliente");
  while(!cliente1.available()){
    delay(1);
  }
  String requisicao = cliente1.readStringUntil('\r');
  Serial.println(requisicao);
  cliente1.flush();

  int status = LOW;
  if(requisicao.indexOf("/LED=ON") != -1) {
    status = HIGH;
  }
  if(requisicao.indexOf("/LED=OFF") != -1) {
    status = LOW;
  }
  Serial.print("status:");
  Serial.println(status);
  digitalWrite(pinoLed, status);

  cliente1.println("HTTP/1.1 200 OK");
  cliente1.println("Content-Type:text/html");
  cliente1.println("");
  cliente1.println("<!DOCTYPE HTML>");

  cliente1.println("<html>");
  cliente1.println("<br><br>");
  cliente1.println("First name: <input type=\"/text\"\" name=\"/firstname\"\" id=\"/firstname\"\"><br>");
  cliente1.println("Last name: <input type=\"/text\"\" name=\"/lastname\"\" id=\"/lastname\"\"><br><br>");
  cliente1.println("<input type=\"/submit\"\" value=\"/Submit\"\"></form></body>");

  cliente1.println("</html>");
  
  float u = dht.readHumidity();
  float t = dht.readTemperature();
  val = analogRead(pinoSensorChuva);
  
  if(isnan(t) || isnan(u)){
    Serial.println("Falha de leitura");
  }
  cliente.add("Umidade", u);
  cliente.add("Temperatura", t);

  delay(10000);
  cliente.ubidotsPublish("dht11-v2-");
  cliente.loop();

  Serial.print("Umidade:");
  Serial.print(u);
  Serial.print(",");
  Serial.print("Temperatura:");
  Serial.println(t);
  Serial.print("Estado do Sensor Chuva");
  Serial.println(analogRead(pinoSensorChuva));

  int umidadeEsp=60, temperaturaEsp=30, chanceChuvaEsp=40,chanceChover;
    chanceChover = (umidadeEsp - t)+(temperaturaEsp - u)+ chanceChuvaEsp;

 if(analogRead(pinoSensorChuva) < 700 || chanceChover > 75){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
     digitalWrite(pinoLed, HIGH); //ACENDE O LED
 }else{ //SENÃO, FAZ
   digitalWrite(pinoLed, LOW); //APAGA O LED
 }
  delay(6000);

}
