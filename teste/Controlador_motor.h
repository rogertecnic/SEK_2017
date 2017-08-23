#ifndef CONTROLADOR_MOTOR_H_
#define CONTROLADOR_MOTOR_H_
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "ev3dev.h"
using namespace std;
typedef std::chrono::high_resolution_clock Time;

class Controlador_motor {
public:
	Controlador_motor(string motor_port, double raio, double velo_max, double aceleracao, bool debug = false); // construtor
	void tora_o_pau();  //gira pro infinito e alem com pwm = 100
	void set_velo(double velocidade);
	void set_acc(double acc);

private:

	//****************VARIAVEIS DO MOTOR********************
	//******************************************************
	//******************************************************
	double raio = 0.04, // metro 0.0408 padrao, 0.04 para o juvenal
	delta_S_roda, // metros rodados
	velo_sp, // m/s
	velo_retardada = 0,
	velo_max, // em graus/seg
	aceleracao; // m/s²



	//***************VARIAVEIS DO CONTROLADOR***************
	//******************************************************
	//******************************************************
	int delay;
	double erro = 0,
			pos_inicial = 0,
			pos_final = 0, // acumulativo
			velo_inicial = 0,
			velo_final = 0,
			acumulador = 0,
			kp = 3,
			ki = 0.05,
			kd = 0.02,
			pwm = 0; // -100 ate 100
	chrono::system_clock::time_point t_inicial = Time::now();
	chrono::system_clock::time_point t_final;
	std::chrono::duration<double> delta_t; // usar dt = t1-t2 e dt.count() para pegar o tempo em seg

	//*****************VARIAVEIS SISTEMA********************
	//******************************************************
	//******************************************************
	ofstream file;
	bool debug;
	thread thread_controle; // objeto da thread
	void loop_controlador();
	ev3dev::large_motor roda;
};

#endif /* CONTROLADOR_MOTOR_H_ */
