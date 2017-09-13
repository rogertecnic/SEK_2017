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
	Controlador_robo(bool debug, string nome_arquivo);

	void andar(int);
	void parar();
	void girar(int angulo_robo_graus); // positivo anti-horario

	bool inicializar_thread_aceleracao();
	bool finalizar_thread_aceleracao();
	flag_aceleracao get_estado();
	int get_tacometro();
	double get_distancia();


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
	double fator_croda = 0.9945;
	double delay = 5.0;//Em miliseg
	double aceleracao = 1000.0;//Em pwm/seg
	double raio_roda = 0.0545/2; // metros
	double relacao_engrenagem = 0.5; // 2 volta motor equivale 1 voltas roda
	double raio_robo = 0.1506/2; // largura entre os centros das rodas div por 2
	double angulo_robo_graus = 0; // quanto o robo vai girar quando chamar o metodo girar
	double distancia_linha_reta = 0; // distancia que  o robo ja andou

	/*Variáveis controlador PWM*/
	double erro = 0.0;
	double erro_anterior = 0.0;
	double kp = 2;// melhor valor 2
	double kd = 0; // melhor valor 0 (duvida, olha os graficos)
	double pid = 0;

	double pwm_sp = 0;//Valor desejado
	double pwm_retardada = 0.0;//Valor que será incrementado
	double pwm = 0;//Valor argumento da função "set_duty_cycle_sp"

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
