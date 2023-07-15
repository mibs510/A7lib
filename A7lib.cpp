/** 
 * A7lib Version 1.0
 * modified by Connor McMillan
 * 
 * adopted from
 * 
 * SerialGSM version 1.1
 * by Meir Michanie
 * meirm@riunx.com
 * https://github.com/meirm/SerialGSM
**/
#include <A7lib.h>

A7lib::A7lib(Stream *modem, Stream *gps) : _modem(modem), _gps(gps){
  debug = false;
}

A7lib::A7lib(Stream &modem, Stream &gps) : _modem(&modem), _gps(&gps){
  debug = false;
}

void A7lib::FwdSMS2Serial(){
  if (debug)
    Serial.println(F("AT+CMGF=1")); // set SMS mode to text
  
  _modem->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  this->ReadLine();
  
  if (debug)
   Serial.println(F("AT+CNMI=2,2,0,0,0")); // set module to send SMS data to serial out upon receipt 
  
  _modem->println("AT+CNMI=2,2,0,0,0"); // set module to send SMS data to serial out upon receipt 
  delay(200);
  this->ReadLine();
}

void A7lib::DoNotFwdSMS2Serial(){
  if (debug)
    Serial.println(F("AT+CMGF=0")); // set SMS mode to text
  
  _modem->println("AT+CMGF=0"); // set SMS mode to text
  delay(200);
  this->ReadLine();
  
  if (debug)
    Serial.println(F("AT+CNMI=0,2,0,0,0")); // set module to send SMS data to serial out upon receipt 
  
  _modem->println("AT+CNMI=0,2,0,0,0"); // set module to send SMS data to serial out upon receipt 
  delay(200);
  this->ReadLine();
}

void A7lib::answer(){
  _modem->println("ATA");
}

void A7lib::hangUp(){
  _modem->println("ATH");
}

void A7lib::dial(char * pnumber){
  this->Rcpt(pnumber);
  _modem->print("ATD");
  _modem->println(pnumber);
}

void A7lib::SendSMS(char * cellnumber, char * outmsg){
  this->Rcpt(cellnumber);
  this->Message(outmsg);
  
  if (debug){
    Serial.print(F("Sending to: "));
    Serial.println(rcpt);
    Serial.print(F("Message: "));
    Serial.println(outmessage);
    Serial.println(F(""));
  }
  
  this->StartSMS();
  _modem->print(outmessage);
  this->EndSMS();
  delay(500);
  this->ReadLine();
}

void A7lib::SendSMS(){
  if (debug){
    Serial.print(F("Sending to: "));
    Serial.println(rcpt);
    Serial.print(F("Message: "));
    Serial.println(outmessage);
    Serial.println(F(""));
  }
  
  this->StartSMS();
  
  _modem->print(outmessage);
  this->EndSMS();
  delay(500);
  this->ReadLine();
}

void A7lib::DeleteAllSMS(){
  if (debug)
    Serial.println(F("AT+CMGD=1,4")); // delete all SMS
  
  _modem->println("AT+CMGD=1,4"); // delete all SMS
  delay(200);
  this->ReadLine();
}

void A7lib::Reset(){
  if (debug)
    Serial.println(F("AT+CFUN=1,1")); // Reset Modem
  
  _modem->println("AT+CFUN=1,1"); // Reset Modem
  delay(200);
  this->ReadLine();
}


void A7lib::EndSMS(){
  _modem->print(char(26));  // ASCII equivalent of Ctrl-Z
  Serial.println(F(""));

  //delay(5 * 1000); // the SMS module needs time to return to OK status
}

void A7lib::StartSMS(){
  if (debug)
    Serial.println(F("AT+CMGF=1")); // set SMS mode to text
  
  _modem->println("AT+CMGF=1"); // set SMS mode to text
  delay(200);
  this->ReadLine();
  
  if (debug)
    Serial.print(F("AT+CMGS="));
  
  _modem->print("AT+CMGS=");
  _modem->print(char(34)); // ASCII equivalent of "
  
  _modem->print(rcpt);
  _modem->println(char(34));  // ASCII equivalent of "
  delay(500); // give the module some thinking time
  this->ReadLine();

}

int A7lib::ReadLine(){
  static int pos = 0;
  char nc;
  
  while (_modem->available()){
    nc = _modem->read();
    
    if (nc == '\n' or (pos > MAXMSGLEN) or ((millis()> lastrec + SERIALTIMEOUT) and (pos > 0))){
      nc = '\0';
      lastrec = millis();
      inmessage[pos] = nc;
      pos = 0;
      
//      if (debug)
//        Serial.println(F(inmessage));
      
      return 1;
    }
    else if (nc == '\r') {
    }
    else{
      inmessage[pos++] = nc;
      lastrec = millis();
    }
  }
  return 0;
}

int A7lib::ReceiveSMS(){
  static bool insms = 0;
  
  if (this->ReadLine()){
    // Get the number of the sms sender in order to be able to reply
	  if ( strstr(inmessage, "+CMT: \"+") != NULL ){
	      insms = 1;
	      int sf = 8;
      
        for (int i = 0; i < PHONESIZE; i++){
          sendernumber[i] = inmessage[sf+i];
        }
      
        sendernumber[PHONESIZE - 1] = '\0';
      
        return 0;
    }else{
        if(insms) {
			    insms = 0;
			    return 1;
        }
	  }
  }
  return 0;
}

void A7lib::turnOnGPS(){
  if (debug)
    Serial.println(F("AT+GPS=1"));
  _modem->println("AT+GPS=1");
  delay(200);
  this->ReadLine();
}

void A7lib::turnOffGPS(){
  if (debug)
    Serial.println(F("AT+GPS=0"));
  _modem->println("AT+GPS=0");
}

