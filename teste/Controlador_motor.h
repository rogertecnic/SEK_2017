#ifndef CONTROLADOR_MOTOR_H_
#define CONTROLADOR_MOTOR_H_
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <math.h>
#include "ev3dev.h"
#include "MArquivos.h"
using namespace std;
typedef std::chrono::high_resolution_clock Time;

class Controlador_motor {
public:
	Controlador_motor(string motor_port, double raio, double velo_max, double aceleracao, bool debug, string nome_arquivo); // construtor
	void tora_o_pau();  //gira pro infinito e alem com pwm = 100
	void set_velo(double velo_sp);
	void set_aceleracao(double aceleracao);
	bool finaliza_thread();
private:

	//****************VARIAVEIS DO MOTOR********************
	//******************************************************
	//******************************************************
	double raio = 0, // metro 0.0408 padrao, 0.04 para o juvenal
	delta_S_roda = 0, // metros rodados
	velo_sp = 0, // m/s
	velo_retardada = 0,
	velo_max = 0, // em graus/seg
	aceleracao = 0; // m/s²



	//***************VARIAVEIS DO CONTROLADOR***************
	//******************************************************
	//******************************************************
	int delay = 2; // ms
	double erro = 0,
			pos_inicial = 0,
			pos_final = 0, // acumulativo
			velo_inicial_med = 0,
			velo_final = 0,
			velo_final_med = 0,
			acumulador = 0,
			kp = 0,
			ki = 0,
			kd = 0,
			pwm = 0, // -100 ate 100
			tempo_total = 0;
	chrono::system_clock::time_point t_inicial = Time::now();
	chrono::system_clock::time_point t_final;
	std::chrono::duration<double> delta_t; // usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	double delta_t_5_iteracoes; // usado para somar as 5 iteracoes do delta_t
	//*****************VARIAVEIS SISTEMA********************
	//******************************************************
	//******************************************************
	bool fecha_arquivo();
	M_arquivos arquivo;
	string arquivo_nome;
	bool debug;
	bool thread_finalizar = false;
	bool arquivo_aberto = false;
	thread thread_controle; // objeto da thread
	void loop_controlador();
	ev3dev::large_motor roda;
};

#endif /* CONTROLADOR_MOTOR_H_ */
