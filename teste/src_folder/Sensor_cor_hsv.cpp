#include "Sensor_cor_hsv.h"

static double Min(double a, double b) {
	return a <= b ? a : b;
}

static double Max(double a, double b) {
	return a >= b ? a : b;
}

Sensor_cor_hsv::Sensor_cor_hsv(string sensor_port_E, string sensor_port_D) {


}

HSV Sensor_cor_hsv::RGBtoHSV(RGB rgb){
	HSV hsv;
	double delta, min;

		min = Min(Min(rgb.r, rgb.g), rgb.b);
		hsv.v = Max(Max(rgb.r, rgb.g), rgb.b);
		delta = hsv.v - min;

		if (hsv.v == 0.0)
			hsv.s = 0;
		else
			hsv.s = delta / hsv.v;

		if (hsv.s == 0)
			hsv.h = 0.0;

		else
		{
			if (rgb.r == hsv.v)
				hsv.h = (rgb.g - rgb.b) / delta;
			else if (rgb.g == hsv.v)
				hsv.h = 2 + (rgb.b - rgb.r) / delta;
			else if (rgb.b == hsv.v)
				hsv.h = 4 + (rgb.r - rgb.g) / delta;

			hsv.h *= 60;

			if (hsv.h < 0.0)
				hsv.h = hsv.h + 360;
		}

		return hsv;
}
