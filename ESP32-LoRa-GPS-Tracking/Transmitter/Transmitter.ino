// TX
#include "LoRa_E32.h"
#include <SoftwareSerial.h>

//LoRa Initials
SoftwareSerial mySerial(25, 26);//rx10 , tx11
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
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }



  data.enlem = gps.location.lat();
  data.boylam = gps.location.lng();
  
  ResponseStatus rs = E32.sendFixedMessage(0, 3, 23, &data, sizeof(veriler));
  Serial.println(rs.getResponseDescription());
  
  delay(2000);
}




void displayInfo()
{

  Serial.println("---------------------------------------");
  Serial.println("Reading Locations from GPS..");
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  Serial.println("---------------------------------------");
}