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

enum Cor{nda, preto, branco, vermelho, verde, azul}; // cores possiveis

struct RangesCor{
	int r[2],g[2], b[2];
	static const int range =40;
	static const int range_preto = 10;
	static const int range_branco = 85;
};

class Sensor_cor {
public:
	Sensor_cor(string sensor_port); // construtor
	Cor ler_cor(); // por favor ne
	void calibra();

private:
	RangesCor r_Preto, r_Branco, r_Vermelho, r_Verde, r_Azul;
	bool corEstaEntre(std::tuple<int, int, int>, RangesCor);

	ev3dev::color_sensor sensor;
};

#endif /* SENSOR_COR_H_ */
