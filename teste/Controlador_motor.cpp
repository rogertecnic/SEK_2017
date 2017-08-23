/*
 * Classe.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */

#include "Controlador_motor.h"

Controlador_motor::Controlador_motor(string motor_port, double raio, double velo_max, double aceleracao, bool debug = false)
:roda(ev3dev::large_motor(motor_port)),
 raio(raio),
 delta_S_roda(0),
 velo_sp(0),
 velo_retardada(0),
 velo_max(velo_max),
 aceleracao(aceleracao),
 debug(debug),
 thread_controle(std::thread(&Controlador_motor::loop_controlador, this))
{
	roda.reset();
	roda.set_stop_action("hold");
}


void Controlador_motor::tora_o_pau(){ // gira pro infinito e alem com pwm = 100
	roda.set_duty_cycle_sp(100);
	roda.run_direct();
}


void Controlador_motor::set_velo(double velocidade){
	this->velo_sp = velo_sp;
}


void Controlador_motor::set_acc(double acc){
	aceleracao = acc;
}

void Controlador_motor::loop_controlador(){
	while(1){
		pos_final = roda.position()*3.141592/180*raio;
		t_final = Time::now();
		delta_t = t_final-t_inicial;

		velo_final = (pos_final - pos_inicial)/delta_t.count(); // count retorna em seg

		erro = (velo_retardada - velo_final)*100/velo_max;// m/s


		acumulador += erro;

		pwm = kp*erro + ki*acumulador + kd*(velo_final - velo_inicial);
		if(pwm > 100) pwm =100 ;
		if(pwm < -100) pwm = -100;

		//file<<pwmE<<"\t"<<pwmD<<"\t"<<pos2E<<"\t"<<pos2D<<"\t"<<v2E<<"\t"<<v2D<<"\t"<<v_retardada<<endl;

		roda.set_duty_cycle_sp(pwm);

		t_inicial = t_final;
		pos_inicial = pos_final;
		velo_inicial = velo_final;

		if(velo_sp == 0 &&
				(velo_retardada < aceleracao*delay/1000 && velo_retardada > -aceleracao*delay/1000)){
			roda.stop();
			cout<<"parou"<<endl;
			while(velo_sp == 0){
				usleep(1000*delay);
			}
		}


		usleep(1000*delay);
		if(velo_retardada < (velo_sp-aceleracao*delay/1000)) velo_retardada += aceleracao*delta_t.count();
		if(velo_retardada > (velo_sp+aceleracao*delay/1000)) velo_retardada -= aceleracao*delta_t.count();
		roda.run_direct();
	}
}
