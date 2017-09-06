#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include "ev3dev.h"
#include "Controlador_motor.h"

using namespace std;

enum Direcao{esquerda, direita};

class Controlador_robo {
public:
	Controlador_robo();
	void inicializar_thread();
	void finalizar_thread();
	void frente();
	void tras();
	void parar ();
	void girar(Direcao);
private:
	Controlador_motor *rodaE;
	Controlador_motor *rodaD;

};



#endif
