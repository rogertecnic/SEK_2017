#ifndef CONTROLADOR_ROBO_H_
#define CONTROLADOR_ROBO_H_
#include <iostream>
#include "ev3dev.h"
#include "Controlador_motor.h"
#include "Sensor_cor.h"

using namespace std;

enum Direcao{esquerda, direita};

class Controlador_robo {
public:
	void frente();
	void tras();
	void girar(Direcao);

private:

};



#endif
