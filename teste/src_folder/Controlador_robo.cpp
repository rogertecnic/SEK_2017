#include "Controlador_robo.h"

Controlador_robo::Controlador_robo(double aceleracao)
:aceleracao(aceleracao)
{
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
	while(velo_retardada <= velo_sp){
		rodaE->set_velo(velo_retardada);
		rodaD->set_velo(velo_retardada);
		usleep(1000*delay);
		velo_retardada += aceleracao*(aceleracao*(10^-3));
	}

}

void Controlador_robo::tras(double velo_sp){
	while(velo_retardada >= velo_sp){
		rodaE->set_velo(velo_retardada);
		rodaD->set_velo(velo_retardada);
		usleep(1000*delay);
		velo_retardada -= aceleracao*(aceleracao*(10^-3));
	}

}

void Controlador_robo::parar () {
	double velo_sp = 0.0;
	while(velo_retardada > velo_sp){
		rodaE->set_velo(velo_retardada);
		rodaD->set_velo(velo_retardada);
		usleep(1000*(delay*5));
		if(velo_retardada < aceleracao*0.01   &&   velo_retardada > -aceleracao*0.01 ){
			rodaE->set_velo(0);
			rodaD->set_velo(0);
			velo_retardada = 0;
			usleep(1000*(delay*5));
		}
		velo_retardada -= aceleracao*(aceleracao*(10^-3));
	}

}

void Controlador_robo::girar(Direcao dir){
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

}
