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
 thread_controle(std::thread(&Controlador_motor::loop_controlador, this))
{
	if(debug) file.open(arquivo_nome);
	roda.reset();
	roda.set_stop_action("hold");
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

void Controlador_motor::close_file(){
	file.close();
}
void Controlador_motor::loop_controlador(){
	velo_max = velo_max*3.141592/180*raio;

	if(debug){
		while(!file.is_open()){ }
		file<<"v_calc"<<"\t"<<"v_speed"<<endl;
		//file<<"erro"<<"\t"<<"pwm"<<"\t"<<"pos_f"<<"\t"<<"velo_f"<<"\t"<<"v_retar"<<endl;
	}
	while(1){
		pos_final = roda.position()*3.141592/180*raio;
		t_final = Time::now();
		delta_t = t_final-t_inicial;

		velo_final = (pos_final - pos_inicial)/delta_t.count(); // count retorna em seg

		erro = (velo_retardada - velo_final)*100/velo_max;// m/s
		acumulador += erro;

		pwm = velo_retardada*100/velo_max;
		pwm += kp*erro + ki*acumulador + kd*(velo_final - velo_inicial);
		if(pwm > 100) pwm =100 ;
		if(pwm < -100) pwm = -100;

		int decimal = 1000;
		if(debug) file<<trunc(velo_final*decimal)/decimal<<"\t"<<trunc(roda.speed()*decimal)/decimal<<endl;
		//file<<trunc(erro*decimal)/decimal<<"\t"<<trunc(pwm*decimal)/decimal<<"\t"<<trunc(pos_final*decimal)/decimal<<"\t"<<trunc(velo_final*decimal)/decimal<<"\t"<<trunc(velo_retardada*decimal)/decimal<<endl;

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
		velo_retardada = velo_sp;
//		if(velo_retardada < (velo_sp)) velo_retardada += aceleracao*delta_t.count();
//		if(velo_retardada > (velo_sp)) velo_retardada -= aceleracao*delta_t.count();
		roda.run_direct();
	}
}
