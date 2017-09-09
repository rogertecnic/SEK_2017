#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include <thread>
#include <chrono>
#include "ev3dev.h"


#define delay 5
#define aceleracao 0.2


using namespace std;

typedef chrono::high_resolution_clock Time;


enum Direcao{esquerda, direita};
enum flag_aceleracao{nda, frente, tras, parar};


class Controlador_robo {
public:
	Controlador_robo(double);

	void frente(int);
	void tras(int);
	void parar();
	void girar(Direcao);

	bool inicializar_thread_aceleracao();
	bool finalizar_thread_aceleracao();


private:
	/* Objetos da classe Controlador_motor*/
	ev3dev::large_motor *rodaE;
	ev3dev::large_motor *rodaD;

	/*Flag de controle do Switch da thread loop_controle_aceleracao*/
	flag_aceleracao estado = nda;

	/*Variáveis de controle da velocidade de cada roda
	double velo_retardada_me = 0.0;
	double velo_retardada_md = 0.0;
	double velo_sp_me = 0.0;
	double velo_sp_md = 0.0;
	*/

	/*Thread: loop_controle_aceleracao*/
	bool thread_rodando = false;
	thread thread_controle_velocidade;
	void loop_controle_aceleracao();




	/***********************************************************************************************/


	/**/
	double fator_croda;


	/*Variáveis controlador PWM*/
	double erro = 0;
	double erro_anterior = 0;
	int kp = 6;
	int kd = 1;
	int pid = 0;

	int pwm_sp = 0;
	int pwn_retardada = 0;

	/*Leitura de tempo p/ debug*/
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	chrono::duration<double> delta_t; // usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	double tempo = 0;



};



#endif
