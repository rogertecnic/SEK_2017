#include <M_arquivos.h>
#include <Sensor_cor.h>
#include <unistd.h>
#include "ev3dev.h"
#include "src_folder/Controlador_motor.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Ultrassom_nxt.h"
#include "src_folder/Controlador_robo.h"
using namespace std;
#include <chrono>
typedef std::chrono::high_resolution_clock Time;

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");


	//*****************************************
	//*****************************************
	//**************TESTES ROGERIO***************
	//*****************************************
	//	Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.7, true,  "debug_lego_E.m");
	//	Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04055, 0.7, true,  "debug_lego_D.m");
	//
	//	rodaE.inicializa_thread();
	//	rodaD.inicializa_thread();
	//
	//	rodaE.set_velo(0.3);
	//	rodaD.set_velo(0.3);
	//
	//	while(!ev3dev::button::enter.process()){}
	//
	//	rodaE.set_velo(0);
	//	rodaD.set_velo(0);
	//
	//	rodaE.finaliza_thread();
	//	rodaD.finaliza_thread();

	//	M_arquivos arquivo("debug posicao direto no pwm.m", 4);
	//
	//	chrono::system_clock::time_point t_inicial;
	//	chrono::system_clock::time_point t_final;
	//	std::chrono::duration<double> delta_t; // usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	//	double tempo = 0;
	//	ev3dev::large_motor rodaE(ev3dev::OUTPUT_A);
	//	ev3dev::large_motor rodaD(ev3dev::OUTPUT_B);
	//	rodaE.reset();
	//	rodaD.reset();
	//	rodaE.set_duty_cycle_sp(-100);
	//	rodaD.set_duty_cycle_sp(-100);
	//	//
	//	rodaE.run_direct();
	//	rodaD.run_direct();
	//	int erro = 0;
	//	int pwm_sp = 60;
	//	int pwm = pwm_sp;
	//	double fator = 10;
	//	t_inicial = Time::now();
	//
	//	double acumulador = 0,
	//			kp = 6, // 6
	//			kd = 1,
	//			ki = 0,
	//			erro_ant = 0,
	//			pid = 0;
	//
	//	while(!ev3dev::button::enter.process()){
	//		erro = (rodaE.position()*1.005) - rodaD.position();
	//		acumulador += erro;
	//		pid = kp*erro + kd*(erro - erro_ant) + ki*acumulador;
	//
	//		pwm = pwm_sp - pid;
	//		if(pwm > 100) pwm = 100;
	//		if(pwm < -100) pwm = -100;
	//		rodaE.set_duty_cycle_sp(pwm);
	//
	//		pwm = pwm_sp + pid;
	//		if(pwm > 100) pwm = 100;
	//		if(pwm < -100) pwm = -100;
	//		rodaD.set_duty_cycle_sp(pwm);
	//		usleep(1000*5);
	//
	//		t_final = Time::now();
	//		delta_t = t_final - t_inicial;
	//		t_inicial = t_final;
	//		tempo += delta_t.count();
	//		arquivo.elementos_arq((double)tempo, (double) rodaE.position(), (double)rodaD.position(), (double)erro);
	//	}
	//	rodaE.stop();
	//	rodaD.stop();
	//	arquivo.fecha_arq();
	//	arquivo.string_arq("plot(t,x1);");



	Controlador_robo robot(true, "debug posicao direto no pwm.m"); // fator_croda = 1.005

	robot.inicializar_thread_aceleracao();
	//	robot.andar(100);
	//	while(robot.get_distancia()<3){
	//	}
	//robot.parar();
	//usleep(1000*1000*6);
	robot.girar(90);
//	usleep(1000*1000);
//	robot.andar(50);
//	usleep(1000*5000);
//	robot.andar(-50);
	while(!ev3dev::button::enter.process()){}
	robot.parar();
	//usleep(1000*500);
	robot.finalizar_thread_aceleracao();






	//*****************************************
	//*****************************************
	//**************TESTES LUANA***************
	//*****************************************
	/*	Controlador_robo robots;
		robots.inicializar_threads_motor();
		robots.inicializar_thread_aceleracao();

		cout << "Threads inicializadas" << endl;
		usleep(5*1000000);
		//while(!ev3dev::button::enter.process()){}

		robots.frente(0.3);
		usleep(10*1000000);

		robots.parar();

		cout << "Esperando... Finalizar threads" << endl;
		while(!ev3dev::button::enter.process()){}
		robots.finalizar_thread_aceleracao();
		robots.finalizar_threads_motor();*/
}
