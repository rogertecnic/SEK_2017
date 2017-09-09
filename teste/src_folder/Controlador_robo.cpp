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
	/*
	velo_sp_me = velo_sp;
	velo_sp_md = velo_sp;
	 */
	this->pwm_sp = pwm_sp;
	estado = flag_aceleracao::linha_reta;
}


void Controlador_robo::parar () {
	/*
	velo_sp_me = 0.0;
	velo_sp_md = 0.0;
	 */
	pwm_sp = abs(pwm_sp);
	estado = flag_aceleracao::parar;

}

void Controlador_robo::girar(Direcao dir){
	/*
	parar();
	if(dir == Direcao::esquerda){
		rodaE->set_velo(-0.3);
		rodaD->set_velo(0.3);
		usleep(200*1000);
	}
	else{
		rodaE->set_velo(0.3);
		rodaD->set_velo(-0.3);
		usleep(200*1000); //millis*1000
	}
	parar();
	 */

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
		/*  Controle de aceleração utilizando as variáveis:
			velo_sp_me		velo_sp_md
			Que serão atualizadas nos métodos: frente(), tras(), parar().
		 */

		switch(estado){
		case flag_aceleracao::linha_reta :
			//while(true){
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

			/*
				if (velo_retardada_me <= velo_sp_me)
					rodaE->set_velo(velo_retardada_me);
				if (velo_retardada_md <= velo_sp_md)
					rodaD->set_velo(velo_retardada_md);
				usleep(1000*delay);
				velo_retardada_me += aceleracao*(delay*0.001);
				velo_retardada_md += aceleracao*(delay*0.001);
				if (velo_retardada_me > velo_sp_me && velo_retardada_md > velo_sp_md)
					break;
			 */
			//}
			//estado = nda;
			break;

		case 5 : //  ->tras(velo_sp);
			//			erro = rodaE->position()*fator_croda - rodaD->position();
			//			pid = kp*erro + kd*(erro-erro_anterior);
			//
			//			pwm = pwm_retardada + pid;
			//			if(pwm > 100) pwm = 100;
			//			if(pwm < -100) pwm = -100;
			//			rodaE->set_duty_cycle_sp(pwm);
			//
			//			pwm = pwm_retardada - pid;
			//			if(pwm > 100) pwm = 100;
			//			if(pwm < -100) pwm = -100;
			//			rodaD->set_duty_cycle_sp(pwm);
			//			usleep(1000*delay);
			//
			//			rodaE->run_direct();
			//			rodaD->run_direct();
			//
			//			if(pwm_retardada > pwm_sp)
			//				pwm_retardada -= aceleracao*delay/1000; // converte para miliseg
			//			cout<<pwm_retardada<<";"<<pwm_sp<<endl;


			//while(true){
			//				if (velo_retardada_me >= velo_sp_me)
			//					rodaE->set_velo(velo_retardada_me);
			//				if (velo_retardada_md >= velo_sp_md)
			//					rodaD->set_velo(velo_retardada_md);
			//				usleep(1000*delay);
			//				velo_retardada_me -= aceleracao*(delay*(0.001));
			//				velo_retardada_md -= aceleracao*(delay*(0.001));
			//				if (velo_retardada_me < velo_sp_me && velo_retardada_md < velo_sp_md)
			//break;
			//}

			//flag_aceleracao = 0;
			break;

		case flag_aceleracao::parar: //  ->parar(0);
			//while(true) {
			//while(true){
			//					if(velo_retardada_me > velo_sp_me)
			//						rodaE->set_velo(velo_retardada_me);
			//					if(velo_retardada_md > velo_sp_md)
			//						rodaD->set_velo(velo_retardada_md);
			//					usleep(1000*(delay*5));
			//
			//					if(velo_retardada_me < aceleracao*0.01   &&   velo_retardada_me > -aceleracao*0.01 ){
			//						rodaE->set_velo(0);
			//						velo_retardada_me = 0;
			//						usleep(1000*(delay*5));
			//					}
			//					if(velo_retardada_md < aceleracao*0.01  &&   velo_retardada_md > -aceleracao*0.01 ){
			//						rodaD->set_velo(0);
			//						velo_retardada_md = 0.0;
			//						usleep(1000*(delay*5));
			//					}
			//					velo_retardada_me -= aceleracao*(10*delay*(0.001));
			//					velo_retardada_md -= aceleracao*(10*delay*(0.001));
			//
			//					if (velo_retardada_me == 0.0 && velo_retardada_md == 0.0)
			//break;
			//}

			//}
			rodaE->stop();
			rodaD->stop();
			break;
		}
	}

}
