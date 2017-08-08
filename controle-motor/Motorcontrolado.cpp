/*
 * Motorcontrolado.cpp
 *
 *  Created on: 7 de ago de 2017
 *      Author: pcsek
 */

#include "Motorcontrolado.h"
#include "ev3dev.h"

Motor_controlado::Motor_controlado(string porta)
:motor(ev3dev::large_motor(porta)),
 thread_pid(std::thread(&Motor_controlado::run_controle, this))
{
	motor.reset();
}

int Motor_controlado::girar_indefinido(Sentido sentido){
std::thread();
	return 0;
}



void Motor_controlado::run_controle(){
	double pwm=0;
	double velo_real;
	double velo_sp_tmp;
	double e=0;
	double eant = 0;
	double kp = 0.11;
	double kd = 1;
	int i=1;


	//-------------CONTROLADOR-THREAD
	while(true){
		if(!parado){ // ----ACELERACAO
			if(sentido_giro){ // FRENTE
				velo_sp_tmp = (velo_sp_tmp<velo_sp ? velo_sp_tmp + acc_sp*0.05 : velo_sp);
			}else{ // TRAZ
				velo_sp_tmp = (velo_sp_tmp>velo_sp ? velo_sp_tmp - acc_sp*0.05 : velo_sp);
			}
		}
		else {// ----DESACELERACAO
			if(sentido_giro){ // FRENTE
				velo_sp_tmp = (velo_sp_tmp>0 ? velo_sp_tmp - acc_sp*0.05 : 0);
			}else{ // TRAZ
				velo_sp_tmp = (velo_sp_tmp<0 ? velo_sp_tmp + acc_sp*0.05 : 0);
			}
		}

		usleep(5*1000); // pausa por 5 milis
		velo_real = motor.speed();
		e =  (velo_sp_tmp-velo_real)/velo_max*100; // erro positivo saida baixa, aumentala
		pwm = pwm + kp*e + kd*(e - eant);
		if(pwm>100) pwm = 100;
		if(pwm<-100) pwm = -100;
		motor.set_duty_cycle_sp(pwm);
		eant = e;

		cout<<pwm<<";"<<motor.speed()<<endl;
	}
}
