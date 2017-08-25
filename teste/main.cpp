/*
 * main.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */




#include <unistd.h>
#include "ev3dev.h"
#include <math.h>
#include <iostream>
#include <fstream>

// *******LIB TEMPO
#include <chrono>
#include "Controlador_motor.h"
#include "MArquivos.h"

using namespace std;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

ev3dev::large_motor rodaE(ev3dev::OUTPUT_A); // roda esquerda
ev3dev::large_motor rodaD(ev3dev::OUTPUT_B); // roda direita
ofstream file;

/*
 * tentativa de criar um controlador de velocidade
 * ir alterando o raio da roda
 */
void teorema_de_roger(){
	file.open ("debug_lego.txt");
	rodaE.reset();
	rodaD.reset();
	rodaE.set_stop_action("hold");
	rodaD.set_stop_action("hold");
	//rodaE.set_ramp_down_sp(1500);
	//rodaD.set_ramp_down_sp(1500);


	//atributos do motor
	double raioE = 0.04, // metro 0.0408 padrao, 0.04 para o juvenal
			raioD = 0.0402, // metro 0.0402 para o juvenal
			v_max_E = 775.57, // em graus/seg
			v_max_D = 809.59, // em graus/seg
			velocidade = 0.4, // m/s
			v_retardada = 0,
			t_aceleracao = 1, // tempo para ir de 0 m/s a "velocidade" m/s
			aceleracao = velocidade/t_aceleracao, // m/s²
			distancia = 3, // m

			//variaveis do controlador de velocidade (PID)
			delay = 5, //tempo de amostragem do controlador em ms

			erroE = 0,
			pos1E = 0,
			pos2E = 0,
			v1E = 0,
			v2E = 0,
			acumuladorE = 0,

			erroD = 0,
			pos1D = 0,
			pos2D = 0,
			v1D = 0,
			v2D = 0,
			acumuladorD = 0,

			kp = 3, // 3
			ki = 0.05, // 0.05
			kd = 0.02, // 0.02
			pwmE = 0, // -1 ate 1
			pwmD = 0;
	chrono::system_clock::time_point t1 = Time::now();
	chrono::system_clock::time_point t2;
	std::chrono::duration<double> dt; // usardt = t1-t2 e dt.count() para pegar o tempo em seg

	//********************************CONTROLADOR DE VELOCIDADE, TEM DOIS RODANDO JUNTOS********************
	//******************************************************************************************************
	//******************************************************************************************************
	//******************************************************************************************************

	// pode deletar
	chrono::system_clock::time_point ti = t1;
	std::chrono::duration<double> dt_experimental; // fazer possiveis medidas
	bool acelerou = false;
	//pode deletar

	v_max_E = v_max_E*3.141592/180*raioE; // convertendo para m/s
	v_max_D = v_max_D*3.141592/180*raioE; // convertendo para m/s
	while(!ev3dev::button::enter.process()){
		pos2E = rodaE.position()*3.141592/180*raioE;
		pos2D = rodaD.position()*3.141592/180*raioD;
		t2 = Time::now();
		dt = t2-t1;

		v2E = (pos2E - pos1E)/dt.count(); // count retorna em seg
		v2D = (pos2D - pos1D)/dt.count();

		erroE = (v_retardada - v2E)*100/v_max_E;// m/s
		erroD = (v_retardada - v2D)*100/v_max_D;// m/s


		acumuladorE += erroE;
		acumuladorD += erroD;

		pwmE = kp*erroE + ki*acumuladorE + kd*(v2E - v1E);
		pwmD = kp*erroD + ki*acumuladorD + kd*(v2D - v1D);
		if(pwmE > 100) pwmE =100 ;
		if(pwmE < -100) pwmE = -100;
		if(pwmD > 100) pwmD = 100;
		if(pwmD < -100) pwmD = -100;

		// pode apagar
		//		if(v2E >= velocidade && !acelerou){
		//			acelerou = true;
		//			dt_experimental = t2-ti;
		//			ti = t2;
		//			file<<"terminou de acelerar:"<<endl;
		//			file<<v2E<<"\t"<<pos2E<<"\t"<< dt_experimental.count()<<endl;
		//		}
		// pode apagar

		file<<pwmE<<"\t"<<pwmD<<"\t"<<pos2E<<"\t"<<pos2D<<"\t"<<v2E<<"\t"<<v2D<<"\t"<<v_retardada<<endl;

		rodaE.set_duty_cycle_sp(pwmE);
		rodaD.set_duty_cycle_sp(pwmD);

		t1 = t2;
		pos1E = pos2E;
		pos1D = pos2D;
		v1E = v2E;
		v1D = v2D;

		if(pos2E >= distancia*2){
			//rodaE.stop();
			//rodaD.stop();
			//			dt_experimental = t2-ti;
			//			file<<"terminou o percurso:"<<endl;
			//			file<<v2E<<"\t"<<pos2E<<"\t"<< dt_experimental.count()<<endl;
			while(!ev3dev::button::enter.process()){
			}
			break;
		}

		// testando desaceleracao e parada
		//		if(pos2E > 1 && pos2E < 2.7){
		//			velocidade = 0.3;
		//			acumuladorD = 0;
		//			acumuladorE = 0;
		//		}
		if(pos2E > 2.7){
			velocidade = 0;
			aceleracao = 0.3/0.5;
			acumuladorD = 0;
			acumuladorE = 0;
		}
		if(velocidade == 0 &&
				(v_retardada > -aceleracao*delay/1000 && v_retardada < aceleracao*delay/1000)){
			rodaE.stop();
			rodaD.stop();
			cout<<"parou"<<endl;
			while(velocidade == 0 && !ev3dev::button::enter.process()){

			}
			break;
		}


		usleep(1000*delay);
		if(v_retardada < (velocidade-aceleracao*delay/1000)) v_retardada += aceleracao*dt.count();
		if(v_retardada > (velocidade+aceleracao*delay/1000)) v_retardada -= aceleracao*dt.count();
		rodaE.run_direct();
		rodaD.run_direct();
	}

}

