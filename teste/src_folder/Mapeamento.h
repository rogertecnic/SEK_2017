#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include "Controlador_robo.h"
#include "Sensor_cor_hsv.h"
#include "Arquivos_mapeamento.h"
#include "Ultrassom_nxt.h"


#define virar_direita(i) robo->girar(-90*i)
#define virar_esquerda(i) robo->girar(90*i)


#define distancia_boneco 5 //MODIFICAR DEPOIS
#define delay_f 5 //Usado na detecção do final da cidade


using namespace std;


class Mapeamento {
public:
	Mapeamento(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);

	/*
	 *  Método de controle da jornada do robo pela "cidade"
	 *  segura a execucao ate terminar a cidade
	 */
	void mapear();

	/* Métodos de controle das Threads de mapeamento dos bonecos (sensor ultrassom)*/
	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();




private:

	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;

	/* Método de mapemento das direções das intersecções*/
	void mapeamento_intersec();


	/* Método utilizado em: mapeamento_intersec
	 * Se o robô passar por uma interssecção onde a mesma já foi previamente mapeada
	 * ele segue pelo "caminho certo"
	 */
	void caminho_certo();


	/* Método utilizado em: mapeamento_intersec
	 * Determina se foi encontrado o final da cidade
	 * representado por uma rampa com uma sequência de cores
	 */
	bool fim_da_cidade();


	/* Método para definir se a cor lida está entre as cores:
	 * Vermelho, azul, verde e preto
	 */
	bool colorido(string lado);

	/**/
	bool cor_ja_vista(string lado);

	/* Variável de controle de start do mapeamento dos bonecos */
	bool map_boneco_inicio = false;

	/* Arquivo para salvar as informações de mapeamento*/
	Arquivos_mapeamento *arq_map;


	/* == 1 se estiver indo do ponto de start para a rampa
	 * == -1 se estiver indo da rampa para o ponto de start
	 */
	int sentido_navegacao = 0;

	/* Flags mapeamento intersecção*/
	Cor cor_atual = Cor::ndCor;
	direcao direcao_atual = direcao::ndStatus;

	/* Status de cada intersecção*/
	status_checkpoint cp = {direcao::ndStatus, direcao::ndStatus, direcao::ndStatus};

	/* Variável de controle de posicao na chegada de um quadrado(deadend/intersec)*/
	//double posicao_inicialt = 0;

	/* Variável de controle de correção de rota*/
	int delay = 0;

	/* Variável de controle se a intersecção já foi mapeada*/
	bool confirmacao_status = false;

	bool dead_end = false;


	/*************************Alinhamento******************************/

	int count_nda = 0;

	int cor_E = 0, cor_D = 0;
	double 	dist = 0, ang_robo = 0, posicao_inicial = 0, posicao_final = 0;

	estados estd = estados::faixa;


	/********************Mapeamento dos bonecos************************/

	/* Variável de controle da thread*/
	bool thread_rodando_bonecos = false;

	/* Uma thread para cada sensor ultrassom: esquerdo e direito*/
	thread mapeamento_bonecoE;
	thread mapeamento_bonecoD;

	/* Métodos das threads*/
	void loop_mapeamento_bonecoE();
	void loop_mapeamento_bonecoD();

	/* Lista de Vectors e seus iteradores
	 * Os Vectors armazenam as posições dos bonecos em relação à intersecção
	 * Cada nó trata-se de uma intersecção
	 * Cada intersecção deve conter as posições dos bonecos antes e depois dela (se houver)
	 */
	list<no_intersec> no;
	list<no_intersec>::iterator it_no_atual = no.begin();
	list<no_intersec>::iterator it_no_anterior = no.begin();

	/* Controlador de posição do vector em loop_mapeamento_boneco*/
	unsigned j = 0;

	/* Demarca se o robo está dentro de uma interseccao ou não*/
	bool interseccao = false;

	/* Controla se houve leitura de bonecos entre uma intersecção e outra*/
	bool leu_boneco = false;

	/* Distância total entre uma intersecção e outra*/
	double posicao_intersec = 0.0;

};


#endif
