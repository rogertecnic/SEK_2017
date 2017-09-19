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



void teste_rogerio(){
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
	Sensor_cor cor(ev3dev::INPUT_1, ev3dev::INPUT_2);
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
	//cor.calibra();
	//	while(!ev3dev::button::back.process()){
	//		while(!ev3dev::button::enter.process()){}
	//		while(!ev3dev::button::enter.process()){}
	//		robot.andar(50);
	//		while(!ev3dev::button::enter.process())
	//			cout<<cor.ler_cor_E()<<";"<<cor.ler_cor_D()<<endl;
	//		robot.parar();
	//		while(!ev3dev::button::enter.process()){}
	//	}


	//se der certo depois eh so imbutir isso tudo em um metodo
	//e passar como argumento o primeiro sensor e o segundo sensor q detectar a linha
	double 	velo_robo = 0,
			dist = 0,
			ang_robo = 0,
			posicao_inicial = 0;
	chrono::system_clock::time_point t_inicial;
	chrono::system_clock::time_point t_final;
	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg

	/*
	 * VERIFICA SE A VELOCIDADE LIDA EH A VELOCIDADE REAL
	 * RESULTADOS BEM SATISFATORIOS, PRECISAO DE 0.001 m/s
	 */
	//	t_inicial = Time::now();
	//robot.andar(70);
	//	while(robot.get_distancia() < 3){}
	//	velo_robo = robot.get_velocidade();
	//	robot.parar();
	//	t_final = Time::now();
	//	delta_t = t_final - t_inicial;
	//	cout<<3/delta_t.count()<<endl;
	//	cout<<velo_robo<<endl;

	/*
	 * METODO PARA CORRIGIR ROBO TORTO QUANDO PASSAR PELA
	 * INTERCESSAO
	 */
	int i = 0;
	cor.calibra();
	while(!ev3dev::button::back.process()){
		//		if(cor.ler_cor_D() == Cor::nda || cor.ler_cor_E() == Cor::nda){
		//			i ++;
		//			usleep(1000*100);
		//		}else i = 0;
		//
		//		if(i >=9){
		//			robot.parar();
		//			robot.andar(-50);
		//			usleep(1000*3000);
		//			robot.girar(30);
		//		}


		//velo_robo = robot.get_velocidade();
		robot.andar(70);
		cout<<cor.ler_cor_E()<<";"<<cor.ler_cor_D()<<endl;
		//cor.define_ranges();
		//		robot.parar();
		//		usleep(1000*1000);
		//		while(!ev3dev::button::enter.process()){}
		//		usleep(1000*500);
		//		ev3dev::button::enter.process();
		//		if(cor.ler_cor_E() == Cor::preto){
		//			t_inicial = Time::now();
		//			cout<<"viu esquerdo"<<endl;
		//			while(cor.ler_cor_D() != Cor::preto){
		//				if(cor.ler_cor_D() == Cor::nda){
		//					i ++;
		//					usleep(1000*100);
		//				}else i = 0;
		//
		//				if(i >=8){
		//					robot.parar();
		//					robot.andar(-50);
		//					usleep(1000*2000);
		//					robot.girar(30);
		//					break;
		//				}
		//			}
		//			if( i <= 8){
		//				t_final = Time::now();
		//				posicao_inicial = robot.get_distancia();
		//				while(robot.get_distancia() < posicao_inicial + 0.19){}
		//				robot.parar();
		//				delta_t = t_final - t_inicial;
		//				dist = velo_robo*delta_t.count();
		//				// aqui era pra ser a largura entre os sensores, mas
		//				// o angulo dava muito pequeno, entao eu diminui para dar um angulo maior
		//				ang_robo = atan2(dist, 0.17);
		//				cout<< dist<<endl;
		//				cout<<delta_t.count()<<endl;
		//				cout<<velo_robo<<endl;
		//				cout<<ang_robo*180/3.141592<<endl;t_inicial = Time::now();
		//				//while(!ev3dev::button::enter.process()){}
		//				robot.girar(-90+ang_robo*180/3.141592);
		//				while(robot.get_estado() == flag_aceleracao::girar){}
		//				robot.andar(70);
		//				usleep(1000*3000);
		//			}
		//		}
		//
		//		if(cor.ler_cor_D() == Cor::preto){
		//			t_inicial = Time::now();
		//			cout<<"viu direito"<<endl;
		//			while(cor.ler_cor_E() != Cor::preto){
		//				if(cor.ler_cor_D() == Cor::nda){
		//					i ++;
		//					usleep(1000*100);
		//				}else i = 0;
		//
		//				if(i >=8){
		//					robot.parar();
		//					robot.andar(-50);
		//					usleep(1000*2000);
		//					robot.girar(30);
		//					break;
		//				}
		//			}
		//			if( i >= 8) break;
		//			t_final = Time::now();
		//			posicao_inicial = robot.get_distancia();
		//			while(robot.get_distancia() < posicao_inicial + 0.19){}
		//			robot.parar();
		//			delta_t = t_final - t_inicial;
		//			dist = velo_robo*delta_t.count();
		//			// aqui era pra ser a largura entre os sensores, mas
		//			// o angulo dava muito pequeno, entao eu diminui para dar um angulo maior
		//			ang_robo = atan2(dist, 0.17);
		//			cout<< dist<<endl;
		//			cout<<delta_t.count()<<endl;
		//			cout<<velo_robo<<endl;
		//			cout<<ang_robo*180/3.141592<<endl;
		//			//while(!ev3dev::button::enter.process()){}
		//			robot.girar(-90- ang_robo*180/3.141592);
		//			while(robot.get_estado() == flag_aceleracao::girar){}
		//			robot.andar(70);
		//			usleep(1000*3000);
		//		}

				if(cor.ler_cor_E() == Cor::fora){
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(-35);
					robot.andar(70);
				}
				if(cor.ler_cor_D() == Cor::fora){
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(35);
					robot.andar(70);
				}
	}


	/*
	 * TESTE COM SENSOR TOUCH PARA VERIFICAR TEMPO
	 */
	//	ev3dev::touch_sensor toque(ev3dev::INPUT_1);
	//	while(!ev3dev::button::enter.process()){
	//		//while(!toque.is_pressed()){}
	//		t_inicial = Time::now();
	//		//toque.is_pressed();
	//		//toque.is_pressed();
	//		//toque.is_pressed();
	//		usleep(1000*1000);
	//		//while(toque.is_pressed()){}
	//		t_final = Time::now();
	//		delta_t = t_final - t_inicial;
	//		cout<<delta_t.count()<<endl;
	//		usleep(1000*3000);
	//	}

	//while(!ev3dev::button::enter.process()){}
	//robot.parar();
	usleep(1000*500);
	//robot.finalizar_thread_aceleracao();
}


int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	teste_rogerio();






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
