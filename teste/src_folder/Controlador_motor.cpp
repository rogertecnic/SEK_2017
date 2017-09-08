/*
 * Classe.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */

#include "Controlador_motor.h"

Controlador_motor::Controlador_motor(string motor_port, double raio, double velo_max, bool debug = false, string arquivo_nome = "")
:arquivo_nome(arquivo_nome),
 roda(ev3dev::large_motor(motor_port)),
 raio(raio),
 velo_max(velo_max),
 debug(debug),
 arquivo_aberto(debug)
{
	if(debug)
		arquivo = new M_arquivos(arquivo_nome, 3); // vetores: tempo, pos, velo, pwm, erro;

	roda.reset();
	roda.set_stop_action("hold");
}



void Controlador_motor::tora_o_pau(string arquivo_nome){ // gira pro infinito e alem com pwm = 100
	this->arquivo_nome = arquivo_nome;
	thread_controle = thread(&Controlador_motor::loop_tora_o_pau, this);
	thread_controle.detach();
}


void Controlador_motor::set_velo(double velo_sp){
	this->velo_sp = velo_sp;
}



bool Controlador_motor::fecha_arquivo(){
	if(arquivo_aberto)
		arquivo->fecha_arq();
	return true;
}


bool Controlador_motor::finaliza_thread(){
	if(thread_rodando){
		thread_rodando = false;
		while(arquivo_aberto){};
	}
	usleep(1000*100);
	return true;
}


bool Controlador_motor::inicializa_thread(){
	thread_controle = thread(&Controlador_motor::loop_controlador, this);
	thread_controle.detach();
	usleep(1000*100);
	return thread_rodando;
}

void Controlador_motor::loop_tora_o_pau(){
	thread_rodando = true;
	arquivo = new M_arquivos(arquivo_nome,2);
	arquivo_aberto = true;
	debug = true;
	roda.set_duty_cycle_sp(100);
	roda.run_direct();
	t_inicial = Time::now();

	while(thread_rodando){
		for(int i = 0 ; i<5 ; i++)
			velo_final_med += roda.speed();

		t_final = Time::now();
		delta_t = t_final - t_inicial;
		velo_final_med = velo_final_med/5;
		arquivo->elementos_arq(delta_t.count(),velo_final_med*3.141592/180*raio);
	}

	roda.stop();
	arquivo_aberto = !fecha_arquivo();
	arquivo->string_arq("plot(t,x1);");
}


void Controlador_motor::loop_controlador(){
		thread_rodando = true;
		t_inicial = Time::now();
	//
	//	double posicao_final = 0;
	//	double posicao_inicial = roda.position();
	//	roda.set_duty_cycle_sp(30);
	//	roda.run_direct();
	//	while(thread_rodando){
	//		while(posicao_final < (posicao_inicial + 10)){
	//			posicao_final = roda.position();
	//		}
	//		t_final = Time::now();
	//
	//		if(posicao_final != (posicao_inicial + 10)){
	//			posicao_inicial += 10;
	//			t_inicial = t_final;
	//		} else{
	//			delta_t = t_final - t_inicial;
	//			t_inicial = t_final;
	//			tempo_total += delta_t.count();
	//
	//			velo_final_med = (posicao_final - posicao_inicial)/delta_t.count();
	//
	//			arquivo->elementos_arq((double)tempo_total, (double)posicao_inicial, (double)velo_final_med);
	//			posicao_inicial = posicao_final;
	//		}
	//	}
	//
	//	roda.stop();
	//	arquivo_aberto = !fecha_arquivo();
	//	if(debug) arquivo->string_arq("plot(t,x4);");


	while(thread_rodando){
		//******************TIRA A MEDIA DA VELOCIDADE REAL INSTANTANEA*******
		for(int i = 0 ; i < 5 ; i++){
			velo_final_med += roda.speed()*3.141592/180*raio;// graus/s para m/s
			usleep(1000*delay);
		}
		velo_final_med = velo_final_med/5;

		//******************SALVA DADOS EM ARQUIVO*********
		if(debug){
			t_final = Time::now();
			delta_t = t_final - t_inicial;
			t_inicial = t_final;
			tempo_total += delta_t.count();
			arquivo->elementos_arq((double)tempo_total,(double)(roda.position()*3.141592/180*raio), (double)velo_final_med, (double)pwm, (double)erro);
		}

		//******************CONTROLADOR PID VELOCIDADE*******
		erro = (velo_sp - velo_final_med)*100/velo_max;// m/s
		acumulador += erro;
		pwm = kp*erro + ki*acumulador + kd*(velo_final_med - velo_inicial_med);

		//******************CEIFA O PWM NO INTERVALO PERMITIDO********
		if(pwm > 100) pwm =100;
		if(pwm < -100) pwm = -100;

		//******************SALVA VELOCIDADE PARA PROXIMA ITERACAO*********
		velo_inicial_med = velo_final_med;
		velo_final_med =0;

		roda.set_duty_cycle_sp(pwm);



		if(velo_sp != 0)roda.run_direct();
		else roda.stop();
	}

	roda.stop();
	arquivo_aberto = !fecha_arquivo();
	if(debug) arquivo->string_arq("plot(t,x4);");
}


double Controlador_motor::get_velo(){
	return velo_inicial_med;
}
double Controlador_motor::get_posicao(){
	//TODO fazer metodo get_posicao
	return 0;
}
