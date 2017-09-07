#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include "ev3dev.h"
#include "Controlador_motor.h"
#include <thread>

#define delay 20

using namespace std;

enum Direcao{esquerda, direita};

class Controlador_robo {
public:
	Controlador_robo(double );
	void inicializar_threads_motor();
	void finalizar_threads_motor();
	void frente(double, );
	void tras(double);
	void parar ();
	void girar(Direcao);
private:
	double velo_retardada = 0.0;
	double aceleracao;
	Controlador_motor *rodaE;
	Controlador_motor *rodaD;
};



#endif
