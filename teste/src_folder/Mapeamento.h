//TODO fazer fim_cidade (aguardando e mail do chair sobre local da marcacao na rampa)
//TODO (testado uma vez)tirar verificacao do 3 checkpoint de forma que fique facil voltar
/*TODO (TESTAR) verificar todos os lugares onde ele chama o metodo realinhar,
 * em algum lugar o robo viu cores diferentes com os sensores e entrou em um loop onde
 * ele ficava somente realinhando
 */

#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include "Controlador_robo.h"
#include "Sensor_cor_hsv.h"
#include "Arquivos_mapeamento.h"
#include  "Ultrassom_nxt.h"
#include "Const.h"

//FIXME distancia_boneco e delay_f deixar aqui ou levar pra outro lugar?
#define distancia_boneco 5 //MODIFICAR DEPOIS
#define delay_f 5 //Usado na detecção do final da cidade


using namespace std;


class Mapeamento {
public:
	Mapeamento(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);

	// inicia rotina de mapeamento
	void mapear();


private:
	// objetos utilizadospara controlar o hardware
	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;

	//FIXME deletar variaveis abaixo, criadas somente pra simular o fim_da_cidade
	int iterador_fim_cidade = 0;

	/*********************************************************************************************/
	/*************************VARIAVEIS DO MAPEAMENTO DE BONECO***********************************/
	/*********************************************************************************************/
	/* == 1 se estiver indo do ponto de start para a rampa
	 * == -1 se estiver indo da rampa para o ponto de start
	 */

	//Variável de controle de posicao na chegada de um quadrado(deadend/intersec)
	double posicao_inicialt = 0;

	/* Variável de controle de correção de rota*/
	int delay = 0;

	/* Variável de controle da thread*/
	bool thread_rodando_bonecos = false;

	/* Uma thread para cada sensor ultrassom: esquerdo e direito*/
	thread mapeamento_bonecoE;
	thread mapeamento_bonecoD;

	//Variável de controle de start do mapeamento dos bonecos
	bool map_boneco_inicio = false;

	//Arquivo para salvar as informações de mapeamento
	Arquivos_mapeamento *arq_map;

	/* Lista de Vectors e seus iteradores
	 * Os Vectors armazenam as posições dos bonecos em relação à intersecção
	 * Cada nó trata-se de uma intersecção
	 * Cada intersecção deve conter as posições dos bonecos antes e depois dela (se houver)
	 */
	//list<no_intersec> no;
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

	/*-------------------------------------------------------------------------------------------*/
	/*------------------------FIM VARIAVEIS DO MAPEAMENTO DE BONECO------------------------------*/
	/*-------------------------------------------------------------------------------------------*/


	/*********************************************************************************************/
	/***************************METODOS DO MAPEAMENTO DE BONECO***********************************/
	/*********************************************************************************************/
	//Métodos de controle das Threads de mapeamento dos bonecos (sensor ultrassom)
	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();

	//Métodos das rotinas das threads
	void loop_mapeamento_bonecoE();
	void loop_mapeamento_bonecoD();

	/*-------------------------------------------------------------------------------------------*/
	/*-------------------------FIM METODOS DO MAPEAMENTO DE BONECO-------------------------------*/
	/*-------------------------------------------------------------------------------------------*/


	/*********************************************************************************************/
	/**************************VARIAVEIS DO MAPEAMENTO DE DIRECAO*********************************/
	/*********************************************************************************************/
	// cor que o referente a intersecao que o robo acabou de sair
	Cor cor_atual = Cor::ndCor;

	// direcao referente a intersecao que o robo acabou de sair
	direcao direcao_atual = direcao::ndDirecao;

	//	intersecao atual ja foi mapeada? se nao,
	//  quando encontrar uma nova intersecao mapear a atual
	bool confirmacao_status = false;

	// robo esta voltando de um dead-end?
	bool dead_end = false;

	//flags para salvar ultima leitura da cor quando necessario
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;

	// flag que controla o comportamento do robo de acordo com onde ele esta
	estados_arena estd = estados_arena::faixa;

	// contador de quantas cores ja mapeou
	int qnt_cores_mapeadas = 0;

	/*-------------------------------------------------------------------------------------------*/
	/*-------------------------FIMVARIAVEIS DO MAPEAMENTO DEDIRECAO------------------------------*/
	/*-------------------------------------------------------------------------------------------*/


	/*********************************************************************************************/
	/***************************METODOS DO MAPEAMENTO DE DIRECAO**********************************/
	/*********************************************************************************************/

	// quando o robo estiver saindo de um lado ele volta e realinha
	void realinha(direcao lado_saindo);
	// quando o robo chegar em uma intersec (vermelho, verde, azul ou preto)
	void intersec();

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


	/*verifica se a cor que o robo esta sobre quando chamar o metodo ja foi mapeada*/
	bool cor_ja_mapeada();
	/*-------------------------------------------------------------------------------------------*/
	/*-------------------------FIM METODOS DO MAPEAMENTO DE DIRECAO------------------------------*/
	/*-------------------------------------------------------------------------------------------*/
};


#endif
