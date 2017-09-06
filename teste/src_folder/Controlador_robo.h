#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include "ev3dev.h"
#include "Controlador_motor.h"

using namespace std;

enum Direcao{esquerda, direita};

Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, true,  "debug_lego_E.m");
Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");

class Controlador_robo {
public:
	void inicializar_thread();
	void finalizar_thread();
	void frente();
	void tras();
	void parar ();
	void girar(Direcao);

private:

};



#endif
