#include <Arduino.h>
#include <Wire.h>
#include "SevSeg.h"
#include <DS3231.h>
#include <OneWire.h>


// ---- Time Sensor DS3231 Srart ----

  void getTime();
  void getTemp();
  boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours);

  DS3231 Clock;
  bool Century = false;
  bool h12;
  bool PM;
  boolean isSummer = true;
  short jahr, monat, tag, stunden, minuten, sekunden;
  long temperatur; 

  int time = 0;
  int stateTime = 0;
  unsigned long nTtE;
  int tempState;
  int waitTemp = 5000; 
  unsigned long nTtETemp;
  int wait = 1000;

  int loopWait = 2;
  int loopstate;
  unsigned long nTtELoop;

// ---- Time Sensor DS3231 End ----

// ---- 7-Segment Start ----

  SevSeg sevseg;

  void setUPSevSeg();
  void printTime();


// ---- 7-Segment End ----

void makeEverythingInLoop();

void setup() {

  Serial.begin(115200);
  Wire.begin();

  setUPSevSeg(); 
 
}

void loop() {
  makeEverythingInLoop();
  sevseg.refreshDisplay();
}

void getTemp(){
  
  switch (tempState)
  {
  case 0:
    nTtETemp = millis() + waitTemp;
    tempState = 1; 
    break;
  case 1: 
    if(millis() > nTtETemp){
      
      temperatur = Clock.getTemperature();
      //Serial.print(temperatur);	
      tempState = 0; 
    }
    break;
  default:
    break;
  } 

}

void setUPSevSeg(){
  byte numDigits = 4; //Hier wird die Anzahl der Ziffern angegeben
  byte digitPins[] = {
      2,
      3,
      4,
      5
    }; //Die Pins zu den Ziffern werden festgelegt
    byte segmentPins[] = {
      6,
      7,
      8,
      9,
      10,
      11,
      12,
      13
    }; //Die Pins zu den //Segmenten werden festgelegt
    sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
    sevseg.setBrightness(70);
    sevseg.refreshDisplay();

}

void makeEverythingInLoop(){

  
   switch (loopstate) {
    case 0:
      nTtELoop = loopWait + millis();
      //sevseg.refreshDisplay();
      loopstate = 1;

    break;
  case 1:
    if (millis() > nTtELoop) {

      getTime();
      printTime();
      getTemp();
      //sevseg.refreshDisplay();

      loopstate = 0;
    }
  break;
  default:
    break;
  }
}

void getTime() {

  switch (stateTime) {
  case 0:
    
    nTtE = millis() + wait;
    stateTime = 1;
    break;
  case 1:
    if (millis() > nTtE) {

      //Serial.print("Jahr: ");
      jahr = 2000 + Clock.getYear();
      //Serial.print(jahr, DEC);
      //Serial.print("\nMonat: ");

      monat = Clock.getMonth(Century);
      //if (monat < 10)
      //  Serial.print('0');
      //Serial.print(monat, DEC);
      //Serial.print("\nTag: ");

      tag = Clock.getDate();
      //if (tag < 10)
        //Serial.print('0');
      //Serial.print(tag, DEC);
      //Serial.print('\n');
      //Serial.print("Uhrzeit: ");

    //Check Summer Time
      if(stunden>2 && stunden < 3){        
        isSummer = summertime_EU(jahr, monat, tag, Clock.getHour(h12, PM), 1);
      }


      if(isSummer){
        stunden = Clock.getHour(h12, PM) + 1; 
      }else{
        stunden = Clock.getHour(h12, PM);
      }
      //if (stunden < 10)
        //Serial.print('0');
      //Serial.print(stunden, DEC);
      //Serial.print(':');

      minuten = Clock.getMinute();
      //if (minuten < 10)
        //Serial.print('0');
      //Serial.print(minuten, DEC);
      //Serial.print('.');

      sekunden = Clock.getSecond();
    //  if (sekunden < 10)
      //  Serial.print('0');
      //Serial.print(sekunden, DEC);


      //Display on 7 Seg
      //printTime();

      stateTime = 0;
    }

    break;
  default:
    break;
  }

}

unsigned long nTtEPrintEach;
int waitPrint = 2;
int statePrint = 0; 

void printTime() {


  time = (stunden * 100) + minuten;
  sevseg.setNumber(time,2);

  //sevseg.refreshDisplay();


}





// Sommerzeit rechnen
//-------- utility code from jurs ----------
boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month < 3 || month > 10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month > 3 && month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))
    return true;
  else
    return false;
}
