#ifndef CONTROLADOR_MOTOR_H_
#define CONTROLADOR_MOTOR_H_
#include <thread>
#include <chrono>
#include <unistd.h>
#include <math.h>
#include "M_arquivos.h"
#include "ev3dev.h"
using namespace std;
typedef std::chrono::high_resolution_clock Time;

class Controlador_motor {
public:
	Controlador_motor(string motor_port, double raio, double velo_max, double aceleracao, bool debug, string nome_arquivo);
	void tora_o_pau(string arquivo_nome);  //gira pro infinito e alem com pwm = 100 e salva um arquivo com as velos maximas
	void set_velo(double velo_sp);
	void set_aceleracao(double aceleracao);
	bool finaliza_thread();
	bool inicializa_thread();
	//TODO adicionar metodo de leitura de distancia
private:

	//****************VARIAVEIS DO MOTOR********************
	//******************************************************
	//******************************************************
	double raio = 0, // metro 0.0408 padrao, 0.04 para o juvenal
	velo_sp = 0, // m/s
	velo_retardada = 0,
	velo_max = 0, // em graus/seg
	aceleracao = 0; // m/s²


	//***************VARIAVEIS DO CONTROLADOR***************
	//******************************************************
	//******************************************************
	int delay = 5; // ms
	double erro = 0,
			velo_inicial_med = 0,
			velo_final_med = 0,
			acumulador = 0,
			kp = 1.2, // 1.2
			ki = 0.1, // 0.1
			kd = 0.1, // 0.1
			pwm = 0, // -100 ate 100
			tempo_total = 0;
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	std::chrono::duration<double> delta_t; // usar dt = t1-t2 e dt.count() para pegar o tempo em seg

	//*****************VARIAVEIS SISTEMA********************
	//******************************************************
	//******************************************************
	bool fecha_arquivo();
	M_arquivos *arquivo;
	string arquivo_nome;
	bool debug;
	bool thread_rodando = false;
	bool arquivo_aberto = false;
	thread thread_controle; // objeto da thread
	thread thread_tora_o_pau; // objeto da thread
	void loop_controlador();
	void loop_tora_o_pau();
	ev3dev::large_motor roda;
};

#endif /* CONTROLADOR_MOTOR_H_ */
