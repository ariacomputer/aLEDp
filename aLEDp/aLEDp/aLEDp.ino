/*
 Name:		aLEDp.ino
 Created:	11/21/2020 8:26:28 PM
 Author:	Aria Bentle-Wasmundt
*/

#include "aLEDp_ColorSpecification.h"

const int keepAliveInterval = 2000;
unsigned long lastKeepAlive = 0;
aLEDp_ColorSpecification defaultColor;

// the setup function runs once when you press reset or power the board
void setup() {
	// Define the default color
	defaultColor.red(0);
	defaultColor.green(0);
	defaultColor.blue(0);
	// Open serial port
	Serial.begin(115200);
	Serial.println("ok aLEDp READY.");
}

// the loop function runs over and over again until power down or reset
void loop() {
	check_keep_alive();
}

void check_keep_alive() {
	if (millis() - lastKeepAlive >= keepAliveInterval) {
		lastKeepAlive += keepAliveInterval;
		Serial.println("ok PING!");
	}
}