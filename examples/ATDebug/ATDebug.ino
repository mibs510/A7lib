#include <A7lib.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

SoftwareSerial modem(2,3); // RX, TX
AltSoftSerial gps(8,9); // RX, TX

A7lib cell(modem,gps);

void setup() {
  Serial.begin(9600);
  modem.begin(57600);
  gps.begin(9600);
  cell.Debug(true);
//  cell.Boot();
  Serial.println("===========================================================");
  Serial.println(" You can now send AT commands");
  Serial.println(" Enter \"AT\" (without quotes), and you should see \"OK\"");
  Serial.println(" If it doesn't work, select \"Both NL & CR\" in Serial Monitor");
  Serial.println("===========================================================");
}

void loop() {
  if (modem.available())
    Serial.write(modem.read());
  if (Serial.available())
    modem.write(Serial.read());
}
