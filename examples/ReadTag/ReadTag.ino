#include "RFID.h"
//#include <RFID.h>
#include <EEPROM.h>
#include <Arduino.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif


RFID myTag; 

void setup() { 
   Serial.begin(2400); 
   
}

void loop() { 
  
  myTag.enableRFIDReader();   //Enable the RFID reader pin  /enable
  myTag.readRFIDTag();        //This function waits and read RFID Tag.
  myTag.printTag();
  myTag.enableRelay();
  myTag.beep();
 
unsigned char status=myTag.isTagValid();
  if(status== VALID_TAG){
      myTag.enableRelay();      //Send a pulse to the relay pin
      myTag.beep();
    }else{
      delay(2000);
    }
   Serial.print("status = ");
   Serial.println(status); 
  
}
