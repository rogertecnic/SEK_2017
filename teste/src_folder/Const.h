#ifndef CONST_H_
#define CONST_H_

#include <iostream>
#include <list>
#include <vector>
#include "ev3dev.h"

using namespace std;

enum status{ndStatus, em_frente, direita, esquerda};// usado na classe MApeamento
enum Cor{ndCor, preto, branco, vermelho, verde, azul, fora}; // cores possiveis
enum flag_aceleracao{ndAcel, linha_reta, parar, girar}; // usado dentro do Controlador_robo

struct RGB{ // utilizado na classe Sensor_cor_hsv
	int r = 0, g = 0, b = 0;
};

struct HSV{ // utilizado na classe Sensor_cor_hsv
	double h = 0, s = 0, v = 0;
};




// usado na classe MApeamento
typedef struct status_checkpoint {
	status checkpoint_vermelho = status::ndStatus;
	status checkpoint_verde = status::ndStatus;
	status checkpoint_azul = status::ndStatus;

}status_checkpoint;

// usado na classe MApeamento
typedef struct no_intersec {
	bool pre, pos;
	vector<double> posicao_pre_d;
	vector<double> posicao_pos_d;
	vector<double> posicao_pre_e;
	vector<double> posicao_pos_e;
}no_intersec;


#endif
