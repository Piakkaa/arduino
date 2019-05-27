
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// wifimanager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


#include <SoftwareSerial.h>
SoftwareSerial NanoSerial(D3, D2); // RX | TX

//wifi variables
//const char auth[] = "a7a249ec5f2c41edabf98479ba690559";
const char auth[] = "d7c89935fa6449caabdb6753b6d80b11";





void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// read the stupid board

char datar;

unsigned int dust=0;
unsigned int timer=0;
unsigned int Speed=0;
unsigned int Power=0;
unsigned int Auto=0;

void readReturnSignal() { 
  while(NanoSerial.available()>0){
    
  datar = NanoSerial.read();

  if (datar=='S'){
       Speed = NanoSerial.parseInt(); 
       Blynk.virtualWrite(V5,Speed);
       }
  if (datar=='d'){
       dust = NanoSerial.parseInt();
       Blynk.virtualWrite(V9, dust);
      }
  if (datar=='A'){
       Auto = NanoSerial.parseInt();
       WidgetLED led2(V8);
       if(Auto==1){led2.setValue(255);}
       if(Auto==0){led2.setValue(0);}
      }
   if (datar=='P'){
       Power = NanoSerial.parseInt();
       Blynk.virtualWrite(V0, !Power);
      }
   if (datar=='T'){
       timer = NanoSerial.parseInt(); 
       Blynk.virtualWrite(V6,timer);}      
   }
}


// Blynk read button and Trasfer functions///////////////////////////////////////////////////////////
unsigned int current=0;
unsigned int timesent=0;



void Sensor_DHT(){
  int h = dht.readHumidity();
  int t = dht.readTemperature();

   if (isnan(h) || isnan(t)) {
      return;
   }
  Blynk.virtualWrite(V11,t);
  Blynk.virtualWrite(V10,h);
}



BLYNK_WRITE(V0) // ON-OFF
{

       NanoSerial.print("O"); 

}

BLYNK_WRITE(V1) // Speed
{

       NanoSerial.print("s");
  
}

BLYNK_WRITE(V3) // Timer
{

       NanoSerial.print("t"); 
  
}

BLYNK_WRITE(V4) //Auto
{

       NanoSerial.print("a");  
  
}

/////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  // Debug console
  Serial.begin(9600);

   dht.begin();

  /////////////// wifimanager//////////////////
  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("nodemcu");
  Serial.println("connected...yeey :)");
  Blynk.config(auth);

  /////////////////////////////////Send data////////////////////////////////////
  pinMode(D3, INPUT);
  pinMode(D2, OUTPUT);
  NanoSerial.begin(57600);
}


void loop()
{
  Blynk.run();
  readReturnSignal();
//  sent();
  Sensor_DHT();
}
