#include <A7lib.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h>

SoftwareSerial modem(2, 3); // RX, TX
AltSoftSerial gps(8, 9); // RX, TX
A7lib cell(modem, gps);

String authPhoneNmbrs[] = {"1XXXXXXXXXX" , "1XXXXXXXXXX"};
int authPhoneNmbrsCnt = sizeof(authPhoneNmbrs) / sizeof(authPhoneNmbrs[0]);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  modem.begin(57600);
  gps.begin(9600);
  cell.Debug(true);
  cell.Boot();
  cell.turnOnGPS();
  cell.FwdSMS2Serial();
  cell.DeleteAllSMS();

  while (! cell.GPSlock()){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(F("No GPS lock yet, trying again in 5 seconds."));
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
  }
  Serial.println(F("GPS locked"));
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.print(F("Listening from: "));
  for (int i = 0; i < authPhoneNmbrsCnt; i++){
    Serial.print(authPhoneNmbrs[i]);
    Serial.print(F(" "));
  }
  Serial.println(F(""));
}

void loop() {
  if (cell.ReceiveSMS()) {
    delay(200);
    for (int i = 0; i < authPhoneNmbrsCnt; i++) {
      if (strstr(authPhoneNmbrs[i].c_str(), cell.Sender()) != NULL ) {
        Serial.print("AUTHORIZED NUMBER: ");
        Serial.println(cell.Sender());
        //Serial.println(cell.getGmapURL()); // Print Google Maps URL instead of sending
        cell.SendSMS(cell.Sender(),cell.getGmapURL());
      }
    }
    cell.DeleteAllSMS();
  }
}




