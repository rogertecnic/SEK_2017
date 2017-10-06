#ifndef CONST_H_
#define CONST_H_

#include <iostream>
#include <list>
#include <vector>
#include "ev3dev.h"

using namespace std;

enum estados{faixa, leu_nda, leu_fora, intersec, terminado}; //usado na classe Mapeamento
enum direcao{ndStatus, frente, direita, esquerda, traz};// usado na classe Mapeamento
enum Cor{ndCor, preto, branco, vermelho, verde, azul, fora}; // cores possiveis
enum flag_aceleracao{ndAcel, linha_reta, parar, girar}; // usado dentro do Controlador_robo

struct RGB{ // utilizado na classe Sensor_cor_hsv
	int r = 0, g = 0, b = 0;
};

struct HSV{ // utilizado na classe Sensor_cor_hsv
	double h = 0, s = 0, v = 0;
};




// usado na classe MApeamento
struct status_checkpoint {
	direcao checkpoint_vermelho;
	direcao checkpoint_verde;
	direcao checkpoint_azul;

};



// usado na classe MApeamento
struct no_intersec {
	bool pre, pos;
	vector<double> posicao_pre_d;
	vector<double> posicao_pos_d;
	vector<double> posicao_pre_e;
	vector<double> posicao_pos_e;
};


#endif
