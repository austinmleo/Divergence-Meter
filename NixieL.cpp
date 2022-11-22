#include "Arduino.h"
#include "Wire.h"
#include "NixieL.h"

#define DS3231_I2C_ADDRESS 0x68
#define NUMANODES 4
#define NUMVALUES 10
#define NUMADJUST 3

// Convert Decimal to Binary Coded Decimal
byte decToBcd(byte value) {
	return (value/10*16 + value%10);
}

// Convert Binary Coded Decimal to Decimal
byte bcdToDec(byte value) {
	return (value/16*10 + value%16);
}

// Returns a random digit from 0 to 9 as char
char randNumChar() {
	return char(random(48,58));
}


NixieL::NixieL() {
	_pulseWidth = 50;
	_dutyCycle = 0.95;
	
	_mode = "";
	_displayStr = "";
	
	for(int i = 0; i < NUMVALUES; i++) {
		pinMode(_pinValues[i], OUTPUT);
	}
	
	for(int i = 0; i < NUMANODES; i++) {
		pinMode(_pinAnodes[i], OUTPUT);
	}
	
	for(int i = 0; i < NUMADJUST; i++) {
		pinMode(_pinAdjust[i], INPUT);
		digitalWrite(_pinAdjust[i], INPUT_PULLUP);
	}
	
}

/* NixieL::NixieL(int pinValues[], int pinAnodes[], int pinAdjust[])
{
	_pinValues = pinValues;
	
} */


// Read time from RTC
void NixieL::readTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0);
	Wire.endTransmission();
	
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
	*second = bcdToDec(Wire.read() & 0x7f);
	*minute = bcdToDec(Wire.read());
	*hour = bcdToDec(Wire.read() & 0x3f);
	*dayOfWeek = bcdToDec(Wire.read());
	*dayOfMonth = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}

// Set time on RTC
// Parameters are sent through Serial transmission to DS3231 RTC.
void NixieL::setTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0);
	Wire.write(decToBcd(second));
	Wire.write(decToBcd(minute));
	Wire.write(decToBcd(hour));
	Wire.write(decToBcd(dayOfWeek));
	Wire.write(decToBcd(dayOfMonth));
	Wire.write(decToBcd(month));
	Wire.write(decToBcd(year));
	Wire.endTransmission(); 
}

// Display a value on a specified tube
// Tubes are numbered 0 to 7 from left to right
void NixieL::displayTube(int tubeNum, char value) {
	int chan = tubeNum/NUMANODES;
	
	switch (value) {
		case '0':
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '1':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '2':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '3':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '4':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '5':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '6':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;		                                        
		case '7':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '8':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW); 
			break;                                              
		case '9':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '.':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], HIGH);
			break;
		default:
			digitalWrite(_pinValues[chan*(NUMVALUES/2)], HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
	}	
}

void NixieL::displayTubes(String values)
{
	for(int i = 0; i < NUMANODES; i++){
		displayTube(i, _displayStr[i]);
		displayTube(i + NUMANODES, _displayStr[i + NUMANODES]);
		digitalWrite(_pinAnodes[i], HIGH);
		delay(long(_pulseWidth * _dutyCycle));
		
		displayTube(i, ' ');
		displayTube(i + NUMANODES, ' ');
		digitalWrite(_pinAnodes[i], LOW);
		delay(long(_pulseWidth * (1.0 - _dutyCycle)));
	}
}


void NixieL::displayTime()
{	
	while(_mode == "time") {
		byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
		readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
		
		if(analogRead(_pinAdjust[1] > 1000)) {
			minute += 1;
			setTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
		} else if(analogRead(_pinAdjust[2] > 1000)) {
			hour += 1;
			setTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
		}
		
		if(digitalRead(_pinAdjust[0]) == HIGH) {
			_mode = "demo";
			break;
		}
		
		_displayStr = String(int(hour)) + '.' + String(int(minute)) + '.' + String(int(second));
		displayTubes(_displayStr);
	}
}

void NixieL::displayDemo()
{	
	double temp = _dutyCycle;
	_displayStr = String(random(2)) + '.';
	
	while(_displayStr.length() < NUMANODES*2) {
		_displayStr += randNumChar();
	}
	
	for(int i = 0; i < 20; i++) {
		if(random(3) == 0) {
			_dutyCycle = 0.75;
		} else {
			_dutyCycle = temp;
		}
	}
	
	_dutyCycle = temp;
	
	while(_mode == "demo") {
		if(analogRead(_pinAdjust[1] > 1000)) {
			break;
		}
		
		if(digitalRead(_pinAdjust[0]) != HIGH) {
			_mode = "time";
			break;
		}
		
		if(random(3000) == 0) {
			break;
		}
		
		displayTubes(_displayStr);
	}
}

void NixieL::scramble()
{
	double temp = _dutyCycle;
	_displayStr = "";
	
	while(_displayStr.length() < NUMANODES * 2) {
		_displayStr += randNumChar();
	}
	
	for(int i = 0; i < 20 ; i++) {
		for(int j = 0; j < _displayStr.length(); j++) {
			if(random(2) == 0) {
				_displayStr.setCharAt(j, randNumChar());
			}
		}
		
		//_dutyCycle = temp - sin(PI*j/20)*0.25;
		
		if(random(2) == 0) {
			_dutyCycle = 0.75;
		} else {
			_dutyCycle = temp;
		}
			
		displayTubes(_displayStr);
	}
	
	_dutyCycle = temp;
}

void NixieL::train() {
	
}