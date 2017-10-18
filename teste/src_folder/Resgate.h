#ifndef RESGATE_H_
#define RESGATE_H_

#include "Const.h"
#include "Controlador_robo.h"
#include "Ultrassom_nxt.h"

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

	estados_arena estd_resgate = estados_arena::faixa;
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;


	void intersec();
	void realinha(direcao );
	void caminho_certo();
	void inverter_ponteiros_pos();

};
