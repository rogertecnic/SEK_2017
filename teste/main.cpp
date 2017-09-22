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
	 * imprime o que o sensor esta lendo usando a classe
	 */
		cor.calibra();
		while(!ev3dev::button::back.process()){
			while(!ev3dev::button::enter.process()){}
			usleep(1000*800);
			ev3dev::button::enter.process();
			robot.andar(30);
			while(!ev3dev::button::enter.process())
				cout<<cor.ler_cor_E()<<";"<<cor.ler_cor_D()<<endl;
			robot.parar();
			usleep(1000*800);
			ev3dev::button::enter.process();
		}


	/*
	 * teste da classe de cor
	 * define os ranges de quanto a leitura da cor varia
	 */
	//	while(!ev3dev::button::back.process()){
	//		while(!ev3dev::button::enter.process()){}
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//		robot.andar(30);
	//		cor.define_ranges();
	//		robot.parar();
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//	}


	/*
	 * teste da classe de cor
	 * imprime a leitura rgb do sensor esquerdo
	 */
	//	while(!ev3dev::button::back.process()){
	//		cor.printa_rgb_E();
	//	}


	/*
	 * VERIFICA SE A VELOCIDADE LIDA EH A VELOCIDADE REAL
	 * RESULTADOS BEM SATISFATORIOS, PRECISAO DE 0.001 m/s
	 */
	//	double 	velo_robo = 0,
	//			dist = 0,
	//			ang_robo = 0,
	//			posicao_inicial = 0;
	//	chrono::system_clock::time_point t_inicial;
	//	chrono::system_clock::time_point t_final;
	//	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	//	t_inicial = Time::now();
	//	robot.andar(70);
	//	while(robot.get_distancia() < 3){}
	//	velo_robo = robot.get_velocidade();
	//	robot.parar();
	//	t_final = Time::now();
	//	delta_t = t_final - t_inicial;
	//	cout<<3/delta_t.count()<<endl;
	//	cout<<velo_robo<<endl;


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
void teste_rogerio_alinhamento(){
	Controlador_robo robot(true, "debug posicao direto no pwm.m"); // fator_croda = 1.005
	Sensor_cor cor(ev3dev::INPUT_1, ev3dev::INPUT_2);
	robot.inicializar_thread_aceleracao();

	double 	dist = 0,
			ang_robo = 0,
			posicao_inicial = 0,
			posicao_final = 0;

	int i = 0;
	cor.calibra();
	while(!ev3dev::button::back.process()){
		// verifica se o sensor esta vendo "nda" por um longo tempo
		if(cor.ler_cor_D() == Cor::nda || cor.ler_cor_E() == Cor::nda){
			i ++;
			usleep(1000*100);
		}else i = 0;
		if(i >=6){
			cout<<"viu nda 6x"<<endl;
			robot.parar();
			robot.andar(-50);
			usleep(1000*3000);
			robot.girar(30);
		}
		//***********termina verificacao


		robot.andar(70);
		// se o sensor E ver preto
//		if(cor.ler_cor_E() == Cor::preto){
//			cout<<"viu esquerdo"<<endl;
//			posicao_inicial = robot.get_distancia();
//			while(cor.ler_cor_D() != Cor::preto){}
//			posicao_final = robot.get_distancia();
//			while(robot.get_distancia() < posicao_final + 0.19){}
//			robot.parar();
//			dist = posicao_final - posicao_inicial;
//			ang_robo = atan2(dist, 0.17);
//			cout<< dist<<endl;
//			cout<<ang_robo*180/3.141592<<endl;
//			robot.girar(-90+ang_robo*180/3.141592);
//			while(robot.get_estado() == flag_aceleracao::girar){}
//			robot.andar(70);
//			usleep(1000*3000);
//		}
//
//		// se o sensor D ver preto
//		if(cor.ler_cor_D() == Cor::preto){
//			cout<<"viu direito"<<endl;
//			posicao_inicial = robot.get_distancia();
//			while(cor.ler_cor_E() != Cor::preto){}
//			posicao_final = robot.get_distancia();
//			while(robot.get_distancia() < posicao_final + 0.19){}
//			robot.parar();
//			dist = posicao_final - posicao_inicial;
//			ang_robo = atan2(dist, 0.17);
//			cout<< dist<<endl;
//			cout<<ang_robo*180/3.141592<<endl;
//			robot.girar(-90- ang_robo*180/3.141592);
//			while(robot.get_estado() == flag_aceleracao::girar){}
//			robot.andar(70);
//			usleep(1000*3000);
//		}


		if(cor.ler_cor_E() == Cor::fora){
			cout<<"saiu E"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*2000);
			robot.girar(-35);
			robot.andar(70);
		}
		if(cor.ler_cor_D() == Cor::fora){
			cout<<"saiu D"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*2000);
			robot.girar(35);
			robot.andar(70);
		}
	}
}


int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	//teste_rogerio();
	teste_rogerio_alinhamento();





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
