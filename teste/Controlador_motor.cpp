/*
 * Classe.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */

#include "Controlador_motor.h"

Controlador_motor::Controlador_motor(string motor_port, double raio, double velo_max, double aceleracao, bool debug = false, string arquivo_nome = "")
:arquivo_nome(arquivo_nome),
 roda(ev3dev::large_motor(motor_port)),
 raio(raio),
 velo_max(velo_max),
 aceleracao(aceleracao),
 debug(debug),
 arquivo_aberto(debug),
 arquivo(arquivo_nome, 5), // vetores: tempo, pos, velo, pwm, erro;
 thread_controle(std::thread(&Controlador_motor::loop_controlador, this))
{

	roda.reset();
	roda.set_stop_action("hold");
	thread_controle.detach();
}



void Controlador_motor::tora_o_pau(){ // gira pro infinito e alem com pwm = 100
	roda.set_duty_cycle_sp(100);
	roda.run_direct();
}


void Controlador_motor::set_velo(double velo_sp){
	this->velo_sp = velo_sp;
}


void Controlador_motor::set_aceleracao(double aceleracao){
	this->aceleracao = aceleracao;
}


bool Controlador_motor::fecha_arquivo(){
	arquivo.fecha_arquivo();
	return true;
}


bool Controlador_motor::finaliza_thread(){
	if(!thread_finalizar){
		thread_finalizar = true;
		while(arquivo_aberto){};
	}
	return true;
}


void Controlador_motor::loop_controlador(){
	velo_max = velo_max*3.141592/180*raio;

	while(!thread_finalizar){
		for(int i = 0; i < 6 ; i++){
			pos_final = roda.position()*3.141592/180*raio;
			t_final = Time::now();
			delta_t = t_final-t_inicial;
			delta_t_5_iteracoes += delta_t.count();
			tempo_total += delta_t.count();

			velo_final = (pos_final - pos_inicial)/delta_t.count(); // count retorna em seg
			velo_final_med += roda.speed();//velo_final;
			t_inicial = t_final;
			pos_inicial = pos_final;
			usleep(1000*delay);
		}
		velo_final_med = velo_final_med/5;

		erro = (velo_retardada - velo_final_med)*100/velo_max;// m/s
		acumulador += erro;

		//pwm = velo_retardada*100/velo_max;
		pwm = velo_sp*100/velo_max;
		pwm += kp*erro + ki*acumulador + kd*(velo_final_med - velo_inicial_med);
		if(pwm > 100) pwm =100 ;
		if(pwm < -100) pwm = -100;

		if(debug)arquivo.elementos_arq(tempo_total,pos_final, velo_final_med, pwm, erro);

		velo_inicial_med = velo_final_med;
		velo_final_med =0;

		roda.set_duty_cycle_sp(pwm);

		//verifica se a roda esta quase parando, se sim, trava a roda
		if(velo_sp == 0 &&
				(velo_retardada < aceleracao*delay/1000 && velo_retardada > -aceleracao*delay/1000)){
			roda.stop();
			cout<<"parou"<<endl;
			while(velo_sp == 0){

				usleep(1000*delay);
			}
		}

		if(velo_retardada < (velo_sp-aceleracao*delay*2/1000)) velo_retardada += aceleracao*delta_t_5_iteracoes;
		if(velo_retardada > (velo_sp+aceleracao*delay*2/1000)) velo_retardada -= aceleracao*delta_t_5_iteracoes;
		delta_t_5_iteracoes =  0;
		roda.run_direct();
	}
	roda.stop();
	arquivo_aberto = !fecha_arquivo();
}
