#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include <thread>
#include "ev3dev.h"
#include "Controlador_motor.h"


#define delay 5
#define aceleracao 0.2


using namespace std;


enum Direcao{esquerda, direita};


class Controlador_robo {
public:
	Controlador_robo();

	void inicializar_threads_motor();
	void finalizar_threads_motor();

	void frente(double);
	void tras(double);
	void parar();
	void girar(Direcao);

	bool inicializar_thread_aceleracao();
	bool finalizar_thread_aceleracao();


private:
	/* Objetos da classe Controlador_motor*/
	Controlador_motor *rodaE;
	Controlador_motor *rodaD;

	/*Flag de controle do Switch da thread loop_controle_aceleracao*/
	int flag_aceleracao = 0;

	/*Variáveis de controle da velocidade de cada roda*/
	double velo_retardada_me = 0.0;
	double velo_retardada_md = 0.0;
	double velo_sp_me = 0.0;
	double velo_sp_md = 0.0;

	/*Thread: loop_controle_aceleracao*/
	bool thread_rodando = false;
	thread thread_controle_velocidade;
	void loop_controle_aceleracao();
};



#endif
