/*---------------------------------
 *  Divergence Meter Nixie Clock
 *  
 *  Nixie tube clock replica of the Divergence Meter from Steins;Gate.
 *  Uses DS3231 to keep time.
 * 
 *  Austin Leo
 *  11/14/2022
 *  Ver 1.0
 ----------------------------------*/

#include <NixieL.h>
#include <Wire.h>

NixieL dMeter;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  dMeter = NixieL();
  //dMeter.setTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void loop() {
  // put your main code here, to run repeatedly:
  dMeter.run();
  delay(1);
}
