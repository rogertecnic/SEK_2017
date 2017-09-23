#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include "ev3dev.h"
#include "Controlador_robo.h"
#include "Sensor_cor.h"
#include "Ultra_Som.h"
#include "Arquivos_mapeamento.h"
#include "Ultrassom_nxt.h"


#define delay_f 5
#define virar_direita(i) robo->girar(-90*i) //considerando sentido horário
#define virar_esquerda(i) robo->girar(90*i) //ou 270


using namespace std;


typedef struct no_intersec {
	list<double> posicao_pre_d;
	list<double> posicao_pos_d;
	list<double> posicao_pre_e;
	list<double> posicao_pos_e;
	list<double>::iterator it1, it2;
}no_intersec;

/*
typedef union posicao_bonecos{
	int interseccao = 0;
	double distancia = 0.0; // Dado pelo tempo que levou para diminuir a distancia entre os sensores
} posicao_bonecos;
*/



class Mapeamento {
public:
	void mapeamento(Controlador_robo *, Sensor_cor *);
	bool mapeamento_intersec(Controlador_robo *, Sensor_cor *);
	void caminho_certo(Controlador_robo *, Sensor_cor *);
	void ajeita_quadrado(Controlador_robo *);

	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();

	bool inicializar_thread_navegacao();
	bool finalizar_thread_navegacao();

private:
	bool map;

	/*Arquivo para salvar as informações de mapeamento*/
	Arquivos_mapeamento *arquivo;

	/* == 1 se estiver indo do ponto de start para a rampa
	 * == -1 se estiver indo da rampa para o ponto de start
	 */
	int sentido_navegacao = 0;


	/*	Checkpoint faz o controle de qual cor estamos avaliando no momento
	 * Status indica a direção que determinada cor da intersecção indica
	 */
	enum checkpoint{nda, vermelho, verde, azul};
	enum status{nda1, em_frente, direita, esquerda};

	/*Flags mapeamento intersecção*/
	checkpoint cor_atual = checkpoint::nda;
	status status_atual = status::nda1;

	/*Status de cada intersecção*/
	status checkpoint_vermelho = status::nda1;
	status checkpoint_verde = status::nda1;
	status checkpoint_azul = status::nda1;

	/*Variável de controle de posicao na chegada de um quadrado(deadend/intersec*/
	double posicao_inicial = 0;

	/*Variável de controle de correção de rota*/
	int delay = 0;

	/*Variável de controle se a intersecção já foi mapeada*/
	bool confirmacao_status = false;


	/*Mapeamento dos bonecos*/
	bool thread_rodando = false;
	thread mapeamento_bonecoD;
	thread mapeamento_bonecoE;
	void loop_mapeamento_bonecoD(Controlador_robo *, Ultrassom_nxt *);
	void loop_mapeamento_bonecoE(Controlador_robo *, Ultrassom_nxt *);

	//no_intersec no[MAX_intersec];
	list<no_intersec> no;
	list<no_intersec>::iterator it1 = no.begin();
	list<no_intersec>::iterator it2 = no.begin();
	list<double>::iterator it3;
	int contador = 0;

	bool interseccao = true;
	double posicao_bonecos = 0.0;
	double posicao_intersec = 0.0;

};


#endif
