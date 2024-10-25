/*-----------------------	
 *	Nixie Tube Clock Library
  *
 *	Austin Leo
 *	11/14/2022
 -------------------------*/


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
		void setPulseWidth(unsigned long microSeconds);	
		void setDutyCycle(double dutyCycle);	
		void displayTube(int tubeNum, char value);
		void displayTubes();
		void scramble();
		void displayTime();
		void displayDemo();
		void run();
		
	private:
		uint8_t _pinValues[10] = {2, 3, 4, 5, A3, 	// Pins for Channel 0, A3 for RHDP
					6, 7, 8, 9, A2};		// Pins for Channel 1, A2 for RHDP					  
		uint8_t _pinAnodes[4] = {10, 11, 12, 13};	// Pins for Anode Pairs
		uint8_t _pinAdjust[3] = {A1, A6, A7};		// Pins 15 for mode, A6 and A7 for Minute and Hour 
													// adjustment of RTC and scramble
		unsigned long _pulseWidth;
		double _dutyCycle;
		String _mode;
		String _displayStr;
};
					   
#endif