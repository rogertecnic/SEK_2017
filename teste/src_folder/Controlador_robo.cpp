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


void Controlador_robo::frente (int pwm_sp){
	/*
	velo_sp_me = velo_sp;
	velo_sp_md = velo_sp;
	*/

	estado = frente;
}

void Controlador_robo::tras(int pwm_sp){
	/*
	velo_sp_me = velo_sp;
	velo_sp_md = velo_sp;
	*/

	estado = tras;
}

void Controlador_robo::parar () {
	/*
	velo_sp_me = 0.0;
	velo_sp_md = 0.0;
	*/

	estado = parar;

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
		case flag_aceleracao::nda : break;
		case flag_aceleracao::frente :
			while(true){
				erro = rodaE->position()*fator_croda - rodaD->position();
				pid = kp*erro + kd*(erro-erro_anterior);



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
			}
			estado = nda;
			break;

		case flag_aceleracao::tras : //  ->tras(velo_sp);
			while(true){
				if (velo_retardada_me >= velo_sp_me)
					rodaE->set_velo(velo_retardada_me);
				if (velo_retardada_md >= velo_sp_md)
					rodaD->set_velo(velo_retardada_md);
				usleep(1000*delay);
				velo_retardada_me -= aceleracao*(delay*(0.001));
				velo_retardada_md -= aceleracao*(delay*(0.001));
				if (velo_retardada_me < velo_sp_me && velo_retardada_md < velo_sp_md)
					break;
			}

			flag_aceleracao = 0;
			break;

		case flag_aceleracao::parar: //  ->parar(0);
			while(true) {
				while(true){
					if(velo_retardada_me > velo_sp_me)
						rodaE->set_velo(velo_retardada_me);
					if(velo_retardada_md > velo_sp_md)
						rodaD->set_velo(velo_retardada_md);
					usleep(1000*(delay*5));

					if(velo_retardada_me < aceleracao*0.01   &&   velo_retardada_me > -aceleracao*0.01 ){
						rodaE->set_velo(0);
						velo_retardada_me = 0;
						usleep(1000*(delay*5));
					}
					if(velo_retardada_md < aceleracao*0.01  &&   velo_retardada_md > -aceleracao*0.01 ){
						rodaD->set_velo(0);
						velo_retardada_md = 0.0;
						usleep(1000*(delay*5));
					}
					velo_retardada_me -= aceleracao*(10*delay*(0.001));
					velo_retardada_md -= aceleracao*(10*delay*(0.001));

					if (velo_retardada_me == 0.0 && velo_retardada_md == 0.0)
						break;
				}

			}

			flag_aceleracao = 0;
			break;
		}
	}

}
