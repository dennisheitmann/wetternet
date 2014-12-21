#include <Wire.h>
#include "Adafruit_BMP085.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "DHT.h"
#include <UIPEthernet.h>

Adafruit_BMP085 pressure;

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

EthernetServer server = EthernetServer(23);

#define DELAYTIME 10000  // 10 sec delay
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

float temp_bmp;
float pres_bmp;
float baro_bmp;
float temp_1wi;
float temp_dht;
float humi_dht;

void setup() {
  Serial.begin(9600);
  pressure.begin();
  sensors.begin();
  dht.begin();

  uint8_t mac[6] = { 0xDE,0xAD,0xBE,0xEF,0xDD,0xEE };
  IPAddress myIP(192,168,1,90);
  Ethernet.begin(mac,myIP);
  server.begin();
} 

void loop() {

  if((millis() - previousMillis) > DELAYTIME)
  {
    readSensors();
    printData();
    previousMillis = millis();
  }

  ethernetClient();

} 

void readSensors() {
  
    temp_bmp = pressure.readTemperature(); 
    pres_bmp = pressure.readPressure();
    baro_bmp = pressure.readSealevelPressure(535) * 0.01;
    sensors.setResolution(TEMP_12_BIT);
    sensors.requestTemperatures();
    temp_1wi = sensors.getTempCByIndex(0);
    temp_dht = dht.readTemperature();
    humi_dht = dht.readHumidity();

}  

void printData() {

  Serial.print("value.temp_bmp ");
  Serial.print(temp_bmp);
  Serial.print(";");
  Serial.print("value.baro_bmp ");
  Serial.print(baro_bmp);
  Serial.print(";");
  Serial.print("value.temp_1wi ");
  Serial.print(temp_1wi);
  Serial.print(";");
  Serial.print("value.temp_dht ");
  Serial.print(temp_dht);
  Serial.print(";");
  Serial.print("value.humi_dht ");
  Serial.print(humi_dht);
  Serial.print(";");
  Serial.println();

}

void ethernetClient() {

  if (EthernetClient client = server.available())
  {
    readSensors();
    client.print("value.temp_bmp ");
    client.print(temp_bmp);
    client.print(";");
    client.print("value.baro_bmp ");
    client.print(baro_bmp);
    client.print(";");
    client.print("value.temp_1wi ");
    client.print(temp_1wi);
    client.print(";");
    client.print("value.temp_dht ");
    client.print(temp_dht);
    client.print(";");
    client.print("value.humi_dht ");
    client.print(humi_dht);
    client.print(";");
    client.println();
    client.println();
    client.stop();
  }
}



