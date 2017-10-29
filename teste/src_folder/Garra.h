#ifndef GARRA_H_
#define GARRA_H_

#include "Const.h"

using namespace std;

// a garra abre no sentido negativo, o limite eh -42
class Garra {
public:
	Garra(string motor_port, int graus_abertura_do_motor);
	void abrir();
	void fechar();

private:
	bool garra_aberta = false;
	ev3dev::large_motor garra;
	int graus_abertura_do_motor = 0;
};



#endif /* GARRA_H_ */
