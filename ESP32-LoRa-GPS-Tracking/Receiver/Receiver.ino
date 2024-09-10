// RX
#include "LoRa_E32.h"
#include <SoftwareSerial.h>

//LoRa Initials
SoftwareSerial mySerial(25, 26); // Arduino RX <-- E32 TX, Arduino TX --> E32 RX
LoRa_E32 E32(&mySerial);
#define M0 16
#define M1 17


//GPS Initials
#include <TinyGPSPlus.h>
static const int RXPin = 14, TXPin = 12;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);// The serial connection to the GPS device


struct veriler {
  float    enlem;
  float    boylam;
} data;

double rx_Lat;
double rx_Lng;
double tx_Lat;
double tx_Lng;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);


  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  
  E32.begin();
  delay(500);
}

void loop() {
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
      if(gps.location.isValid())
      {
        
        rx_Lat = gps.location.lat();
        rx_Lng = gps.location.lng();
      }
      
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }



  while (E32.available()  > 1) {
 
    // Gelen mesaj okunuyor
    ResponseStructContainer rsc = E32.receiveMessage(sizeof(veriler));
    struct veriler data = (veriler) rsc.data;
    Serial.println("Latitude: "); Serial.println(data.enlem,7); tx_Lat = data.enlem;
    Serial.println("Longtide: "); Serial.println(data.boylam,7); tx_Lng = data.boylam;
    Serial.println("\n");
  

 
    rsc.close();
  }


  float distance = gps.distanceBetween(rx_Lat, rx_Lng, tx_Lat, tx_Lng);
  Serial.println("----Locations------");
  Serial.print("Rx Enlem / Boylam: ");Serial.print(rx_Lat);Serial.print(" , ");Serial.print(rx_Lng); 
  Serial.print("Tx Enlem / Boylam: ");Serial.print(tx_Lat);Serial.print(" , ");Serial.print(tx_Lng);
  Serial.println();
   Serial.println("----Distance------");
  Serial.print("Distance:  ");
  Serial.println(distance);
  
  delay(1000);
}