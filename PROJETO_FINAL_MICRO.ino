#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11
float tempEsperada=0,umiEsperada=0;
float umiSensor=0,tempSensor=0;
const int pinoLed = 12;
const int pinoLed2 = 04;
int val = 0; 

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Familia 2G";
const char* password = "MIRANDA2G";
  
 void desSerial(String payload){
  StaticJsonDocument<6000> doc;
  DeserializationError err = deserializeJson(doc,payload);
    if(err){
      Serial.print("erro");
      Serial.println(err.c_str());
      return;
}
  String list = doc["list"][1];

  StaticJsonDocument<6000> doc2;
  DeserializationError err2 = deserializeJson(doc2,list);
    if(err2){
      Serial.print("erro");
      Serial.println(err2.c_str());
      return;
}
  String main = doc2["main"];

  StaticJsonDocument<6000> doc3;
  DeserializationError err3 = deserializeJson(doc3,main);
    if(err3){
      Serial.print("erro");
      Serial.println(err3.c_str());
      return;
}


  float message = doc["message"];
  int cnt = doc["cnt"];
  float tempNuvem = doc3["temp"];
  float umiNuvem = doc3["humidity"];
  tempEsperada=tempNuvem;
  umiEsperada=umiNuvem;
  

  return;
  }

void setup () {
 
  Serial.begin(115200);
  dht.begin();
  delay(10);
  WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print("Connecting..");
}
  Serial.println("WiFi conectado");
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(pinoLed2, OUTPUT);
  digitalWrite(pinoLed2, LOW);
  digitalWrite(pinoLed, LOW); //LED INICIA DESLIGADO

}
 
void loop() {
  float u = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(t) || isnan(u)){
    Serial.println("Falha de leitura");
  }
  

  Serial.print("Umidade:");
  Serial.print(u);
  Serial.print(",");
  Serial.print("Temperatura:");
  Serial.println(t);
  
  
  if(u < 50){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
     digitalWrite(pinoLed, HIGH); //ACENDE O LED
 }else{ 
   digitalWrite(pinoLed, LOW); //APAGA O LED
 }
   if(t > 28){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
     digitalWrite(pinoLed2,HIGH);
     
 }else{ //SENÃO, FAZ
   digitalWrite(pinoLed2,LOW);
  
 }
  //NUVEM
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://api.openweathermap.org/data/2.5/forecast?q=maracanau,BR&cnt=3&appid=38dca12bdf43c4cb127fb9079d7c2d40");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();   //Get the request response payload
             
    desSerial(payload);
    http.end();   //Close connection
    Serial.print("Valor equacao: ");
 
   Serial.print("Valor u: ");
  Serial.println(u);
 Serial.print("Valor t: ");
  Serial.println(t);
 Serial.print("Valor previsao u ");
  Serial.println(umiEsperada);
  Serial.print("Valor previsao t ");
  Serial.println(tempEsperada);
  
}

delay(5000);    //Send a request every 30 seconds
 
}
}