/**
 * testa a aceleracao da biblioteca
 */
void testeAceleracao(){
	// usando as funcoes ramp_up e ramp_down:
	// argumento: milliseg que o motor demora pra acelerar/desacelerar de 0
	// ate a velocidade maxima teórica (max_speed)
	// como não queremos a velocidade máxima precisamos fazer umas conversões
	// tambem da pra fazer por semelhança de triangulos imaginando
	// uma reta num grafico velo vs tempo o seu triangulo menor
	// teria os catetos como o t_sp e o v_sp e o triangulo maior
	// seria o tm e o vm, daria na mesma conta, a aceleraçao eh a inclinacao
	// da reta, aooo fisica 1 em

	int v_sp = 180*3; // setpoint da velocidade real
	int t_sp = 5; // tempo de aceleracao/desaceleracao de 0 ate v_sp
	int a_sp = v_sp/t_sp; // aceleracao/desaceleracao que queremos no motor
	int vm = rodaD.max_speed(); // velocidade teorica maxima do motor
	int tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg

	rodaD.reset();
	rodaD.set_position_sp(360*25);
	rodaD.set_speed_sp(v_sp);
	rodaD.set_ramp_up_sp(tm*1000); // em miliseg
	rodaD.set_ramp_down_sp(50);
	rodaD.run_forever();

	auto t0 = Time::now();
	std::cout<<"iniciado"<<endl;

	while((rodaD.speed()>=180*3?false:true )){
	}

	auto t1 = Time::now();
	fsec fs = t1 - t0;
	//std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
	std::cout << fs.count() << "s\n";
	//std::cout << d.count() << "ms\n";


	// espera apertar e soltar o botao enter do brick
	while(!ev3dev::button::enter.process()){
	}
	while(!ev3dev::button::enter.process()){
	}

}

/**
 * metodo simplesmente anda ate a posicao e para, usando a biblioteca
 */
void anda_e_para(){
	rodaE.reset();
	rodaD.reset();
	rodaE.set_stop_action("hold");
	rodaD.set_stop_action("hold");

	file.open ("debug_lego.txt");

	// usando o controlador pid interno
	int pos_sp = 360*120;
	//	rodaE.set_position_sp(pos_sp);
	//	rodaD.set_position_sp(pos_sp);
	rodaE.set_speed_sp(180*2);
	rodaD.set_speed_sp(180*2);
	//	rodaE.set_ramp_up_sp(4180);
	//	rodaD.set_ramp_up_sp(4000);
	//	rodaE.run_to_rel_pos();
	//	rodaD.run_to_rel_pos();

	// usando apenas o pwm sem controlador

	rodaE.set_duty_cycle_sp(25);
	rodaD.set_duty_cycle_sp(25);
	chrono::system_clock::time_point ti = Time::now();
	chrono::system_clock::time_point tf;
	double pos_inicialE = 0;
	double pos_finalE = 0;
	double pos_inicialD = 0;
	double pos_finalD = 0;
	std::chrono::duration<double> dt; // usardt = t1-t2 e dt.count() para pegar o tempo em seg
	double tempoooooooo =0;
	//file<<"velo_e"<<"\t"<<"velo_d"<<"\t"<<"v_e_lib"<<"\t"<<"v_d_lib"<<endl;
	ti = Time::now();
	while(!ev3dev::button::enter.process() &&
			rodaE.position() <= pos_sp){
		pos_inicialE = rodaE.position();
		pos_inicialD = rodaD.position();

		usleep(1000*5);
		pos_finalE = rodaE.position();
		pos_finalD = rodaD.position();
		tf = Time::now();

		dt = tf - ti;
		ti = tf;
		tempoooooooo += dt.count();
		//file <<(pos_finalE-pos_inicialE)/dt.count()<<"\t"<<(pos_finalD-pos_inicialD)/dt.count()<<"\t"<<rodaE.speed()<<"\t"<<rodaD.speed()<<"\t"<<tempoooooooo<<endl;

		// tempo, posicao, velocidade media
		//arquivo.elementos_arq(3, tempoooooooo, pos_finalE, rodaE.speed());
		rodaE.run_forever();
		rodaD.run_forever();
	}
	rodaE.stop();
	rodaD.stop();

	// espera apertar e soltar o botao enter do brick
	while(!ev3dev::button::enter.process()){
		file << rodaE.position()<<"\t"<<rodaD.position()<<"\t"<< rodaE.speed()<<"\t"<<rodaD.speed()<<endl;
	}
	while(!ev3dev::button::enter.process()){
	}
}



