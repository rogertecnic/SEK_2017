#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_
#include <iostream>
#include <stack>
#include <string>
#include "ev3dev.h"
#include "Controlador_robo.h"
#include "Sensor_cor.h"
#include "Ultra_Som.h"

using namespace std;

enum status{em_frente, direita, esquerda};

class Mapeamento {
public:
	void mapeamento_cores();

private:
	stack<status> dfs;
	status status_atual;
	string checkpoint;


};



#endif
