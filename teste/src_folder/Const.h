#ifndef CONST_H_
#define CONST_H_

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include "ev3dev.h"


using namespace std;

enum estados_arena{faixa, leu_fora, leu_nda, intersec, terminado, atencao}; //usado na classe Mapeamento
enum direcao{ndDirecao, frente, direita, esquerda, traz};// usado na classe Mapeamento
enum Cor{ndCor, preto, branco, vermelho, verde, azul, fora}; // cores possiveis
enum flag_aceleracao{ndAcel, linha_reta, parar, girar}; // usado dentro do Controlador_robo

struct RGB{ // utilizado na classe Sensor_cor_hsv
	int r = 0, g = 0, b = 0;
};

struct HSV{ // utilizado na classe Sensor_cor_hsv
	double h = 0, s = 0, v = 0;
};




// usado na classe MApeamento
struct direcao_checkpoint {
	direcao checkpoint_vermelho;
	direcao checkpoint_verde;
	direcao checkpoint_azul;
};



/*
 * posicao do boneco relativa a intersecao pre ou pos
 */
struct no_intersec {
	bool pre, pos;
	vector<double> posicao_pre_d;
	vector<double> posicao_pos_d;
	vector<double> posicao_pre_e;
	vector<double> posicao_pos_e;
};

const bool automapear_3_checkpoint = true;

/* == 1 se estiver indo do ponto de start para a rampa
 * == -1 se estiver indo da rampa para o ponto de start
 */
extern int sentido_navegacao;


/* Variaveis de mapeamento de intersecção e de bonecos*/
extern vector<no_intersec> no;
extern direcao_checkpoint cp;
const bool arena_pequi = false; // testar o robo na nossa arena e na arena oficial
#endif
