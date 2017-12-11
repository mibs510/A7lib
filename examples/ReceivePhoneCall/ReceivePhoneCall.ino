#include <A7lib.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

SoftwareSerial modem(2,3); // RX, TX
AltSoftSerial gps(8,9); // RX, TX

A7lib cell(modem,gps);

void setup(){
  Serial.begin(9600);
  cell.begin(57600);
  cell.Debug(true);
  cell.Boot();
  cell.answer();
  delay(10000);
  cell.hangup();
  Serial.println("Done");
}


void loop() {
  
}

