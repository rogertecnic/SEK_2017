/*
 * main.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */




#include "Classe.h"
#include <unistd.h>
#include "ev3dev.h"

using namespace std;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

ev3dev::large_motor rodaE(ev3dev::OUTPUT_A); // roda esquerda
ev3dev::large_motor rodaD(ev3dev::OUTPUT_B); // roda direita

void testeAceleracao(){
	// usando as funcoes ramp_up e ramp_down:
	// argumento: milliseg que o motor demora pra acelerar/desacelerar de 0
	// ate a velocidade maxima teórica (max_speed)
	// como não queremos a velocidade máxima precisamos fazer umas conversões
	// tambem da pra fazer por semelhança de triangulos imaginando
	// uma reta num grafico velo vs tempo o seu triangulo menor
	// teria os catetos como o t_sp e o v_sp e o triangulo maior
	// seria o tm e o vm, daria na mesma conta, a aceleraçao eh a inclinacao
	// da reta, aooo fisica 1 em

	int v_sp = 180*3; // setpoint da velocidade real
	int t_sp = 5; // tempo de aceleracao/desaceleracao de 0 ate v_sp
	int a_sp = v_sp/t_sp; // aceleracao/desaceleracao que queremos no motor
	int vm = rodaD.max_speed(); // velocidade teorica maxima do motor
	int tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg

	rodaD.reset();
	rodaD.set_position_sp(360*25);
	rodaD.set_speed_sp(v_sp);
	rodaD.set_ramp_up_sp(tm*1000); // em miliseg
	rodaD.set_ramp_down_sp(50);
	rodaD.run_forever();

	auto t0 = Time::now();
	std::cout<<"iniciado"<<endl;

	while((rodaD.speed()>=180*3?false:true )){
	}

	auto t1 = Time::now();
	fsec fs = t1 - t0;
	//std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
	std::cout << fs.count() << "s\n";
	//std::cout << d.count() << "ms\n";


	// espera apertar e soltar o botao enter do brick
	while(!ev3dev::button::enter.process()){
	}
	while(!ev3dev::button::enter.process()){
	}

}

void anda_e_para(){
	rodaE.reset();
	rodaD.reset();
	rodaE.set_stop_action("hold");
	rodaD.set_stop_action("hold");

	double pos_sp = 360*3;
	double v_sp = 180*4; // setpoint da velocidade real
	double t_sp = 5; // tempo de aceleracao/desaceleracao de 0 ate v_sp
	double a_sp = v_sp/t_sp; // aceleracao/desaceleracao que queremos no motor
	double vm = rodaE.max_speed(); // velocidade teorica maxima do motor
	double tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg
	std::cout<<vm<<std::endl;

	rodaE.set_speed_sp(v_sp);
	rodaE.set_ramp_up_sp(tm*1000);
	rodaE.set_ramp_down_sp(tm*1000);
	rodaE.set_position_sp(pos_sp);

	vm = rodaD.max_speed(); // velocidade teorica maxima do motor
	tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg
	std::cout<<vm<<std::endl;

	rodaD.set_speed_sp(v_sp);
	rodaD.set_ramp_up_sp(tm*1000);
	rodaD.set_ramp_down_sp(tm*1000);
	rodaD.set_position_sp(pos_sp);

	//rodaE.run_to_rel_pos();
	//rodaD.run_to_rel_pos();

	//rodaD.run_forever();
	rodaE.run_forever();
	usleep(7000*1000); // em milli

	// reajuste da aceleração
	v_sp = 180*1; // setpoint da velocidade real
	t_sp = 0.2; // tempo de aceleracao/desaceleracao de 0 ate v_sp
	a_sp = v_sp/t_sp; // aceleracao/desaceleracao que queremos no motor
	tm = vm/a_sp; // tempo que o motor demoraria para acelerar/desacelerar de 0 a vm em seg

	rodaE.set_ramp_up_sp(tm*1000);
	rodaE.set_ramp_down_sp(tm*1000);

	// reajuste da velocidade
	rodaE.set_speed_sp(v_sp);
	rodaE.run_forever();
	usleep(6000*1000); // em milli
	rodaE.stop();
	rodaD.stop();

	// espera apertar e soltar o botao enter do brick
	while(!ev3dev::button::enter.process()){
	}
	while(!ev3dev::button::enter.process()){
	}

}


















int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	anda_e_para();
}
