/* Nixie Tube Clock 


*/


#ifndef NixieL_h
#define NixieL_h

#include "Arduino.h"
#include "Wire.h"

class NixieL
{
	public:
		NixieL();
		// NixieL(int pinValues[10], int pinAnodes[4], int pinAdjust[3]);

		void readTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
		void setTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);		
		void displayTube(int tubeNum, char value);
		void displayTubes(String values);
		void scramble();
		void displayTime();
		void displayDemo();
		void train();
		void start();
	
	private:
		int _pinValues[10] = {2, 3, 4, 5, 14, 	// Pins for Channel 0, 14 for RHDP
							  6, 7, 8, 9, 15};	// Pins for Channel 1, 15 for RHDP					  
		int _pinAnodes[4] = {10, 11, 12, 13};	// Pins for Anode Pairs
		int _pinAdjust[3] = {16, 18, 19};		// Pins 16 for mode, 18 and 19 for Minute and Hour adjustment of RTC	
		
		int _pulseWidth;
		float _dutyCycle;
		String _mode;
		String _displayStr;
};
					   
#endif