/**
 * tentativa de controlar a velocidade e aceleracao do robo falha, os tacometros sao errados
 */
void teste_controle_velocidade(){
	rodaE.reset();
	rodaD.reset();
	rodaE.set_stop_action("hold");
	rodaD.set_stop_action("hold");

	file.open ("debug_lego.txt");



	double vm = rodaE.max_speed(); // velocidade teorica maxima do motor
	double pos_sp = 360*7;
	double v_sp = 180*2; // setpoint da velocidade real
	double t_sp = 1; // tempo de aceleracao/desaceleracao de 0 ate v_sp
	double a_sp = v_sp/t_sp; // aceleracao/desaceleracao que queremos no motor
	double tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg

	//rodaE.set_speed_sp(v_sp);
	//rodaE.set_ramp_up_sp(tm*1000);
	rodaE.set_ramp_down_sp(tm*1000/3);
	rodaE.set_position_sp(pos_sp);

	//rodaD.set_speed_sp(v_sp);
	//rodaD.set_ramp_up_sp(tm*1000);
	rodaD.set_ramp_down_sp(tm*1000/3);
	rodaD.set_position_sp(pos_sp);

	//int kgz = 1;
	float raio_roda = 0.0407; // metros, diametro/2
	float largura_robo = 0.261; // largura de uma roda a outra em metros
	//float erro_angulo = 0; // pos se o robo estiver para direita e neg se estiver para a esquerda
	//float fator_veloE = 1;
	//float fator_veloD = 1;

	//rodaE.run_to_rel_pos();
	//rodaD.run_to_rel_pos();
	//usleep(1000*50000);

	// variaveis do controle de posicao
	double erro_pos;
	double erro_ang;
	double fator_posicao;
	double v_retardada=0;
	double expoente = 1;
	chrono::system_clock::time_point t1 = Time::now();
	chrono::system_clock::time_point t2;
	std::chrono::duration<double> dt; // usar dt.count() para pegar o tempo em seg


	// variaveis de controle da aceleracao
	//	chrono::system_clock::time_point t1 = Time::now();
	//	chrono::system_clock::time_point t2;
	//	std::chrono::duration<double> dt; // usar dt.count() para pegar o tempo em seg
	//	double v1E = 0;
	//	double v1D = 0;
	//	double v2E = 0;
	//	double v2D = 0;
	//
	//	double dv;
	//	double accE;
	//	double accD;
	//	double accErro;
	//	double fator_accE;
	//	double fator_accD;

	//a_sp = 360;
	rodaE.run_direct();
	rodaD.run_direct();
	while(!ev3dev::button::enter.process()){
		//**********************CONTROLADOR POSICAO*************
		// erro_pos: diferenca de posicao entre as rodas em metros
		// erro positivo se a roda direita estiver na frente
		erro_pos = (rodaD.position() - rodaE.position())*3.1415/180*raio_roda;

		// erro_ang: angulo que a frente do robo esta inclinada em relacao
		// a linha reta inicial que ele deve seguir, positivo sentido anti-hor
		erro_ang = atan2(erro_pos, largura_robo);// variando de -pi ate pi
		erro_ang = erro_ang/3.1415*10; // deixa o range de -1 ate 1
		//cout<<erro_ang<<endl;
		// fator de posicao eh um valor de ajuste que vaira de -1 a 1
		// que indica o quanto o robo esta inclinado
		if(erro_ang >0){
			fator_posicao = pow(erro_ang,expoente);
			if(fator_posicao > 1) fator_posicao = 1;
		}
		if(erro_ang<0){
			erro_ang = abs(erro_ang);
			fator_posicao = -pow(erro_ang,expoente);
			if(fator_posicao < -1) fator_posicao = -1;
		}

		t2 = Time::now();
		dt = t2-t1;
		if(v_retardada<v_sp){
			v_retardada += a_sp*dt.count();
		}
		t1 = t2;
		file << (int)(v_retardada*(1+fator_posicao)>850 ? 850 : v_retardada*(1+fator_posicao) )*100/850<<";"<<(int)(v_retardada*(1-fator_posicao)>850 ? 850 : v_retardada*(1-fator_posicao) )*100/850<<";"<<dt.count()<<";"<<erro_ang<<endl;
		//cout<<(int)(v_retardada*(1+fator_posicao)>850 ? 850 : v_retardada*(1+fator_posicao) )*100/850<<";"<<(v_retardada*(1-fator_posicao)>850 ? 850 : v_retardada*(1-fator_posicao) )*100/850<<endl;
		//rodaE.set_speed_sp(1051);
		//rodaE.set_speed_sp(v_retardada*(1+fator_posicao)>1050 ? 1050 : v_retardada*(1+fator_posicao));
		//rodaD.set_speed_sp(v_retardada*(1-fator_posicao)>1050 ? 1050 : v_retardada*(1-fator_posicao));

		//rodaE.run_forever();
		//rodaD.run_forever();

		//int pwm = (v_retardada+v_retardada*(1+fator_posicao)>850 ? 850 : v_retardada*(1+fator_posicao))*100/850;
		rodaE.set_duty_cycle_sp( 0.9*(v_retardada*(1+fator_posicao)>850 ? 850 : v_retardada*(1+fator_posicao) )*100/850);
		rodaD.set_duty_cycle_sp( (v_retardada*(1-fator_posicao)>850 ? 850 : v_retardada*(1-fator_posicao) )*100/850);

		usleep(1000*100);

		//		// **********************CONTROLADOR VELOCIDADE DO ROBO*************
		//		erro_angulo = std::atan2( (rodaE.position() - rodaD.position())*raio_roda*3.1415/180 , largura_robo);
		//		fator_veloE = 1-std::sin(erro_angulo)+kgz*tan(-erro_angulo);
		//		fator_veloD= 1+std::sin(erro_angulo)+kgz*tan(erro_angulo);
		//
		//		if(fator_veloE > 1 ) fator_veloE = 1;
		//		if(fator_veloE < -1 ) fator_veloE = -1;
		//		if(fator_veloD > 1 ) fator_veloD = 1;
		//		if(fator_veloD < -1 ) fator_veloD = -1;
		//
		//		rodaE.set_speed_sp(v_sp*fator_veloE);
		//		rodaD.set_speed_sp(v_sp*fator_veloD);
		//
		//		// **********************CONTROLADOR ACELERACAO DO ROBO*************
		//		t2 = Time::now();
		//		v2E = rodaE.speed()*raio_roda*3.1415/180 ;
		//		v2D = rodaD.speed()*raio_roda*3.1415/180 ;
		//		dt = t2-t1;
		//		t1 = t2;
		//
		//		dv = v2E-v1E;
		//		accE = dv/dt.count();
		//		v1E = v2E;
		//
		//		dv = v2D-v1D;
		//		accD = dv/dt.count();
		//		v1D = v2D;
		//		cout<<accD<<endl;
		//
		//		kgz = 60;
		//		accErro = (accE - accD);// accD maior erro negativo
		//		fator_accE = 2-std::sin(accErro)+kgz*tan(-accErro);
		//		fator_accD= 2+std::sin(accErro)+kgz*tan(accErro);
		//
		//		if(fator_accE >2) fator_accE = 2;
		//		if( fator_accE < 0) fator_accE = 1;
		//		if(fator_accD >2) fator_accD = 2;
		//		if( fator_accD < 0) fator_accD = 1;
		//
		//		fator_veloE = (fator_veloE+1)/2;
		//		fator_veloD = (fator_veloD+1)/2;
		//		rodaE.set_ramp_up_sp(calctAcc(360*fator_veloE));
		//		rodaD.set_ramp_up_sp(calctAcc(360*fator_veloD));
		//
		//		rodaE.run_forever();
		//		rodaD.run_forever();
		//		usleep(1000*5);
		//
		if(rodaE.position()>=pos_sp){
			rodaE.stop();
			rodaD.stop();
			while(!ev3dev::button::enter.process()){
			}
			while(!ev3dev::button::enter.process()){
			}
			break;
		}

	}
	file.close();
}
















int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 775, 0.3, true,  "debug_lego_E.m");
	Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.0408, 809, 0.3, true,  "debug_lego_D.m");

	rodaE.set_velo(0.3);
	rodaD.set_velo(0.3);
	//anda_e_para();

	while(!ev3dev::button::enter.process()){
	}

	rodaE.finaliza_thread();
	rodaD.finaliza_thread();
	//teste_controle_velocidade();
	//teorema_de_roger();
	//anda_e_para();
}
