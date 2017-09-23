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

struct LimitesCor{
	int r[3] = {0,0,0},g[3] = {0,0,0}, b[3] = {0,0,0};
};

namespace RangesCor{
	static constexpr  int range_preto[3] = {9,9,9};
	static constexpr  int range_branco[3] = {35,37,35};
	static constexpr  int range_vermelho[3] = {43,26,10};
	static constexpr  int range_verde[3] = {50,75,39};
	static constexpr  int range_azul[3] = {35,43,31};
	static constexpr  int range_fora[3] = {6,6,6};
};

class Sensor_cor {
public:
	Sensor_cor(string sensor_port_E, string sensor_port_D); // construtor
	Cor ler_cor_E(); // por favor ne
	Cor ler_cor_D(); // por favor ne
	void calibra();
	void define_ranges();
	void printa_rgb_E();

private:
	void calibra_cor(const int *range_cor, LimitesCor *limites);
	void coleta_cor(LimitesCor *limites, ev3dev::color_sensor sensor);
	LimitesCor range_cor;

	LimitesCor limites_Preto_E, limites_Branco_E, limites_Vermelho_E,
	limites_Verde_E, limites_Azul_E, limites_Fora_E;

	LimitesCor limites_Preto_D, limites_Branco_D, limites_Vermelho_D,
	limites_Verde_D, limites_Azul_D, limites_Fora_D;

	bool corEstaEntre(std::tuple<int, int, int>, LimitesCor);
	int vezes_calibrou = 4;
	ev3dev::color_sensor sensor_E;
	ev3dev::color_sensor sensor_D;
};

#endif /* SENSOR_COR_H_ */
