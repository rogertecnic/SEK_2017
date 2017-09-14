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
	Sensor_cor corE(ev3dev::INPUT_1);
	Sensor_cor corD(ev3dev::INPUT_2);
	robot.inicializar_thread_aceleracao();
	/*
	 * teste da classe controlador_robo
	 */
	//robot.andar(50);
	//	while(!ev3dev::button::enter.process())
	//	cout<<robot.get_velocidade()<<endl;
	//	usleep(1000*2000);
	//	robot.girar(360*2);
	//	usleep(1000*2000);
	//	robot.andar(50);
	//	usleep(1000*2000);
	//	robot.andar(-50);
	//	usleep(1000*2000);
	//	robot.girar(-360);
	//	while(!ev3dev::button::enter.process()){}

	/*
	 * teste da classe sensor_cor
	 */
	corE.calibra();
	corD.calibra();
	//		while(!ev3dev::button::back.process()){
	//			while(!ev3dev::button::enter.process()){}
	//			while(!ev3dev::button::enter.process()){}
	//			robot.andar(50);
	//			while(!ev3dev::button::enter.process())
	//				cout<<corE.ler_cor()<<";"<<corD.ler_cor()<<endl;
	//			robot.parar();
	//			while(!ev3dev::button::enter.process()){}
	//		}


	//se der certo depois eh so imbutir isso tudo em um metodo
	//e passar como argumento o primeiro sensor e o segundo sensor q detectar a linha
	double leu_corE = 0,
			leu_corD = 0,
			velo_robo = 0,
			dist = 0,
			ang_robo = 0,
			posicao_inicial = 0;
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	robot.andar(50);
	while(!ev3dev::button::enter.process()){
		velo_robo = robot.get_velocidade();
		cout<<corE.ler_cor()<<";"<<corD.ler_cor()<<endl;
		if(corE.ler_cor() != Cor::branco){
			cout<<"viu esquerdo"<<endl;
			t_inicial = Time::now();
			while(corD.ler_cor() == Cor::branco){}
			t_final = Time::now();
			posicao_inicial = robot.get_distancia();
			while(robot.get_distancia() < posicao_inicial + 0.19){}
			robot.parar();
			delta_t = t_final - t_inicial;
			dist = velo_robo*delta_t.count();
			// aqui era pra ser a largura entre os sensores, mas
			// o angulo dava muito pequeno, entao eu diminui para dar um angulo maior
			ang_robo = atan2(dist, 0.11);
			cout<< dist<<endl;
			cout<<delta_t.count()<<endl;
			cout<<velo_robo<<endl;
			cout<<ang_robo*180/3.141592<<endl;
			//while(!ev3dev::button::enter.process()){}
			robot.girar(-90+ang_robo*180/3.141592);
			while(robot.get_estado() == flag_aceleracao::girar){}
			robot.andar(50);
			usleep(1000*3000);
			cout<<"partiu"<<endl;
		}

		if(corD.ler_cor() != Cor::branco){
			cout<<"viu direito"<<endl;
			t_inicial = Time::now();
			while(corE.ler_cor() == Cor::branco){}
			t_final = Time::now();
			posicao_inicial = robot.get_distancia();
			while(robot.get_distancia() < posicao_inicial + 0.19){}
			robot.parar();
			delta_t = t_final - t_inicial;
			dist = velo_robo*delta_t.count();
			// aqui era pra ser a largura entre os sensores, mas
			// o angulo dava muito pequeno, entao eu diminui para dar um angulo maior
			ang_robo = atan2(dist, 0.11);
			cout<< dist<<endl;
			cout<<delta_t.count()<<endl;
			cout<<velo_robo<<endl;
			cout<<ang_robo*180/3.141592<<endl;
			//while(!ev3dev::button::enter.process()){}
			robot.girar(-90-ang_robo*180/3.141592);
			while(robot.get_estado() == flag_aceleracao::girar){}
			robot.andar(50);
			usleep(1000*3000);
			cout<<"partiu"<<endl;
		}
	}


	//robot.andar(50, 3);


	while(!ev3dev::button::enter.process()){}
	robot.parar();
	usleep(1000*500);
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
