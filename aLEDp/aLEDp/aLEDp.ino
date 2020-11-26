/*
 Name:		aLEDp.ino
 Created:	11/21/2020 8:26:28 PM
 Author:	Aria Bentle-Wasmundt
*/

#include "aLEDp_ColorSpecification.h"
// Added to disable the FastLED version number "pragma warning"
#define FASTLED_INTERNAL
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
#define INPUT_SIZE		64
#define REGISTERS_SIZE	26

#define SYNTAX_ERROR 1
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
	_defaultColor.blue(0);
	// Initialize the LEDs
	for (int x = 0; x < NUM_LEDS; x++) {
		set_led_color(x, _defaultColor);
	}
	// Open serial port
#ifndef NDEBUG
	Serial.begin(115200);
	Serial.println("ok aLEDp READY.");
#endif
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
		if (	ch == 0x0d || 
				ch == 0x0a ||
				_input_buffer_pos > (INPUT_SIZE - 1)
			) {
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
		"","","","","","",""
	};
	String error = "";

	int state = PARSE_STATE_READ_PREFIX;
	for (position = 0; position < s.length(); position++) {
		char ch = s[position];
		// Change lowercase letters to uppercase
		if (islower(ch)) {
			ch -= 32;
		}

		// Engage the State Machine
		switch (state) {
			case PARSE_STATE_READ_PREFIX:
				// Ignore extra spaces
				if (isSpace(ch)) {
					break;
				}
				// Letter prefixs are commands or registers
				if (isAlpha(ch)) { 
					state = PARSE_STATE_READ_VALUE;
					// Translate A-Z to 1-26
					reg_index = get_register_index(ch);
					// The first register submitted is the command
					// at register 0
					if (regs[0].equals("")) {
						reg_index = 0;
						regs[0] = ch;
					}
					// Otherwise, clear the existing value if any exists
					// (for overwriting)
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
			error_report(SYNTAX_ERROR, error);
			return;
		}
	} 
	// Wrap up the state machine
	if (state == PARSE_STATE_READ_VALUE) {
		if (value_buffer.length() == 0) {
			error_report(SYNTAX_ERROR, "No value found");
			return;
		}
		regs[reg_index].concat(value_buffer);
	}

	run_command(regs);
}

