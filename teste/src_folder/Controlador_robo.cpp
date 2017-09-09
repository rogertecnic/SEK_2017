#include "Controlador_robo.h"


Controlador_robo::Controlador_robo(double fator_croda)
:fator_croda(fator_croda)
{
	rodaE = new ev3dev::large_motor(ev3dev::OUTPUT_A);
	rodaD = new ev3dev::large_motor(ev3dev::OUTPUT_B);

	rodaE->reset();
	rodaD->reset();

	rodaE->set_stop_action("hold");
	rodaD->set_stop_action("hold");

}


void Controlador_robo::andar (int pwm_sp){
	this->pwm_sp = pwm_sp;
	estado = flag_aceleracao::linha_reta;
}


void Controlador_robo::parar () {
	pwm_sp = abs(pwm_sp);
	estado = flag_aceleracao::parar;

}


void Controlador_robo::girar(Direcao dir){
	//TODO

}


bool Controlador_robo::inicializar_thread_aceleracao(){
	thread_controle_velocidade = thread(&Controlador_robo::loop_controle_aceleracao, this);
	thread_controle_velocidade.detach();
	usleep(1000*100);
	return thread_rodando;
}


bool Controlador_robo::finalizar_thread_aceleracao(){
	if(thread_rodando) thread_rodando = false;
	usleep(1000*100);
	return true;
}


void Controlador_robo::loop_controle_aceleracao(){
	thread_rodando = true;

	while (thread_rodando) {
		switch(estado){
		case flag_aceleracao::linha_reta :
			erro = rodaE->position()*fator_croda - rodaD->position();
			pid = kp*erro + kd*(erro-erro_anterior);

			pwm = pwm_retardada - pid;
			if(pwm > 100) pwm = 100;
			if(pwm < -100) pwm = -100;
			rodaE->set_duty_cycle_sp(pwm);

			pwm = pwm_retardada + pid;
			if(pwm > 100) pwm = 100;
			if(pwm < -100) pwm = -100;
			rodaD->set_duty_cycle_sp(pwm);
			usleep(1000*5);

			rodaE->run_direct();
			rodaD->run_direct();

			if(pwm_retardada < pwm_sp-aceleracao*delay/1000)
				pwm_retardada += aceleracao*delay/1000;
			if(pwm_retardada > pwm_sp+aceleracao*delay/1000)
				pwm_retardada -= aceleracao*delay/1000;

			cout<<pwm_retardada<<";"<<pwm_sp<<endl;

			break;

		case flag_aceleracao::parar :
			rodaE->stop();
			rodaD->stop();

			pwm_sp = 0;
			pwm_retardada = 0.0;
			pwm = 0;

			break;
		}
	}

}
