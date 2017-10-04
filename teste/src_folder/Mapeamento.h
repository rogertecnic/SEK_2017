#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include "Controlador_robo.h"
#include "Sensor_cor.h"
#include "Arquivos_mapeamento.h"
#include "Ultrassom_nxt.h"


#define virar_direita(i) robo->girar(-90*i) //considerando sentido horário
#define virar_esquerda(i) robo->girar(90*i) //ou 270
#define distancia_boneco 5 //MODIFICAR DEPOIS
#define delay_f 5 //Usado na detecção do final da cidade


using namespace std;


class Mapeamento {
public:
	void mapeamento(Controlador_robo *, Sensor_cor *);
	bool mapeamento_intersec(Controlador_robo *, Sensor_cor *);
	void caminho_certo(Controlador_robo *, Sensor_cor *);
	void ajeita_quadrado(Controlador_robo *);
	bool fim_da_cidade(Controlador_robo *, Sensor_cor *);

	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();

	bool inicializar_thread_navegacao();
	bool finalizar_thread_navegacao();

private:
	bool map_boneco_inicio = false;

	/*Arquivo para salvar as informações de mapeamento*/
	Arquivos_mapeamento *arq_map;


	/* == 1 se estiver indo do ponto de start para a rampa
	 * == -1 se estiver indo da rampa para o ponto de start
	 */
	int sentido_navegacao = 0;

	/*Flags mapeamento intersecção*/
	Cor cor_atual = Cor::ndCor;
	status status_atual = status::ndStatus;

	/*Status de cada intersecção*/
	status_checkpoint cp;

	/*Variável de controle de posicao na chegada de um quadrado(deadend/intersec)*/
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

	list<no_intersec> no;
	list<no_intersec>::iterator it_no_atual = no.begin();
	list<no_intersec>::iterator it_no_anterior = no.begin();

	/*Controlador de posição do vector em loop_mapeamento_boneco*/
	unsigned j;

	bool interseccao;
	bool leu_boneco= false;
	double posicao_intersec = 0.0;

};


#endif
