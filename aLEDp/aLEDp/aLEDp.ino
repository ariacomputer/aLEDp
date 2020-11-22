/*
 Name:		aLEDp.ino
 Created:	11/21/2020 8:26:28 PM
 Author:	Aria Bentle-Wasmundt
*/

#include "aLEDp_ColorSpecification.h"

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	Serial.println("ok aLEDp READY.");
	aLEDp_ColorSpecification color;
	Serial.println(color.red());
	Serial.println(color.red(10));
	Serial.println(color.red());
}

// the loop function runs over and over again until power down or reset
void loop() {

}