void run_command(String registers[]) {
	// Register 0 is the command
	// TODO: Find a char way
#ifndef NDEBUG
	Serial.println(registers[0].substring(0, 1));
#endif
	char prefix = registers[0].charAt(0);
	// Ensure the prefix is uppercase
	if (islower(prefix)) {
		prefix -= 32;
	}
	int suffix = registers[0].substring(1).toInt();
#ifndef NDEBUG
	Serial.print("Pre:");
	Serial.println(prefix);
	Serial.print("Suf:");
	Serial.println(suffix);
#endif

	// Split into the prefix and the number
	switch (prefix) {
		case 'A':
			switch (suffix) {
				// A1: Set Color
				case 1:
#ifndef NDEBUG
					Serial.println("Setting color!");
#endif
					aLEDp_ColorSpecification color;
					int firstIndex = 0;
					int lastIndex = NUM_LEDS - 1;
					// Red
					if (registers[get_register_index('R')].length() > 0) {
#ifndef NDEBUG
						Serial.print("Setting red to:");
						Serial.println(registers[
							get_register_index('R')
						].toInt());
#endif
						color.red(registers[
							get_register_index('R')
						].toInt());
					}
					// Green
					if (registers[get_register_index('G')].length() > 0) {
#ifndef NDEBUG
						Serial.print("Setting green to:");
						Serial.println(registers[
							get_register_index('G')
						].toInt());
#endif
						color.green(registers[
							get_register_index('G')
						].toInt());
					}
					// Blue
					if (registers[get_register_index('B')].length() > 0) {
#ifndef NDEBUG
						Serial.print("Setting blue to:");
						Serial.println(registers[
							get_register_index('B')
						].toInt());
#endif
						color.blue(registers[
							get_register_index('B')
						].toInt());
					}
					// Hue
					if (registers[get_register_index('H')].length() > 0) {
#ifndef NDEBUG
						Serial.print("Setting hue to:");
						Serial.println(registers[
							get_register_index('H')
						].toInt());
#endif
						color.hue(registers[get_register_index('H')].toInt());
					}
					// Saturation
					if (registers[get_register_index('S')].length() > 0) {
#ifndef NDEBUG
						Serial.print("Setting saturation to:");
						Serial.println(registers[
							get_register_index('S')
						].toInt());
#endif
						color.saturation(registers[
							get_register_index('S')
						].toInt());
					}
					// Brightness
#ifndef NDEBUG
					Serial.print("Setting brightness to:");
					Serial.println(registers[
						get_register_index('V')
					].toInt());
#endif
					if (registers[get_register_index('V')].length() > 0) {
						color.brightness(registers[
							get_register_index('V')
						].toInt());
					}
					// First LED index
					if (registers[get_register_index('F')].length() > 0) {
						firstIndex = registers[
							get_register_index('F')
						].toInt();
						if (firstIndex < 0) {
							error_report(SYNTAX_ERROR, 
								"First LED must be >= 0");
							break;
						}
						// Since we're doing this first, if it's defined,
						// first assume that it's only specifying one LED.
						lastIndex = firstIndex;
					}
					// Last LED index
					if (registers[get_register_index('L')].length() > 0) {
						lastIndex = registers[
							get_register_index('L')
						].toInt();
						if (lastIndex >= NUM_LEDS) {
							String err = "Last LED must be <";
							err.concat(NUM_LEDS);
							error_report(SYNTAX_ERROR, err);
							break;
						}
						if (lastIndex < firstIndex) {
							error_report(SYNTAX_ERROR, 
								"Last LED must be before first LED");
						}
					}

					// Light it up!
					for (int l = firstIndex; l <= lastIndex; l++) {
						set_led_color(l, color);
					}

					break;
				default: 
					error_report(SYNTAX_ERROR, "Command does not exist");
					break;
			}
			break;
		default:
			error_report(SYNTAX_ERROR, "Command type not supported");
			break;
	}
}

// Helper function to translate a-z to 1-26
int get_register_index(char s) {
	// Trying to limit the number of erroneous register calls on bad ASCII
	if (!(isAlpha(s))) {
		return -1;
	}
	// Change to upper case
	if (islower(s)) {
		s -= 32;
	}
	return int(s) - 64;
}

void error_report(int type, String description) {
	String error_message = "";
	switch (type) {
		case SYNTAX_ERROR:
			Serial.print("?SYNTAX  ERROR");
			break;
		default:
			Serial.print("?UNKNOWN ERROR");
			break;
	}
	Serial.print(error_message);
	Serial.print(": ");
	Serial.println(description);
}

void ok_report(String description = "") {
	Serial.print("ok:");
	Serial.println(description);
}

int set_led_color(
	int led_index, 
	aLEDp_ColorSpecification color
) {
	// Ensure the proper index
	if (led_index < 0 || led_index >= NUM_LEDS) {
		return 0;
	}
	// TODO: Implement proper HSV and RGB controls
	if (color.red() != -1) {
#ifndef NDEBUG
		Serial.print("Changing red to ");
		Serial.println(color.red());
#endif
		_FastLED[led_index].red = color.red();
	}
	if (color.green() != -1) {
#ifndef NDEBUG
		Serial.print("Changing green to ");
		Serial.println(color.green());
#endif
		_FastLED[led_index].green = color.green();
	}
	if (color.blue() != -1) {
#ifndef NDEBUG
		Serial.print("Changing blue to ");
		Serial.println(color.blue());
#endif
		_FastLED[led_index].blue = color.blue();
	}
	// Since HSV is handled differently, we have to do a full color.
	if (color.hue() != -1 && 
		color.saturation() != -1 && 
		color.brightness() != -1
		) {
			_FastLED[led_index].setHSV(
				color.hue(), 
				color.saturation(), 
				color.brightness()
			);
	}

	FastLED.show();
	// Update the array
	_LEDs[led_index] = color;
	return 1;
}