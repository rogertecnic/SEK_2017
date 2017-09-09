#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "ev3dev.h"


using namespace std;


typedef chrono::high_resolution_clock Time;


enum Direcao{esquerda, direita};
enum flag_aceleracao{linha_reta, parar};


class Controlador_robo {
public:
	Controlador_robo(double);

	void andar(int);
	void parar();
	void girar(Direcao);

	bool inicializar_thread_aceleracao();
	bool finalizar_thread_aceleracao();


private:
	/*Roda esquerda e Roda direita do robô*/
	ev3dev::large_motor *rodaE;
	ev3dev::large_motor *rodaD;


	/*Flag de controle do Switch da thread loop_controle_aceleracao*/
	flag_aceleracao estado = flag_aceleracao::parar;


	/*Thread: loop_controle_aceleracao*/
	bool thread_rodando = false;
	thread thread_controle_velocidade;
	void loop_controle_aceleracao();


	/*Variaveis caracteristica do robo*/
	double fator_croda;
	double delay = 5.0;//Em miliseg
	double aceleracao = 60.0;//Em pwm/seg


	/*Variáveis controlador PWM*/
	double erro = 0.0;
	double erro_anterior = 0.0;
	int kp = 5;// 6
	int kd = 1;
	int pid = 0;

	int pwm_sp = 0;//Valor desejado
	double pwm_retardada = 0.0;//Valor que será incrementado
	int pwm = 0;//Valor argumento da função "set_duty_cycle_sp"

	/*Leitura de tempo p/ debug*/
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	double tempo = 0;


};

#endif
