/*
 * Main.cpp
 *
 *  Created on: 8 de ago de 2017
 *      Author: pcsek
 */

#include <iostream>
#include <tuple>
#include "ev3dev.h"
#include <unistd.h>

using namespace std;

enum Cor{preto, branco, verde,nda};
enum Parte_prova{linha, desvia, busca};
enum Direcao{esquerda, direita};

struct RangesCor{
	int r[2],g[2], b[2];
	static const int range = 30;
};

RangesCor r_dir_branco, r_dir_preto, r_dir_verde;
RangesCor r_esq_branco, r_esq_preto, r_esq_verde;

Parte_prova estado;
bool corEstaEntre(std::tuple<int, int, int>, RangesCor);
Cor linhaE();
Cor linhaD();
void segueLinha();
void frente();
void traz();
void stop();
void girar(Direcao, int);
void calibraLinha();
void desviaObstaculo();
void intersecao();

ev3dev::large_motor rodaD(ev3dev::OUTPUT_A);
ev3dev::large_motor rodaE(ev3dev::OUTPUT_B);
ev3dev::color_sensor sensorD(ev3dev::INPUT_1);
ev3dev::color_sensor sensorE(ev3dev::INPUT_2);
ev3dev::ultrasonic_sensor us(ev3dev::INPUT_3);

int main(){
	rodaE.reset();
	rodaD.reset();
	stop();
	system("setfont Greek-TerminusBold20x10.psf.gz");

	calibraLinha();

	while(!ev3dev::button::back.process()){
		segueLinha();


		//cout<<linhaE()<<";"<<linhaD()<<endl;

		//				frente();
		//				usleep(1000000);
		//				stop();
		//				girar(Direcao::esquerda, 300);
		//				usleep(500000);
		//				girar(Direcao::direita, 300);
		//				traz();
		//				usleep(1000000);
		//				stop();
		//				usleep(1000000);

	}
}

/**
 * metodo de auxilio para usar dentro das funcoes linhaE e linhaD
 */
bool corEstaEntre(std::tuple<int, int, int> sample, RangesCor ranges){
	if(get<0>(sample) > ranges.r[0] && get<0>(sample) < ranges.r[1] &&				// verifica se o red ta dentro do range
			get<1>(sample) > ranges.g[0] && get<1>(sample) < ranges.g[1] &&			// verifica se o green ta denrto do range
			get<2>(sample) > ranges.b[0] && get<2>(sample) < ranges.b[1])			// verifica se o blue ta dentro do range
		return true;
	else return false;
}

Cor linhaE(){
	std::tuple<int, int, int> sample;
	sample = sensorE.raw();
	if(corEstaEntre(sample, r_esq_preto)) return Cor::preto;
	if(corEstaEntre(sample, r_esq_branco)) return Cor::branco;
	if(corEstaEntre(sample, r_esq_verde)) return Cor::verde;
	else{
		return Cor::nda;
	}
}

Cor linhaD(){
	std::tuple<int, int, int> sample;
	sample = sensorD.raw();

	if(corEstaEntre(sample, r_dir_preto)) return Cor::preto;
	if(corEstaEntre(sample, r_dir_branco)) return Cor::branco;
	if(corEstaEntre(sample, r_dir_verde)) return Cor::verde;
	else{
		return Cor::nda;
	}
}

void segueLinha(){


	estado = Parte_prova::linha;

	while(estado == Parte_prova::linha){
		frente();

		if(linhaE() == Cor::preto){
			stop();
			traz();
			usleep(300*1000);
			girar(Direcao::esquerda,200);
		}
		if(linhaD() == Cor::preto){
			stop();
			traz();
			usleep(300*1000);
			girar(Direcao::direita,200);
		}


		if(linhaE() == Cor::verde){		// intersecao a esquerda detectada
			stop();
			cout<<"esq"<<endl;
			usleep(500*1000);
			rodaD.set_speed_sp(80);
			rodaE.set_speed_sp(80);
			rodaD.run_forever();
			rodaE.run_forever();
			while(linhaE() != Cor::preto){};
			stop();
			traz();
			usleep(200*1000);
			stop();
			rodaD.set_speed_sp(80);
			rodaD.run_forever();
			usleep(2500*1000);
			rodaE.set_speed_sp(80);
			rodaE.run_forever();
			usleep(700*1000);
			stop();
		}
	}
}


void frente(){
	rodaD.set_speed_sp(180);
	rodaE.set_speed_sp(180);
	rodaD.run_forever();
	rodaE.run_forever();
}

void traz(){
	rodaD.set_speed_sp(-180);
	rodaE.set_speed_sp(-180);
	rodaD.run_forever();
	rodaE.run_forever();
}

