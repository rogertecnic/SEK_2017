#include "Controlador_robo.h"

Controlador_robo::Controlador_robo(){
	rodaE = new Controlador_motor(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, false,  "debug_lego_E.m");
	rodaD = new Controlador_motor(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, false,  "debug_lego_D.m");
}

void Controlador_robo::inicializar_thread() {
	rodaE->inicializa_thread();
	rodaD->inicializa_thread();
}

void Controlador_robo::finalizar_thread(){
	rodaE->finaliza_thread();
	rodaD->finaliza_thread();
}

void Controlador_robo::frente (){
	rodaE->set_velo(0.3);
	rodaD->set_velo(0.3);

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
		//rodaE.set_velo(-0.3);
		//rodaD.set_velo(0.3);
		//usleep(200*1000);
	}
	else{
		//rodaE.set_velo(0.3);
		//rodaD.set_velo(-0.3);
		//usleep(200*1000); //millis*1000
	}
	parar();

}

