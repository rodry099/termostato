
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <U8g2lib.h>
#include <images.h>

void configInicial();
void arranqueNormal();
void callback(char*, byte*, unsigned int);
void reconnect();
void registerDevice();

WiFiClient espClient;
PubSubClient client(espClient);
//Variables **********************
int pinInt = 2; //pin interruptor
String temperatura;
String id_disp = "";
String output = "";
String canal;
String canalTermostato;
String willmsg; //topic direccion dispositivo
String ServerMqtt; // direccion del Brokker mqtt
String categoria; //categoria del dispositivo
String servidorNodeRed;
unsigned int registrado = 0; ///////provisional volver a poner a cero!!!!!!
int canalRiego;
const char* estadoRiego;

#define DHTPIN 2
#define DHTTYPE DHT11

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,/* reset=*/ U8X8_PIN_NONE);
DHT_Unified dht(DHTPIN, DHTTYPE);

//Inicializacion Wifi **************
IPAddress ip(192, 168, 0, 1);
ESP8266WebServer server(80);
WiFiServer servidor(8080);

void setup() {

   Serial.begin(115200);
   dht.begin();
   u8g2.begin();

   bool result = SPIFFS.begin();
   Serial.println("SPIFFS opened: " + result);

     File f = SPIFFS.open("/configuracion.txt", "r");
      if(!f){
        f.close();
        configInicial(); //inicia como AP y permite configurar conexion a router
      }

    // inicia normalmente conectando a la red que se haya configurado

    arranqueNormalJson();
    servidor.begin();
    client.setServer(ServerMqtt.c_str(), 1883);
    client.setCallback(callback);
    pinMode(pinInt,OUTPUT); //pin para activar rele;
    digitalWrite(pinInt, 1);

    if(registrado == 1){
      Serial.println("Dispositivo registrado en el sistema");
    }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if(registrado == 0){
    client.publish("devices", output.c_str());
    Serial.println("Not registered");
    delay(5000);
    }

  if(registrado == 1){
    // Delay between measurements.
  delay(5000);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
      String topicS = servidorNodeRed + "/" + categoria + "/" + id_disp + "/temp";
      temperatura = "";
      temperatura += event.temperature;
      temperatura = temperatura.substring(0, temperatura.length()-1);
      client.publish(topicS.c_str(), temperatura.c_str());
      Serial.println(topicS);
      Serial.print(temperatura);
    }
    displayTemp();
  }
}