void stop(){
	rodaD.set_stop_action(ev3dev::motor::stop_action_hold);
	rodaE.set_stop_action(ev3dev::motor::stop_action_hold);
	rodaD.stop();
	rodaE.stop();

}

void girar(Direcao dir, int millis){
	stop();
	if(dir == Direcao::esquerda){
		rodaE.set_speed_sp(-120);
		rodaD.set_speed_sp(120);
		rodaE.run_forever();
		rodaD.run_forever();
		usleep(millis*1000);
	}
	else{
		rodaE.set_speed_sp(120);
		rodaD.set_speed_sp(-120);
		rodaE.run_forever();
		rodaD.run_forever();
		usleep(millis*1000);
	}
	stop();

}



void calibraLinha(){
	std::tuple<int, int, int> rgbverde;
	std::tuple<int, int, int> rgbpreto;
	std::tuple<int, int, int> rgbbranco;

	cout<<"cal dir preto"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbpreto = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal dir \n branco"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbbranco = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal dir \n verde"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbverde = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	r_dir_preto.r[0] = get<0>(rgbpreto) - RangesCor::range;
	r_dir_preto.r[1] = get<0>(rgbpreto) + RangesCor::range;
	r_dir_preto.g[0] = get<1>(rgbpreto) - RangesCor::range;
	r_dir_preto.g[1] = get<1>(rgbpreto) + RangesCor::range;
	r_dir_preto.b[0] = get<2>(rgbpreto) - RangesCor::range;
	r_dir_preto.b[1] = get<2>(rgbpreto) + RangesCor::range;
	r_dir_branco.r[0] = get<0>(rgbbranco) - RangesCor::range;
	r_dir_branco.r[1] = get<0>(rgbbranco) + RangesCor::range;
	r_dir_branco.g[0] = get<1>(rgbbranco) - RangesCor::range;
	r_dir_branco.g[1] = get<1>(rgbbranco) + RangesCor::range;
	r_dir_branco.b[0] = get<2>(rgbbranco) - RangesCor::range;
	r_dir_branco.b[1] = get<2>(rgbbranco) + RangesCor::range;
	r_dir_verde.r[0] = get<0>(rgbverde) - RangesCor::range;
	r_dir_verde.r[1] = get<0>(rgbverde) + RangesCor::range;
	r_dir_verde.g[0] = get<1>(rgbverde) - RangesCor::range;
	r_dir_verde.g[1] = get<1>(rgbverde) + RangesCor::range;
	r_dir_verde.b[0] = get<2>(rgbverde) - RangesCor::range;
	r_dir_verde.b[1] = get<2>(rgbverde) + RangesCor::range;

	cout<<"cal esq preto"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbpreto = sensorE.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal esq \n branco"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbbranco = sensorE.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal esq \n verde"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgbverde = sensorE.raw();
	while(!ev3dev::button::enter.process()){};

	r_esq_preto.r[0] = get<0>(rgbpreto) - RangesCor::range;
	r_esq_preto.r[1] = get<0>(rgbpreto) + RangesCor::range;
	r_esq_preto.g[0] = get<1>(rgbpreto) - RangesCor::range;
	r_esq_preto.g[1] = get<1>(rgbpreto) + RangesCor::range;
	r_esq_preto.b[0] = get<2>(rgbpreto) - RangesCor::range;
	r_esq_preto.b[1] = get<2>(rgbpreto) + RangesCor::range;
	r_esq_branco.r[0] = get<0>(rgbbranco) - RangesCor::range;
	r_esq_branco.r[1] = get<0>(rgbbranco) + RangesCor::range;
	r_esq_branco.g[0] = get<1>(rgbbranco) - RangesCor::range;
	r_esq_branco.g[1] = get<1>(rgbbranco) + RangesCor::range;
	r_esq_branco.b[0] = get<2>(rgbbranco) - RangesCor::range;
	r_esq_branco.b[1] = get<2>(rgbbranco) + RangesCor::range;
	r_esq_verde.r[0] = get<0>(rgbverde) - RangesCor::range;
	r_esq_verde.r[1] = get<0>(rgbverde) + RangesCor::range;
	r_esq_verde.g[0] = get<1>(rgbverde) - RangesCor::range;
	r_esq_verde.g[1] = get<1>(rgbverde) + RangesCor::range;
	r_esq_verde.b[0] = get<2>(rgbverde) - RangesCor::range;
	r_esq_verde.b[1] = get<2>(rgbverde) + RangesCor::range;

	while(!ev3dev::button::enter.process()){};
	while(!ev3dev::button::enter.process()){};
	cout<<endl<<endl<<endl<<endl;

}
