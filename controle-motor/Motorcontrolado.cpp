/*
 * Motorcontrolado.cpp
 *
 *  Created on: 7 de ago de 2017
 *      Author: pcsek
 */

#include "Motorcontrolado.h"
#include "ev3dev.h"

Motor_controlado::Motor_controlado(string porta)
:motor(ev3dev::large_motor(porta))
{
	motor.reset();
}

Motor_controlado::~Motor_controlado() {
}

void Motor_controlado::run_velo_max(){
	double pwm;
	double velo_real;
	double velo_sp_tmp;
	double e=0;
	double eant = 0;
	double kp = 0.11;
	double kd = 1;
	int i=1;

	if(acc_sp == 0) pwm = velo_sp/velo_max*100;
	else{
		pwm = 0;
	}

	motor.set_duty_cycle_sp(pwm);
	motor.run_direct();

	//-------------CONTROLADOR-THREAD

	while(!ev3dev::button::enter.process()){
		if(acc_sp == 0)
			velo_sp_tmp = velo_sp;
		else if(velo_sp_tmp< velo_sp){
			velo_sp_tmp = acc_sp*i*0.005;
			if(velo_sp_tmp> velo_sp) velo_sp_tmp = velo_sp;
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
		i++;
		switch(i){
		case 500:
			velo_sp = 360;
			break;
		case 1000:
			velo_sp = 540;
			break;
		}
	}
}
