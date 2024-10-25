#include "Arduino.h"
#include "Wire.h"
#include "NixieL.h"

#define DS3231_I2C_ADDRESS 0x68
#define NUMANODES 4
#define NUMTUBES 8
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
	_pulseWidth = 100;
	_dutyCycle = 1.0;
	
	_mode = "";
	_displayStr = "";
	
	for(int i = 0; i < NUMVALUES; i++) {
		pinMode(_pinValues[i], OUTPUT);
	}
	
	for(int i = 0; i < NUMANODES; i++) {
		pinMode(_pinAnodes[i], OUTPUT);
	}
	
	for(int i = 0; i < NUMADJUST; i++) {
		pinMode(_pinAdjust[i], INPUT_PULLUP);
	}
	
	randomSeed(analogRead(0));
}


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

void NixieL::setPulseWidth(unsigned long microSeconds) {
	if(microSeconds > 0) {
		_pulseWidth = microSeconds;
	}
}

void NixieL::setDutyCycle(double dutyCycle) {
	if(dutyCycle <= 1.0 && dutyCycle > 0) {
		_dutyCycle = dutyCycle;
	}
}

// Display a value on a specified tube
// Tubes are numbered 0 to 7 ordered left to right
void NixieL::displayTube(int tubeNum, char value) {
	int chan = tubeNum/NUMANODES;
	
	switch (value) {
		case '0':
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '1':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '2':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '3':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '4':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '5':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '6':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;		                                        
		case '7':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '8':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   LOW);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW); 
			break;                                              
		case '9':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], LOW);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
			break;                                              
		case '.':                                               
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], HIGH);
			break;
		default:
			digitalWrite(_pinValues[chan*(NUMVALUES/2)],   HIGH);	
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+1], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+2], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+3], HIGH);
			digitalWrite(_pinValues[chan*(NUMVALUES/2)+4], LOW);
	}	
}

// Display the stored value of _displayStr on tubes.
void NixieL::displayTubes() {
	for(int i = 0; i < NUMANODES; i++){
		displayTube(i, _displayStr[i]);
		displayTube(i + NUMANODES, _displayStr[i + NUMANODES]);
		digitalWrite(_pinAnodes[i], HIGH);
		delayMicroseconds(_pulseWidth * _dutyCycle);
		
		digitalWrite(_pinAnodes[i], LOW);
		delayMicroseconds(_pulseWidth * (1.0 - _dutyCycle));
	}
}


// Display random flickering integers
void NixieL::scramble() {
	double base = _dutyCycle;
	
	for(int i = 0; i < 50; i++) {
		_displayStr = "";
		
		while(_displayStr.length() < NUMTUBES) {
			_displayStr += randNumChar();
		}

		if(random(4) == 0) {
			_dutyCycle = 0.2;
		} else {
			_dutyCycle = base;
		}
		for(int j = 0; j < 50; j++){
			displayTubes();
		}
	}
	
	_dutyCycle = base;
}

// Retreive and display time stored on DS3231
void NixieL::displayTime() {
	bool adjrollover = false;
	
	while(_mode == "time") {
		byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
		readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
		
		if(digitalRead(_pinAdjust[0]) == LOW) {
			break;
		}
		
		// Change time if adjust buttons are activated
		// Note: analogRead() is used because the Pro Mini pins A6 and A7 cannot(?) digitalRead()
		if(analogRead(_pinAdjust[1]) < 1000) {
			minute += 1;
			
			if(minute >= 60) {
				adjrollover = true;
				minute = 0;
			}
			
			setTime(0, minute, hour, dayOfWeek, dayOfMonth, month, year);
			delay(500);
			
		} else if(analogRead(_pinAdjust[2]) < 1000) {
			hour += 1;
			
			if(hour >= 24) {
				hour = 0;
			}
			
			setTime(0, minute, hour, dayOfWeek, dayOfMonth, month, year);
			delay(500);
		}
		
		String h = String(hour / 10) + String(hour % 10);
		String m = String(minute / 10) + String(minute % 10);
		String s = String(second / 10) + String(second % 10);
		
		if(m == "00" && s == "00") {
			if(adjrollover == false) {
				break;
			} 
		} else {
			adjrollover = false;
		}
			
		_displayStr = h + '.' + m + '.' + s;
		
		for(int i = 0; i < 50; i++) {
			displayTubes();
		}
	}
}

// Display a randomly generated 'World Line.'
void NixieL::displayDemo() {	
	double base = _dutyCycle;
	_displayStr = String(random(2)) + '.';
	
	while(_displayStr.length() < NUMTUBES) {
		_displayStr += randNumChar();
	}
	
	for(int i = 0; i < 80; i++) {
		int flicker = random(4);
		if(flicker == 0) {
			_dutyCycle = 0.6;
		} else if (flicker == 1) {
			_dutyCycle = 0.8;
		} else {
			_dutyCycle = base;
		}
		
		for(int j = 0; j < 30; j++) {
			displayTubes();
		}
	}
	_dutyCycle = base;
	
	unsigned long counter = 0;
	unsigned long target = random(6000000) + random(6000000);
	
	while(_mode == "demo") {
		if(digitalRead(_pinAdjust[0]) == HIGH) {
			break;
		}
		
		if(analogRead(_pinAdjust[1]) < 1000 || analogRead(_pinAdjust[2]) < 1000) {
			break;
		}
		
		if(counter >= target) {
			break;
		} else {
			counter += 1;
		}
		
		for(int i = 0; i < 50; i++) {
			displayTubes();
		}
	}
}



void NixieL::run() {
	scramble();
	
	if(digitalRead(_pinAdjust[0]) == HIGH) {
		_mode = "time";
	} else {
		_mode = "demo";
	}
	
	if(_mode == "demo") {
		displayDemo();
	} else if(_mode == "time") {
		displayTime();
	}
}