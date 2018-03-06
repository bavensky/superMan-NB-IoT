#include "AIS_NB_IoT.h"
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

String serverIP = "===censor===";
String serverPort = "===censor===";

AIS_NB_IoT AISnb;
AIS_NB_IoT_RES resp;

const long interval = 60000;  //millisecond
unsigned long previousMillis = 0;

long cnt = 0;


int field1 = 0;
int field2 = 0;
int field3 = 0;
int field4 = 0;
int field5 = 0;
int field6 = 0;

void setup()
{
  Wire.begin();
  lightMeter.begin();

  dht.begin();
    
  AISnb.debug = true;

  delay(2000);
  Serial.begin(9600);

  AISnb.setupDevice(serverPort);

  String ip1 = AISnb.getDeviceIP();
  pingRESP pingR = AISnb.pingIP(serverIP);

  previousMillis = millis();

}

void loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10 * interval)
  {
    field1 = dht.readTemperature();
    field2 = dht.readHumidity();
    field3 = lightMeter.readLightLevel();
    field4 = 0;
    field5 = analogRead(A0) * (5.0 / 1023.0);
    field6 = cnt++;

    // Send data in String
    //UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort,"{\"myName\": \"SuperMAN\",\"field1\": 30,\"field2\":2,\"field3\":3}");
    UDPSend udp = AISnb.sendUDPmsgStr(serverIP, serverPort, "{\"myName\": \"SuperMAN\",\"field1\": " + String(field1) + ",\"field2\":" + String(field2) + ",\"field3\":" + String(field3) + ",\"field4\":" + String(field4) + ",\"field5\":" + String(field5) + ",\"field6\":" + String(field6) + "}");

    previousMillis = currentMillis;

  }

  UDPReceive resp = AISnb.waitResponse();

}



