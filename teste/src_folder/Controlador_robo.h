#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include "ev3dev.h"
#include "M_arquivos.h"


using namespace std;


typedef chrono::high_resolution_clock Time;


enum flag_aceleracao{nd, linha_reta, parar, girar};


class Controlador_robo {
public:
	Controlador_robo(double fator_croda, bool debug, string nome_arquivo);

	void andar(int);
	void parar();
	void girar(int angulo_robo_graus); // positivo anti-horario

	bool inicializar_thread_aceleracao();
	bool finalizar_thread_aceleracao();
	flag_aceleracao get_estado();


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
	double aceleracao = 100.0;//Em pwm/seg
	double raio_roda = 0.056; // metros
	double raio_robo = 0.150; // largura entre os centros das rodas div por 2

	/*Variáveis controlador PWM*/
	double erro = 0.0;
	double erro_anterior = 0.0;
	int kp = 6;// 6
	int kd = 1;
	int pid = 0;

	double pwm_sp = 0;//Valor desejado
	double pwm_retardada = 0.0;//Valor que será incrementado
	double pwm = 0;//Valor argumento da função "set_duty_cycle_sp"
	int angulo_robo_graus = 0;

	/*Variaveis para debug*/
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	double tempo = 0;
	bool debug;
	string nome_arquivo;
	M_arquivos *arquivo;
	bool arquivo_aberto = false;
};

#endif
