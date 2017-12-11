/** 
 * A7lib Version 1.0
 * modified by Osmar Gonzalez
 * osmar.itzahel.gonzalez@gmail.com
 * 
 * adopted from
 * 
 * SerialGSM version 1.1
 * by Meir Michanie
 * meirm@riunx.com
 * https://github.com/meirm/SerialGSM
**/
#ifndef _A7lib_H
#define _A7lib_H
#include "Arduino.h"

#define SERIALTIMEOUT 2000
#define PHONESIZE 12
#define MAXMSGLEN 160
#define BOOTTIME 10

#define GPS_GMAP_URL_SIZE 64
#define GPS_BUFFER_SIZE 128

class A7lib{
  private:
    Stream *_modem;
    Stream *_gps;
    
    bool readGPSbuffer();
    void SendSMS();
    void EndSMS();
    void StartSMS();
    int ReadLine();
    bool readCoordinates();
    bool ExtractValues();
    float ExtractValue(unsigned char&);
    char ExtractNESW(unsigned char&);
    float ConvertDegrees(float);
    char Skip(unsigned char);
    void clearBufferArray();
    void clearGmapURL();
    
  public:
    A7lib(Stream *, Stream *);
    A7lib(Stream &, Stream &);
    void FwdSMS2Serial();
    void DoNotFwdSMS2Serial();
    void SendSMS(char *, char *);
    void DeleteAllSMS();
    void Reset();
    void answer();
    void hangUp();
    void dial(char *);
    int ReceiveSMS();
    void turnOnGPS();
    void turnOffGPS();
    void turnOnGPSRD();
    void turnOffGPSRD();
    char * getGmapURL();
    bool GPSlock();
    void Debug(bool);
    bool Debug();
    void Sender(char *);
    char * Sender();
    void Rcpt(char *);
    char * Rcpt();
    void Message(char *);
    char * Message();
    void Boot();

    /* GSM */
    bool debug;
    char sendernumber[PHONESIZE + 1];
    char rcpt[PHONESIZE + 1];
    char outmessage[80];
    char inmessage[80];
  
    /* GPS */
    char GmapURL[GPS_GMAP_URL_SIZE];
    char buffer[GPS_BUFFER_SIZE];
    char NS = '\0';
    char EW = '\0';
    float latitude;
    float longitude;
    int count = 0;
    
  protected:
    unsigned long lastrec;
};

#endif /* not defined _A7lib_H */

