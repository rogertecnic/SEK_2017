#ifndef RESGATE_H_
#define RESGATE_H_

#include "Controlador_robo.h"
#include "Sensor_cor_hsv.h"
#include  "Ultrassom_nxt.h"
#include "Const.h"

#define distancia_boneco 18 //MODIFICAR DEPOIS

class Resgate {
public:

	Resgate(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);
	void resgatar();

private:
	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;
	//ev3dev::large_motor *motorCancela;

	/*********************************************************************************************/
	/****************************VARIAVEIS DO CONTROLE DE DIRECAO*********************************/
	/*********************************************************************************************/
	// pwm principal, quando o robo esta andando e procurando
	int pwm_busca = 60;

	int carga_bonecos = 0;

	direcao direcao_boneco = direcao::ndDirecao;
	// cor que o referente a intersecao que o robo acabou de sair
	//Cor cor_atual = Cor::ndCor;

	// direcao referente a intersecao que o robo acabou de sair
	//direcao direcao_atual = direcao::ndDirecao;

	//flags para salvar ultima leitura da cor quando necessario
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;

	// flag que controla o comportamento do robo de acordo com onde ele esta
	estados_arena estd = estados_arena::faixa;
	/*-------------------------------------------------------------------------------------------*/
	/*-------------------------FIM VARIAVEIS DO CONTROLE DEDIRECAO--------------------------------*/
	/*-------------------------------------------------------------------------------------------*/


	/*********************************************************************************************/
	/***************************METODOS DO CONTROLE DE DIRECAO************************************/
	/*********************************************************************************************/

	// quando o robo estiver saindo de um lado ele volta e realinha
	void realinha(direcao lado_saindo);
	// quando o robo chegar em uma intersec (vermelho, verde, azul ou preto)
	void intersec();

	/* Método utilizado em: mapeamento_intersec
	 * Se o robô passar por uma interssecção onde a mesma já foi previamente mapeada
	 * ele segue pelo "caminho certo"
	 */
	void caminho_certo();
	/*-------------------------------------------------------------------------------------------*/
	/*-------------------------FIM METODOS DO CONTROLE DE DIRECAO--------------------------------*/
	/*-------------------------------------------------------------------------------------------*/


	/*********************************************************************************************/
	/******************************VARIAVEIS DO RESGATE*******************************************/
	/*********************************************************************************************/
	bool viu_boneco_E = false;
	bool viu_boneco_D = false;
	int dist_boneco_E = 999;
	int dist_boneco_D = 999;
	/*-------------------------------------------------------------------------------------------*/
	/*------------------------------FIM VARIAVEIS DO RESGATE-------------------------------------*/
	/*-------------------------------------------------------------------------------------------*/

	/*********************************************************************************************/
	/*********************************METODOS DO RESGATE******************************************/
	/*********************************************************************************************/



	/*-------------------------------------------------------------------------------------------*/
	/*--------------------------------FIM METODOS DO RESGATE-------------------------------------*/
	/*-------------------------------------------------------------------------------------------*/

};;
