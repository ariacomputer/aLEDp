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

int aLEDp_ColorSpecification::red(int value = -1) {
	if (value != -1) {
		_red = constrain(value, 0, 255);
	}
	return _red;
}

int aLEDp_ColorSpecification::green(int value = -1) {
	if (value != -1) {
		_green = constrain(value, 0, 255);
	}
	return _green;
}

int aLEDp_ColorSpecification::blue(int value = -1) {
	if (value != -1) {
		_blue = constrain(value, 0, 255);
	}
	return _blue;
}

int aLEDp_ColorSpecification::hue(int value = -1) {
	if (value != -1) {
		_hue = constrain(value, 0, 255);
	}
	return _hue;
}

int aLEDp_ColorSpecification::saturation(int value = -1) {
	if (value != -1) {
		_saturation = constrain(value, 0, 255);
	}
	return _saturation;
}

int aLEDp_ColorSpecification::brightness(int value = -1) {
	if (value != -1) {
		_brightness = constrain(value, 0, 255);
	}
	return _brightness;
}

