#include "Controlador_robo.h"

Controlador_robo::Controlador_robo(){
	rodaE = new Controlador_motor(ev3dev::OUTPUT_A, 0.0408, 0.808, false,  "debug_lego_E.m");
	rodaD = new Controlador_motor(ev3dev::OUTPUT_B, 0.04058, 0.79, false,  "debug_lego_D.m");
}

void Controlador_robo::inicializar_thread_motor() {
	rodaE->inicializa_thread();
	rodaD->inicializa_thread();
}

void Controlador_robo::finalizar_thread_motor(){
	rodaE->finaliza_thread();
	rodaD->finaliza_thread();
}

void Controlador_robo::frente (double velocidade, double aceleracao){
	for (double i = 0; i <= velocidade; i=i+aceleracao){
		rodaE->set_velo(i);
		rodaD->set_velo(i);
	}
}

void Controlador_robo::tras(){
	rodaE->set_velo(-0.3);
	rodaD->set_velo(-0.3);

}

void Controlador_robo::parar () {
	rodaE->set_velo(0);
	rodaD->set_velo(0);

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
