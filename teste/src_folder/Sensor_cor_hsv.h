/**
 * H = hue define a cor, 0a 360, cores predominantes de 60 em 60 sendo:
 * 		0 vermelho;
 * 		60 amarelo;
 * 		120 verde;
 * 		180 azul claro;
 * 		240 azul escuro;
 * 		300 rosa;
 * S = Saturacao 0 a 1 o quao desbotado a cor esta
 * V = value 0 a 1 o quao claro esta a cor (0 preto)
 */

#ifndef SENSOR_COR_HSV_H_
#define SENSOR_COR_HSV_H_
#include <tuple>
#include "ev3dev.h"
#include <unistd.h>
#include <math.h>

using namespace std;

enum Cor{ndCor, preto, branco, vermelho, verde, azul, fora}; // cores possiveis

struct RGB{
	int r = 0, g = 0, b = 0;
};

struct HSV{
	double h = 0, s = 0, v = 0;
};

class Sensor_cor_hsv {
public:
	Sensor_cor_hsv(string sensor_port_E, string sensor_port_D);
	Cor ler_cor_E();
	Cor ler_cor_D();
	void calibra();

private:
	HSV RGBtoHSV(RGB rgb);
	ev3dev::color_sensor sensor_E;
	ev3dev::color_sensor sensor_D;
	//TODO sensor cor hsv, testar limites e verificar se eh melhor abrir os limites no maximo
	double limite_H_Vermelho[2] = {30, 330}; // verificar se esta fora
	double limite_H_Verde[2] = {90, 150}; // verificar se esta dentro
	double limite_H_Azul[2] = {120, 270}; // verificar se esta dentro
};

#endif /* SENSOR_COR_HSV_H_ */
