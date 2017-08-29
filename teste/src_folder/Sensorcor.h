/*
 * Sensorcor.h
 *
 *  Created on: 29 de ago de 2017
 *      Author: pcsek
 */

#ifndef SENSORCOR_H_
#define SENSORCOR_H_
#include <iostream>
#include <tuple>
#include "ev3dev.h"
#include <unistd.h>

using namespace std;

enum Cor{nda, preto, branco, verde, azul, vermelho}; // cores possiveis

struct RangesCor{
	int r[2],g[2], b[2];
	static const int range = 10;
};

class Sensor_cor {
public:
	Sensor_cor(); // construtor
	Cor ler_cor(); // por favor ne

private:
	RangesCor r_preto, r_branco, r_verde, r_azul, r_vermelho;
	bool corEstaEntre(std::tuple<int, int, int>, RangesCor);
	void calibra();
	ev3dev::color_sensor;
};

#endif /* SENSORCOR_H_ */
