/*
 Name:		aLEDp.ino
 Created:	11/21/2020 8:26:28 PM
 Author:	Aria Bentle-Wasmundt
*/

#include "aLEDp_ColorSpecification.h"
#include <FastLED.h>

// HARDWARE
// The LED hardware
#define NUM_LEDS		24
#define DATA_PIN		6
//#define CLOCK_PIN		13
#define LED_TYPE		NEOPIXEL
//#define RGB_ORDER		GRB
//#define SPI_DATA_RATE	DATA_RATE_MHZ(12)

// DEFAULT SETTINGS
// Keep alive in ms
int _keepAliveInterval = 2000;
// Default color
aLEDp_ColorSpecification _defaultColor;
// The current input string buffer
String _input_buffer = "";

// LOOP STORAGE
// Keep alive
unsigned long _lastKeepAlive = 0;

// Arrays to store the LED information
aLEDp_ColorSpecification _LEDs[NUM_LEDS];
CRGB _FastLED[NUM_LEDS];

// INTERNAL DEFINES


// the setup function runs once when you press reset or power the board
void setup() {
	// Initialize FastLED

	FastLED.addLeds<LED_TYPE, DATA_PIN>(_FastLED, NUM_LEDS);
	// Define the default color
	_defaultColor.red(0);
	_defaultColor.green(0);
	_defaultColor.blue(255);
	// Initialize the LEDs
	for (int x = 0; x < NUM_LEDS; x++) {
		set_led_color(x, _defaultColor);
	}
	// Open serial port
	Serial.begin(115200);
	Serial.println("ok aLEDp READY.");
}

// the loop function runs over and over again until power down or reset
void loop() {
	check_for_input();
	check_keep_alive();
}

void check_keep_alive() {
	if (millis() - _lastKeepAlive >= _keepAliveInterval) {
		_lastKeepAlive += _keepAliveInterval;
		Serial.println("ok PING!");
	}
}

void check_for_input() {
	if (Serial.available()) {
		char ch = Serial.read();
		// Parse if a carriage return comes through
		if (ch == '\n') {
			parse_input(_input_buffer);
			_input_buffer = "";
		}
		// Add the input into the buffer
		_input_buffer += (char)ch;
	}
}

void parse_input(String *input) {

}

int set_led_color(
	int color_index, 
	aLEDp_ColorSpecification color
) {
	// Ensure the proper index
	if (color_index < 0 || color_index >= NUM_LEDS) {
		return 0;
	}
	// TODO: Implement proper HSV and RGB controls
	if (color.red() != -1) {
		_FastLED[color_index].red = color.red();
	}
	if (color.green() != -1) {
		_FastLED[color_index].green = color.green();
	}
	if (color.blue() != -1) {
		_FastLED[color_index].blue = color.blue();
	}
	// Since HSV is handled differently, we have to do a full color.
	if (color.hue() != -1 && 
		color.saturation() != -1 && 
		color.brightness() != -1
		) {
			_FastLED[color_index].setHSV(
				color.hue(), 
				color.saturation(), 
				color.brightness()
			);
	}

	FastLED.show();
	// Update the array
	_LEDs[color_index] = color;
	return 1;
}