#ifndef RESGATE_H_
#define RESGATE_H_

#include "Const.h"
#include "Controlador_robo.h"
#include "Ultrassom_nxt.h"

#define distancia_boneco 18

enum lado_ultra {ndaLado, dir, esq};

class Resgate {
public:
	Resgate(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);
	void resgatar();

private:
	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;
	ev3dev::large_motor *motorCancela;

	estados_arena estd_resgate = estados_arena::faixa;
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;

	lado_ultra lado = lado_ultra::ndaLado;

	bool inicio_resg = true;
	bool fim_resg = false;

	int contador_bonecos = 0;
	double gdistancia_boneco = -1;

	int pos_e = 0,
		pre_e = 0,
		pos_d = 0,
		pre_d = 0;


	void goto_plaza();
	void get_distancia_boneco();
	void just_do_it();
	void intersec();
	void realinha(direcao );
	void caminho_certo();
	void inverter_ponteiros();
	bool fim_da_cidade();
	bool inicio_da_cidade();

	vector<no_intersec>::iterator it;

};
