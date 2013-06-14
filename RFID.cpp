#include "RFID.h"

#include <EEPROM.h>
#include <Arduino.h>

//****************************************************************
// Function name:          	RFID
// Descriptions:           	
//          Constructor - initialize private variables and defines
//          pinMode out/in.
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
RFID::RFID(void){


	_enablePin =2;	    // Output to RFID  card /ENABLE pin
	_relayPin=7;		// Output to Relay circuit
	_relayDuration=2000; // Relay on time
	_beepPin=11;		// Beep pin
	_beepDuration=1000;	// Beep on time
	_programPin=12;	// Pin to allow programming master tag
	_maxDataRead=0;
	//clearBuffer();

	//Serial.begin(2400);  
    	
	pinMode(_enablePin,OUTPUT); 
    pinMode(_beepPin,OUTPUT);
    pinMode(_relayPin,OUTPUT);
    pinMode(_programPin,INPUT);
}
/*
void RFID::RFID(int enablePin,int relayPin, int beepPin,int programPin){

	_enablePin =enablePin;  // Output to RFID  card /ENABLE pin
	_relayPin=relayPin;	// Output to Relay circuit
	_relayDuration=2000;	// Relay on time
	_beepPin=beepPin;	// Beep pin
	_beepDuration=1000;	// Beep on time
	_programPin=programPin; // Pin to allow programming master tag
	_maxDataRead=0;
	clearBuffer();

	//Serial.begin(2400);  
    	
	pinMode(_enablePin,OUTPUT); 
    pinMode(_beepPin,OUTPUT);
    pinMode(_relayPin,OUTPUT);
    pinMode(_programPin,INPUT);
}
*/
/*
void RFID::~RFID(){
clearBuffer()
}
*/

//****************************************************************
// Function name:          	clearBuffer
// Descriptions:           	
//     This function clears the rx_Buffer. This ensures no residue data
//     remain in the rx_buffer.         
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::clearBuffer(){
    for(int i=0; i < MAX_BUFFER; i++) {
    _rx_Buffer[i] = 0; 
  }
}
 
//****************************************************************
// Function name:          	printTag
// Descriptions:           	
//     This function sends  that tag data to the serial port. 
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::printTag(){
    //Serial.print("rx_Buffer:"); 
    //Serial.print(_maxDataRead); 
    //Serial.println(_rx_Buffer);
    for(int i=0; i <_maxDataRead; i++) {
      Serial.print(_rx_Buffer[i]); 
    } 
}
 
//****************************************************************
// Function name:          	enableRFIDReader
// Descriptions:           	
//     This function enables the RFID card and allows data
//     to be read and transmitted. The RFID card is enable by
//     pulling the RFID card /enable pin LOW (or ground).
//****************************************************************    
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::enableRFIDReader(){
    digitalWrite(_enablePin, LOW);
    digitalWrite(_beepPin, LOW);   
}

//****************************************************************
// Function name:          	disableRFIDReader
// Descriptions:           	
//     This function disables the RFID reader card and prevents data
//     from being read multiple times. The RFID card is disabled
//     by pulling the /enable pin HIGH (or open circuit).          
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::disableRFIDReader(){
    digitalWrite(_enablePin, HIGH);
    digitalWrite(_beepPin, HIGH);
}

//****************************************************************
// Function name:          	readRFIDTag
// Descriptions:           	
//     This function waits and reads the Arduino serial buffer. When  
//     data are detected, the program disables the RFID reader and
//     fetches the data.   It is important to disable the RFID reader as 
//     soon as possible to prevent RFID reader from transmitting multiple 
//     times. This function DOES NOT re-enable the RFID reader. This 
//     allows the programmer the flexibility to do other tasks.          
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::readRFIDTag(){ 
     _maxDataRead=0; 
     int count;
     count=0;
     clearBuffer();                     
     while(Serial.available() <= 0) {} //Waits forever for data.
     disableRFIDReader();              
     while(Serial.available()){        //Reads the Arduino serial buffer. It should have at least 12 bytes stored.
       delay(5);                       //The START byte should be 0x0A and the STOP byte should be 0x0D.
        _rx_Buffer[count]=Serial.read();
       count++;
	   if(count==MAX_BUFFER) break;
        
     }
     _maxDataRead=count;
}

//****************************************************************
// Function name:          	enableRelay
// Descriptions:           	
//     This function activates the RELAY_PIN for xxx seconds. The delay is
//     set for 2 seconds. If more time is needed, increase RELAY_DURATION.         
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::enableRelay(){
     digitalWrite(_relayPin,HIGH);
     delay(_relayDuration);
     digitalWrite(_relayPin,LOW);   
}

//****************************************************************
// Function name:          	beep
// Descriptions:           	    
//     This function activates the BEEP_PIN for xxx seconds. The delay is
//     set for 1 seconds. If more time is needed, increase BEEP_DURATION.         
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::beep(){
     digitalWrite(_beepPin,HIGH);
     delay(_beepDuration);
     digitalWrite(_beepPin,LOW);   
}

//****************************************************************
// Function name:          	isTagValid
// Descriptions:           	
//      This function checks the FIRST byte and LAST bytes of the message. 
//      If the bytes are correct, it returns VALID_TAG otherwise it returns
//      INVALID_TAG.  The tag data should have at least 12 bytes stored.
//      The first byte should be 0x0A and the last byte should be 0x0D.
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          Boolean
//****************************************************************
unsigned char RFID::isTagValid(){
    if ((_rx_Buffer[0]==FIRST_BYTE) && (_rx_Buffer[11]==LAST_BYTE)) 
		return VALID_TAG;
    
    else
		return INVALID_TAG;
 
}
  
//****************************************************************
// Function name:          	programMasterTag
// Descriptions:           	
//     This function sets the master tag and erases all other stored  
//     tags in EEPROM. The master tag is the only tag that can add
//     other tags. Only ten tags can be stored in EEPROM. Over ten  
//     cards are difficult to manage without external program.
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::programMasterTag(){
    enableRFIDReader();   
    readRFIDTag();
    printTag();
    for (int i = 0; i < 12; i++) EEPROM.write(i, _rx_Buffer[i]);
}

//****************************************************************
// Function name:          	setBeepTime
// Descriptions:           	
//          This function changes the default 2-sec beep.
// Input parameters:        pulseTime (0 to 5 seconds)
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::setBeepTime(int pulseTime){
	_beepDuration=pulseTime;
}

//****************************************************************
// Function name:          	setRelayTime
// Descriptions:           	
//          This function changes the default 2-sec relay time.
// Input parameters:        pulseTime (0 to 5 seconds)
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
void RFID::setRelayTime(int pulseTime){
	_relayDuration=pulseTime;
}

//****************************************************************
// Function name:          	xxx
// Descriptions:           	
//          
// Input parameters:        NONE
// Output parameters:       NONE
// Returned value:          NONE
//****************************************************************
