/*
 * Sensorcor.h
 *
 *  Created on: 29 de ago de 2017
 *      Author: pcsek
 */

#ifndef SENSOR_COR_H_
#define SENSOR_COR_H_
#include <iostream>
#include <tuple>
#include "ev3dev.h"
#include <unistd.h>

using namespace std;

enum Cor{nda, preto, branco, vermelho, verde, azul, fora}; // cores possiveis

struct RangesCor{
	int r[2] = {0,0},g[2] = {0,0}, b[2] = {0,0};
	static const int range_preto = 8; // medido com o metodo de verificar ranges
	static const int range_branco = 35; // medido com o metodo de verificar ranges
	static const int range_vermelho = 30;
	static const int range_verde = 40;
	static const int range_azul = 20; // medido com o metodo de verificar ranges
	static const int range_fora = 20;
};

class Sensor_cor {
public:
	Sensor_cor(string sensor_port_E, string sensor_port_D); // construtor
	Cor ler_cor_E(); // por favor ne
	Cor ler_cor_D(); // por favor ne
	void calibra();
	void define_ranges();

private:
	void calibra_cor(int range_cor, RangesCor *limites, ev3dev::color_sensor sensor);

	RangesCor limites_Preto_E, limites_Branco_E, limites_Vermelho_E,
	limites_Verde_E, limites_Azul_E, limites_Fora_E;

	RangesCor limites_Preto_D, limites_Branco_D, limites_Vermelho_D,
	limites_Verde_D, limites_Azul_D, limites_Fora_D;

	bool corEstaEntre(std::tuple<int, int, int>, RangesCor);

	ev3dev::color_sensor sensor_E;
	ev3dev::color_sensor sensor_D;
};

#endif /* SENSOR_COR_H_ */
