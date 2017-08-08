/*
 * Motorcontrolado.h
 *
 *  Created on: 7 de ago de 2017
 *      Author: pcsek
 */

#ifndef MOTORCONTROLADO_H_
#define MOTORCONTROLADO_H_
#include <iostream>
#include "ev3dev.h"
#include <time.h>
#include <unistd.h>
#include <thread>

using namespace std;


class Motor_controlado {
public:
	Motor_controlado(string);
	virtual ~Motor_controlado();

	void run_velo_max();
	int stop();
	int run_to();


private:
	ev3dev::large_motor motor;
	double velo_max = 880; // velocidade maxima do motor em graus/s sem carga, testada com a bateria em 7.8v
	double velo_sp = 360;
	double acc_sp = 360;



};

#endif /* MOTORCONTROLADO_H_ */
