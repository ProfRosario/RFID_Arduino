#ifndef _PARALLAX_RFID_H
#define _PARALLAX_RFID_H

//#include "Arduino.h"


#include <EEPROM.h>

#define VALID_TAG (0)
#define INVALID_TAG (1)
#define FIRST_BYTE 0x0A
#define LAST_BYTE 0x0D 
#define MAX_TAG_SIZE 12
#define MAX_DATA_SIZE 36
#define MAX_BUFFER 50 // was 200 before

class RFID
{
  public:
	RFID(void);
	//RFID(int enablePin,int relayPin, int beepPin,int programPin);
	//~RFID();
    void clearBuffer();
	void printTag();
	void enableRFIDReader();
	void disableRFIDReader();
	void readRFIDTag();
	void enableRelay();
	void beep();
	unsigned char isTagValid();
	void programMasterTag();
	void addNewTag();
	void setBeepTime(int pulseTime);
	void setRelayTime(int pulseTime);
  private:
	int _enablePin;		// Output to RFID card /ENABLE pin
	int _relayPin;		// Output to Relay circuit
	int _relayDuration;	// Relay pulse ON time
	int _beepPin;		// Beep pin
	int _beepDuration;	// Beep pulse ON time
	int _programPin;	// Pin to allow programming master tag
	char _rx_Buffer[MAX_BUFFER];
	int _maxDataRead;
};

#endif