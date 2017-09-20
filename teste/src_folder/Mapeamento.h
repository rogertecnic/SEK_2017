#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_
#include <iostream>
#include <list>
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

typedef union posicao_bonecos{
	int interseccao = 0;
	double distancia = 0.0; // Dado pelo tempo que levou para diminuir a distancia entre os sensores
} posicao_bonecos;



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

	enum status{nda1, em_frente, direita, esquerda};
	enum checkpoint{nda, vermelho, verde, azul};

	int sentido_navegacao = 0;
	double posicao_inicial = 0;

	int delay = 0;
	bool confirmacao_status = false;

	/*Flags mapeamento intersecção*/
	checkpoint cor_atual = checkpoint::nda;
	status status_atual = status::nda1;

	/*Status de cada intersecção*/
	status checkpoint_vermelho = status::nda1;
	status checkpoint_verde = status::nda1;
	status checkpoint_azul = status::nda1;



	/*Mapeamento dos bonecos*/
	bool intersec = false;
	double distancia = 0;

	bool thread_rodando = false;
	thread mapeamento_bonecoD;
	thread mapeamento_bonecoE;
	void loop_mapeamento_bonecoD(Controlador_robo *, Ultrassom_nxt *);
	void loop_mapeamento_bonecoE(Controlador_robo *, Ultrassom_nxt *);

	/*Navegacao
	bool thread_rodando_naveg = false;
	thread navegacao;
	void loop_navegacao();
	*/


};


#endif
