#include "Controlador_robo.h"

Controlador_robo::Controlador_robo(){
	rodaE = new Controlador_motor(ev3dev::OUTPUT_A, 0.0408, 0.808, false,  "debug_lego_E.m");
	rodaD = new Controlador_motor(ev3dev::OUTPUT_B, 0.04058, 0.79, false,  "debug_lego_D.m");
}

void Controlador_robo::inicializar_threads_motor() {
	rodaE->inicializa_thread();
	rodaD->inicializa_thread();
}

void Controlador_robo::finalizar_threads_motor(){
	rodaE->finaliza_thread();
	rodaD->finaliza_thread();
}

void Controlador_robo::frente (double velo_sp){
	cout << "FRENTE" << endl;
	usleep (1.5*1000000);
	velo_sp_me = velo_sp;
	velo_sp_md = velo_sp;

	flag_aceleracao = 1;
}

void Controlador_robo::tras(double velo_sp){
	cout << "TRAS" << endl;
	usleep (1.5*1000000);
	velo_sp_me = velo_sp;
	velo_sp_md = velo_sp;

	flag_aceleracao = 2;
}

void Controlador_robo::parar () {
	cout << "PARAR" << endl;
	usleep (1.5*1000000);
	velo_sp_me = 0.0;
	velo_sp_md = 0.0;

	flag_aceleracao = 3;

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
	cout << "THREAD_ACELERACAO" << endl;
	while (thread_rodando) {
		/*  Controle de aceleração utilizando as variáveis:
			velo_sp_me		velo_sp_md
			Que serão atualizadas nos métodos: frente(), tras(), parar().
		 */
		switch(flag_aceleracao){
		case 0: break;
		case 1: //  ->frente(velo_sp);
			while(true){
				if (velo_retardada_me <= velo_sp_me)
					rodaE->set_velo(velo_retardada_me);
				if (velo_retardada_md <= velo_sp_md)
					rodaD->set_velo(velo_retardada_md);
				usleep(1000*delay);
				velo_retardada_me += aceleracao*(delay*0.001);
				velo_retardada_md += aceleracao*(delay*0.001);
				if (velo_retardada_me > velo_sp_me && velo_retardada_md > velo_sp_md)
					break;
			}
			cout << flag_aceleracao << endl;
			flag_aceleracao = 0;
			cout << flag_aceleracao << endl;
			break;

		case 2: //  ->tras(velo_sp);
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

		case 3: //  ->parar(0);
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
					velo_retardada_me -= aceleracao*(5*delay*(0.001));
					velo_retardada_md -= aceleracao*(5*delay*(0.001));

					if (velo_retardada_me == 0.0 && velo_retardada_md == 0.0)
						break;
				}

			}

			flag_aceleracao = 0;
			break;
		}
	}

}
