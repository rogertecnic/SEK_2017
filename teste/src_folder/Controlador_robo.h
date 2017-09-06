#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include "ev3dev.h"
#include "Controlador_motor.h"
#include <thread>

using namespace std;

enum Direcao{esquerda, direita};

class Controlador_robo {
public:
	Controlador_robo();
	void inicializar_thread_motor();
	void finalizar_thread_motor();
	void frente(double, double);
	void tras();
	void parar ();
	void girar(Direcao);
private:
	Controlador_motor *rodaE;
	Controlador_motor *rodaD;
};



#endif
