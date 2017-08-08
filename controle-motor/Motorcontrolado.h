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
	enum Sentido{frente, traz};
public:
	Motor_controlado(string);

	void run_controle();
	int parar();
	int girar_indefinido(Sentido);
	int girar_por(int);


private:

	ev3dev::large_motor motor;
	double velo_max = 880; // velocidade maxima do motor em graus/s sem carga, testada com a bateria em 7.8v
	double velo_sp = 360;
	double acc_sp = 360;
	bool parado = true; // true se o moro estiver parado/parando
	Sentido sentido_giro = frente;
	thread thread_pid;

	void thread_girar_indefinido(Sentido);

};

#endif /* MOTORCONTROLADO_H_ */
