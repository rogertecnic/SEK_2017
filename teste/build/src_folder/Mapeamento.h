#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include "Controlador_robo.h"
#include "Sensor_cor_hsv.h"
#include "Arquivos_mapeamento.h"
#include "Ultrassom_nxt.h"
#include "Const.h"


#define distancia_boneco 18 //MODIFICAR DEPOIS

using namespace std;


class Mapeamento {
public:
	Mapeamento(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);
	void mapear();


private:

	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;


	int iterador_fim_cidade = 0;

	/********************************* MAPEAMENTO DE BONECOS***********************************/

	/* Variável de controle de posição de chegada/saída de um quadrado(deadend/intersec) */
	double posicao_saiu_intersec = 0;
	double posicao_chegou_intersec = 0;

	/* Variável de controle de correção de rota*/
	int delay = 0;

	/* Variável de controle da thread*/
	bool thread_rodando_bonecos = false;

	/* Uma thread para cada sensor ultrassom: esquerdo e direito*/
	thread mapeamento_bonecoE;
	thread mapeamento_bonecoD;

	//Arquivo para salvar as informações de mapeamento
	Arquivos_mapeamento *arq_map;

	/* Lista de Vectors e seus iteradores
	 * Os Vectors armazenam as posições dos bonecos em relação à intersecção
	 * Cada nó trata-se de uma intersecção
	 * Cada intersecção deve conter as posições dos bonecos antes e depois dela (se houver)
	 */
	//list<no_intersec>::iterator it_no_atual = no.begin();
	//vector<no_intersec>::iterator it_no_anterior = no.begin();
	//no_intersec aux;
	int it_no = 0;

	/* Demarca se o robo está dentro de uma interseccao ou não*/
	bool interseccao = false;

	/* Controla se houve leitura de bonecos entre uma intersecção e outra*/
	bool leu_boneco = false;

	/* Distância total entre uma intersecção e outra*/
	double posicao_intersec = 0.0;

	/* Métodos de controle das Threads de mapeamento dos bonecos (sensor ultrassom) */
	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();

	/* Métodos das rotinas das threads */
	void loop_mapeamento_bonecoE();
	void loop_mapeamento_bonecoD();



	/********************************* MAPEAMENTO DE DIRECAO*********************************/

	/* Cor referente a intersecção que o robo acabou de sair */
	Cor cor_atual = Cor::ndCor;

	/* Direção referente a intersecção que o robo acabou de sair */
	direcao direcao_atual = direcao::ndDirecao;

	/* Confirmação se a a intersecção anterior já foi mapeada */
	bool confirmacao_status = false;

	/* Confirmação se o robo está voltando de um dead-end */
	bool dead_end = false;

	/* Flags para salvar ultima leitura da cor quando necessario */
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;

	/* Flag que controla o comportamento do robo de acordo com onde ele está */
	estados_arena estd_map = estados_arena::faixa;


	/* Contador de quantas cores já foram mapeadas */
	int qnt_cores_mapeadas = 0;

	/* Quando o robo estiver saindo de um lado ele volta e realinha */
	void realinha(direcao lado_saindo);

	/* Quando o robo chegar em uma intersec (vermelho, verde, azul ou preto) */
	void intersec();

	/* Método de mapemento das direções das intersecções */
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

	/* Verifica se a cor que o robo está sobre quando chamar o método já foi mapeada*/
	bool cor_ja_mapeada();


	void destructor_vector();
	void novo_no_map_boneco();
};


#endif
