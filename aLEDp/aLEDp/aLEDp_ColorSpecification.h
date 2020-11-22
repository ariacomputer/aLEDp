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
	int red();
	void red(int value);
	int green();
	void green(int value);
	int blue();
	void blue(int value);
	int hue();
	void hue(int value);
	int saturation();
	void saturation(int value);
	int brightness();
	void brightness(int value);
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