void A7lib::turnOnGPSRD(){
  if (debug)
    Serial.println(F("AT+GPSRD=1"));
  _modem->println("AT+GPSRD=1");
}

void A7lib::turnOffGPSRD(){
  if (debug)
    Serial.println(F("AT+GPSRD=0"));
  _modem->println("AT+GPSRD=0");
}

char * A7lib::getGmapURL(){
  String buff;
  bool returnValue = false;
  
  clearGmapURL();
  returnValue = readCoordinates();
  
  if(returnValue && longitude != 0.00 && latitude != 0.00 && (NS != '\0' || EW != '\0')){
    if (debug){
      Serial.print(F("Coordinates: "));
      Serial.print(latitude, 7);
      Serial.print(NS);
      Serial.print(F(","));
      Serial.print(longitude, 7);
      Serial.println(EW);
      Serial.println(F(""));
    }
    
    buff += F("http://maps.google.com/maps?q=");
    if (NS == 'S')
      buff += F("-");
    buff += String(latitude,9);
    buff += F("%2c");
    if (EW == 'W')
      buff += F("-");
    buff += String(longitude,9);
    
    strcat(GmapURL,buff.c_str());
    
    NS = '\0';
    EW = '\0';
  } else {
    getGmapURL();
  }
  return GmapURL;
}

bool A7lib::readGPSbuffer(){
  if (_gps->available()){
    while(_gps->available()){
      buffer[count++] = _gps->read();
      if(count == GPS_BUFFER_SIZE)
        break;
    }
  }
  
  if (debug){
    Serial.print(F("buffer[] before strstr() = "));
    Serial.println(buffer);
  }
  
  if (strstr(buffer, "$GPGGA") == NULL){
    if (debug)
      Serial.println(F("No $GPGGA sentence found!"));
    clearBufferArray();
    NS = '\0';
    EW = '\0';
    return false;
  }
    
  snprintf(buffer, GPS_BUFFER_SIZE, "%s", strstr(buffer, "$GPGGA"));
  
  if (debug){
    Serial.print(F("buffer[] after strstr() = "));
    Serial.println(buffer);
  }
  
}

bool A7lib::readCoordinates(){
  bool foundGPGGA = false;
  
  if (! readGPSbuffer())
    return false;
  
  foundGPGGA = count > 45 && ExtractValues();
  clearBufferArray();

  return foundGPGGA;
}

bool A7lib::GPSlock(){
  int lonelyCommas = 0;
  int j = 0;
  
  if (! readGPSbuffer()){
    clearBufferArray();
    return false;
  }
    
  if ( count < 45 ){
    clearBufferArray();
    return false;
  }
  
  for (int i = 0; i < count; i++){
    j = i + 1;
    if (buffer[i] == ',' && buffer[j] == ','){
      lonelyCommas++;
    }
  }
  
  if (debug){
    Serial.print(F("Lonely commas: "));
    Serial.println(lonelyCommas);
  }
  
  if (lonelyCommas > 2){
    clearBufferArray(); 
    return false;
  }
  else{
    clearBufferArray();
  }
  return true;
}

bool A7lib::ExtractValues(){
  unsigned char start = 7;
  
  start = Skip(start);
  latitude = ConvertDegrees(ExtractValue(start) / 100);
  NS = ExtractNESW(start);
  
  if (debug){
    Serial.print(F("NS = "));
    Serial.println(NS);
  }
  if (NS != 'N' && NS != 'S')
    return false;
  
  longitude = ConvertDegrees(ExtractValue(start) / 100);
  EW = ExtractNESW(start);
  
  if (debug){
    Serial.print(F("EW = "));
    Serial.println(EW);
  }
  if (EW != 'E' && EW != 'W')
    return false;
  
  start = Skip(start);
  start = Skip(start);
  start = Skip(start);
  
  return true;
}

float A7lib::ExtractValue(unsigned char& start){
  unsigned char end = start + 1;
  while(end < count && buffer[end] != ',')
    end++;
    
  buffer[end] = 0;
  float result = 0.0;
  if(end != start + 1)
    result = String((const char*)(buffer + start)).toFloat();
    
  start = end + 1;
  return result;
}

char A7lib::ExtractNESW(unsigned char& start){
  char result = buffer[start++];
  start++;
  return result;
}

float A7lib::ConvertDegrees(float input){
  float fractional = input - (int)input;
  return (int)input + (fractional / 60.0) * 100.0;
}

char A7lib::Skip(unsigned char start){
  unsigned char end = start + 1;
  while(end < count && buffer[end] != ',')
    end++;
  return end+1;
}

void A7lib::clearBufferArray(){
  memset(buffer, 0, GPS_BUFFER_SIZE);
  count = 0;
}

void A7lib::clearGmapURL(){
  memset(GmapURL, 0, GPS_GMAP_URL_SIZE);
}

bool A7lib::Debug(){
	return debug;
}

void A7lib::Debug(bool var){
	debug = var;
}

char * A7lib::Sender(){
	return sendernumber;
}

char * A7lib::Rcpt(){
	return rcpt;
}

char * A7lib::Message(){
	return inmessage;
}

void A7lib::Sender(char * var1){
	sprintf(sendernumber,"%s",var1);
}

void A7lib::Rcpt(char * var1){
	sprintf(rcpt,"%s",var1);
}

void A7lib::Message(char * var1){
	sprintf(outmessage,"%s",var1);
}

void A7lib::Boot(){
  int counter = 0;
  if (debug)
    Serial.print(F("Booting"));
  while(counter++ < BOOTTIME){
    if (debug)
      Serial.print(F("."));
    delay(1000);
  }
  
  if (debug)
    Serial.println(F(""));
}
