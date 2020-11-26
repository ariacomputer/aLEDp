// 
// 
// 

#include "Arduino.h"
#include "aLEDp_ColorSpecification.h"

aLEDp_ColorSpecification::aLEDp_ColorSpecification() {
	// TODO: Allow an HTML code to be specified.
	_red = -1;
	_green = -1;
	_blue = -1;
	_hue = -1;
	_saturation = -1;
	_brightness = -1;
}

int aLEDp_ColorSpecification::red() {
	return _red;
}

void aLEDp_ColorSpecification::red(int value) {
	_red = constrain(value, -1, 255);
}

int aLEDp_ColorSpecification::green() {
	return _green;
}

void aLEDp_ColorSpecification::green(int value) {
	_green = constrain(value, -1, 255);
}

int aLEDp_ColorSpecification::blue() {
	return _blue;
}

void aLEDp_ColorSpecification::blue(int value) {
	_blue = constrain(value, -1, 255);
}

int aLEDp_ColorSpecification::hue() {
	return _hue;
}

void aLEDp_ColorSpecification::hue(int value) {
	_hue = constrain(value, -1, 255);
}

int aLEDp_ColorSpecification::saturation() {
	return _saturation;
}

void aLEDp_ColorSpecification::saturation(int value) {
	_saturation = constrain(value, -1, 255);
}

int aLEDp_ColorSpecification::brightness() {
	return _brightness;
}

void aLEDp_ColorSpecification::brightness(int value) {
	_brightness = constrain(value, -1, 255);
}
