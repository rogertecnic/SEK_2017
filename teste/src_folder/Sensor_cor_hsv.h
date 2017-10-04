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
#include "M_arquivos.h"
#include "Const.h"

using namespace std;

class Sensor_cor_hsv {
public:
	Sensor_cor_hsv(string sensor_port_E, string sensor_port_D, bool debug, string nome_arquivo);
	Cor ler_cor_E();
	Cor ler_cor_D();
	ev3dev::color_sensor *get_sensor_E();
	ev3dev::color_sensor *get_sensor_D();
	void set_fator_escalimetro_rgb(double *fator_E, double *fator_D);
	void fecha_arquivo();
private:
	bool debug = false;
	M_arquivos *arquivo_E;
	M_arquivos *arquivo_D;

	HSV RGBtoHSV(RGB rgb);

	ev3dev::color_sensor sensor_E;
	ev3dev::color_sensor sensor_D;


	/*
	 * pensar na possibilidade de criar um struct com estes limites e colocar na const.h
	 */
	double limite_H_Vermelho_E[2] = {30, 330}; // verificar se esta fora
	double limite_H_Verde_E[2] = {90, 150}; // verificar se esta dentro
	double limite_H_Azul_E[2] = {120, 270}; // verificar se esta dentro

	double limite_H_Vermelho_D[2] = {30, 330}; // verificar se esta fora
	double limite_H_Verde_D[2] = {90, 150}; // verificar se esta dentro
	double limite_H_Azul_D[2] = {120, 270}; // verificar se esta dentro

	double fator_escalimetro_rgb_E[3] = {1,1,1}; // mudar o rgb para a escala 0 a 255
	double fator_escalimetro_rgb_D[3] = {1,1,1}; // mudar o rgb para a escala 0 a 255
};

#endif /* SENSOR_COR_HSV_H_ */
