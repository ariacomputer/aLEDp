// aLEDp_ColorSpecification.h

#ifndef _ALEDP_COLORSPECIFICATION_h
#define _ALEDP_COLORSPECIFICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class aLEDp_ColorSpecification {
public:
	aLEDp_ColorSpecification();
	int red(int value);
	int green(int value);
	int blue(int value);
	int hue(int value);
	int saturation(int value);
	int brightness(int value);
	// TODO: Specify an HTML color code
private:
	int _red;
	int _green;
	int _blue;
	int _hue;
	int _saturation;
	int _brightness;
};

#endif

