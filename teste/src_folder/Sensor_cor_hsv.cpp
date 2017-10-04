#include "Sensor_cor_hsv.h"

static double Min(double a, double b) {
	return a <= b ? a : b;
}

static double Max(double a, double b) {
	return a >= b ? a : b;
}

Sensor_cor_hsv::Sensor_cor_hsv(string sensor_port_E, string sensor_port_D,
		bool debug = false, string nome_arquivo = "")
:sensor_E(ev3dev::color_sensor(sensor_port_E)),
 sensor_D(ev3dev::color_sensor(sensor_port_D)),
 debug(debug)
{
	if(debug){
		string s = "_E.m";
		s = nome_arquivo + s;
		arquivo_E = new M_arquivos(s, 7); // tempo, 3rgb, 3hsv
		s = "_D.m";
		s = nome_arquivo + s;
		arquivo_D = new M_arquivos(s, 7); // tempo, 3rgb, 3hsv
	}

}


Cor Sensor_cor_hsv::ler_cor_E() {
	std::tuple<int, int, int> sample;
	sample = sensor_E.raw();
	RGB rgb;
	rgb.r = get<0>(sample);
	rgb.g = get<1>(sample);
	rgb.b = get<2>(sample);
	rgb.r *= fator_escalimetro_rgb_E[0];
	rgb.g *= fator_escalimetro_rgb_E[1];
	rgb.b *= fator_escalimetro_rgb_E[2];

	HSV hsv = RGBtoHSV(rgb);

	if(debug) arquivo_E->elementos_arq((double)0,
			(double)rgb.r, (double)rgb.g, (double)rgb.b,
			(double)hsv.h, (double)hsv.s, (double)hsv.v);

	//TODO fazer condicao quando for preto branco ou fora esquerdo
	if(hsv.h < limite_H_Vermelho_E[0] || hsv.h > limite_H_Vermelho_E[1]) return Cor::vermelho;
	if(hsv.h > limite_H_Verde_E[0] && hsv.h < limite_H_Verde_E[1]) return Cor::verde;
	if(hsv.h > limite_H_Azul_E[0] && hsv.h < limite_H_Azul_E[1]) return Cor::azul;

	return Cor::ndCor;
}


Cor Sensor_cor_hsv::ler_cor_D() {
	std::tuple<int, int, int> sample;
	sample = sensor_D.raw();
	RGB rgb;
	rgb.r = get<0>(sample);
	rgb.g = get<1>(sample);
	rgb.b = get<2>(sample);
	rgb.r *= fator_escalimetro_rgb_D[0];
	rgb.g *= fator_escalimetro_rgb_D[1];
	rgb.b *= fator_escalimetro_rgb_D[2];

	HSV hsv = RGBtoHSV(rgb);

	if(debug) arquivo_D->elementos_arq((double)0,
			(double)rgb.r, (double)rgb.g, (double)rgb.b,
			(double)hsv.h, (double)hsv.s, (double)hsv.v);

	//TODO fazer condicao quando for preto branco ou fora direito
	if(hsv.h < limite_H_Vermelho_D[0] || hsv.h > limite_H_Vermelho_D[1]) return Cor::vermelho;
	if(hsv.h > limite_H_Verde_D[0] && hsv.h < limite_H_Verde_D[1]) return Cor::verde;
	if(hsv.h > limite_H_Azul_D[0] && hsv.h < limite_H_Azul_D[1]) return Cor::azul;

	return Cor::ndCor;
}

void Sensor_cor_hsv::set_fator_escalimetro_rgb(double *fator_E, double *fator_D) {
	fator_escalimetro_rgb_E[0] = fator_E[0];
	fator_escalimetro_rgb_E[1] = fator_E[1];
	fator_escalimetro_rgb_E[2] = fator_E[2];

	fator_escalimetro_rgb_D[0] = fator_D[0];
	fator_escalimetro_rgb_D[1] = fator_D[1];
	fator_escalimetro_rgb_D[2] = fator_D[2];
	if(debug)
		cout << "fatores_r:" << endl
		<< fator_escalimetro_rgb_E[0] << ";" << fator_escalimetro_rgb_D[0] << endl;
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
	hsv.v = hsv.v/255;
	return hsv;
}

void Sensor_cor_hsv::fecha_arquivo(){
	if(debug){
		arquivo_E->fecha_arq();
		arquivo_D->fecha_arq();
	}
	debug = false;
}

ev3dev::color_sensor *Sensor_cor_hsv::get_sensor_E() {
	return &sensor_E;
}

ev3dev::color_sensor *Sensor_cor_hsv::get_sensor_D() {
	return &sensor_D;
}
