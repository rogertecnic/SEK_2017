#ifndef CONST_H_
#define CONST_H_

#include <iostream>
#include <list>
#include <vector>
#include "ev3dev.h"

using namespace std;

enum status{nda1, em_frente, direita, esquerda};
enum checkpoint{nada, vermelho, verde, azul};

typedef struct status_checkpoint {
	status checkpoint_vermelho = status::nda1;
	status checkpoint_verde = status::nda1;
	status checkpoint_azul = status::nda1;

}status_checkpoint;

typedef struct no_intersec {
	bool pre, pos;
	vector<double> posicao_pre_d;
	vector<double> posicao_pos_d;
	vector<double> posicao_pre_e;
	vector<double> posicao_pos_e;
}no_intersec;


#endif
