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
#define PARSE_STATE_READ_PREFIX		0
#define PARSE_STATE_READ_VALUE		1

// DEFAULT SETTINGS
// Keep alive in ms
int _keepAliveInterval = 2000;
// Default color
aLEDp_ColorSpecification _defaultColor;
// Maximum command size
#define INPUT_SIZE 64
// The current input string buffer
int  _input_buffer_pos = 0;
char _input_buffer[INPUT_SIZE];
String _input = "";

// LOOP STORAGE
// Keep alive
unsigned long _lastKeepAlive = 0;

// Arrays to store the LED information
aLEDp_ColorSpecification _LEDs[NUM_LEDS];
CRGB _FastLED[NUM_LEDS];


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
		if (ch == 0x0d || ch == 0x0a || _input_buffer_pos > (INPUT_SIZE - 1)) {
			Serial.print("Parsing: '");
			Serial.print(_input);
			Serial.println("'");
			parse_input(_input);
			_input = "";
		}
		else if (ch >= 0x20) {
			// Add to the buffer
			_input += ch;
		}
	}
}

void parse_input(String &s) {
	// Run a state machine to handle input 
	int position = 0;
	// 0 is the command including number
	// 1-26 are registers A-Z
	int reg_index = -1;
	String value_buffer = "";
	String regs[] = {
		"","","","","","","","","","",
		"","","","","","","","","","",
		"","","","","",""
	};
	String error = "";

	int state = PARSE_STATE_READ_PREFIX;
	for (position = 0; position < s.length(); position++) {
		char ch = s[position];
		switch (state) {
			case PARSE_STATE_READ_PREFIX:
				// Ignore extra spaces
				if (isSpace(ch)) {
					break;
				}
				// Letter prefixs are commands or registers
				if (isAlpha(ch)) { 
					state = PARSE_STATE_READ_VALUE;
					// Translate a-z to 1-26
					reg_index = (int)ch - 96;
					// Set the prefix in the first register for the command
					if (regs[0].equals("")) {
						reg_index = 0;
						// Uppercase
						regs[0] = ch;
						regs[0].toUpperCase();
					}
					// Otherwise, clear the existing value if any exists (for overwriting)
					else {
						regs[reg_index] = "";
					}
					break;
				}
				// Error otherwise
				error = "No prefix";
				break;
			case PARSE_STATE_READ_VALUE:
				// Space means the end of this setting 
				if (isSpace(ch)) {
					// Values must exist
					if (value_buffer.length() == 0) {
						error = "No value found";
						break;
					}
					// Complete the pair
					regs[reg_index].concat(value_buffer);
					// Clear the buffer
					value_buffer = "";
					reg_index = -1;
					state = PARSE_STATE_READ_PREFIX; 
					break;
				}
				// Commands must be numeric
				if (isAlpha(ch) && reg_index == 0) {
					error = "Invalid command";
					break;
				}
				// Add the character to the buffer
				if (isAlphaNumeric(ch)) {
					value_buffer.concat(ch);
					break;
				}
				// Otherwise, something weird got passed
				error = "Invalid character in value";
				break;

		}
		// Break if there's an error
		if (error.length() != 0) {
			Serial.print("?SYNTAX ERROR: ");
			Serial.println(error);
			return;
		}
	} 
	// Wrap up the state machine
	if (state == PARSE_STATE_READ_VALUE) {
		if (value_buffer.length() == 0) {
			Serial.println("?SYNTAX ERROR: No value found");
			return;
		}
		regs[reg_index].concat(value_buffer);
	}
}

int set_led_color(
	int &color_index, 
	aLEDp_ColorSpecification &color
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