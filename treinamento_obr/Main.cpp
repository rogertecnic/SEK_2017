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
	static const int range = 10;
};

RangesCor range_branco, range_preto, range_verde;

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
		//segueLinha();


		cout<<linhaE()<<";"<<linhaD()<<endl;
		//		frente();
		//		usleep(1000000);
		//		stop();
		//		girar(Direcao::esquerda, 300);
		//		usleep(500000);
		//		girar(Direcao::direita, 300);
		//		traz();
		//		usleep(1000000);
		//		stop();
		//		usleep(1000000);
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
	if(corEstaEntre(sample, range_preto)) return Cor::preto;
	if(corEstaEntre(sample, range_branco)) return Cor::branco;
	if(corEstaEntre(sample, range_verde)) return Cor::verde;
	else{
		cout<<"cor irreconhecivel"<<endl;
		return Cor::nda;
	}
}

Cor linhaD(){
	std::tuple<int, int, int> sample;
	sample = sensorD.raw();

	//	if(get<0>(sample)>r && get<1>(sample)>g &&get<2>(sample)>b)
	//		return Cor::branco;
	//	if(get<0>(sample)<r && get<1>(sample)>g &&get<2>(sample)<b)
	//		return Cor::verde;
	//	else return Cor::preto;

	//	if(get<0>(sample)>r)
	//			return Cor::branco; //cout<<"branco"<<endl;
	//		else if (get<1>(sample)>g)
	//			return Cor::verde; //cout<<"verde"<<endl;
	//		else return Cor::preto; //cout<<"preto"<<endl;
	return Cor::nda;
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


		if(linhaE() == Cor::verde){		// intersecao detectada
			stop();
			usleep(10000);
			rodaD.set_speed_sp(80);
			rodaE.set_speed_sp(80);
			frente();
			while(linhaE() != Cor::preto){};
			stop();
			traz();
			usleep(500*1000);
			stop();
			rodaD.run_forever();
			usleep(800*1000);
			stop();


			//			if(linhaE() == Cor::verde){
			//
			//				usleep(500000);
			//				rodaD.set_speed_sp(80);
			//				rodaE.set_speed_sp(80);
			//				frente();
			//				bool rodaEparou = false;
			//				bool rodaDparou = false;
			//				while(true){
			//					if(linhaE() == Cor::preto && !rodaEparou){
			//						rodaE.stop();
			//						rodaEparou = true;
			//					}
			//					if(linhaD() == Cor::preto){
			//						rodaD.stop();
			//						rodaDparou = true;
			//					}
			//					if(rodaEparou && rodaDparou)break;
			//				}
			//				girar(Direcao::esquerda, 500);
			//				frente();
			//				usleep(400*1000);
			//				stop();
			//				usleep(2000000);
			//			}
			//		}
			//
			//		if(linhaD() == Cor::verde){

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
	cout<<"cal dir preto"<<endl;
	while(!ev3dev::button::enter.process()){};
	std::tuple<int, int, int> rgbpreto;
	rgbpreto = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal dir \n branco"<<endl;
	while(!ev3dev::button::enter.process()){};
	std::tuple<int, int, int> rgbbranco;
	rgbbranco = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal dir \n verde"<<endl;
	while(!ev3dev::button::enter.process()){};
	std::tuple<int, int, int> rgbverde;
	rgbverde = sensorD.raw();
	while(!ev3dev::button::enter.process()){};

	range_preto.r[0] = get<0>(rgbpreto) - RangesCor::range;
	range_preto.r[1] = get<0>(rgbpreto) + RangesCor::range;
	range_preto.g[0] = get<1>(rgbpreto) - RangesCor::range;
	range_preto.g[1] = get<1>(rgbpreto) + RangesCor::range;
	range_preto.b[0] = get<2>(rgbpreto) - RangesCor::range;
	range_preto.b[1] = get<2>(rgbpreto) + RangesCor::range;
	range_branco.r[0] = get<0>(rgbbranco) - RangesCor::range;
	range_branco.r[1] = get<0>(rgbbranco) + RangesCor::range;
	range_branco.g[0] = get<1>(rgbbranco) - RangesCor::range;
	range_branco.g[1] = get<1>(rgbbranco) + RangesCor::range;
	range_branco.b[0] = get<2>(rgbbranco) - RangesCor::range;
	range_branco.b[1] = get<2>(rgbbranco) + RangesCor::range;
	range_verde.r[0] = get<0>(rgbverde) - RangesCor::range;
	range_verde.r[1] = get<0>(rgbverde) + RangesCor::range;
	range_verde.g[0] = get<1>(rgbverde) - RangesCor::range;
	range_verde.g[1] = get<1>(rgbverde) + RangesCor::range;
	range_verde.b[0] = get<2>(rgbverde) - RangesCor::range;
	range_verde.b[1] = get<2>(rgbverde) + RangesCor::range;


	//		r = ( get<0>(rgbpreto) + get<0>(rgbbranco) )/2;
	//		g = ( get<1>(rgbpreto) + get<1>(rgbverde) )/2;
	//		b = ( get<2>(rgbpreto) + get<2>(rgbbranco) )/2;
	//cout<<r<<";"<<g<<";"<<b<<endl;
	while(!ev3dev::button::enter.process()){};
	while(!ev3dev::button::enter.process()){};
	cout<<endl<<endl<<endl<<endl;
}



//void calibraLinha(){
//	cout<<"cal dir \n linha preta"<<endl;
//	while(!ev3dev::button::enter.process()){};
//	std::tuple<int, int, int> rgbpreto;
//	rgbpreto = sensorD.raw();
//	while(!ev3dev::button::enter.process()){};
//
//	cout<<"cal dir \n branco"<<endl;
//	while(!ev3dev::button::enter.process()){};
//	std::tuple<int, int, int> rgbbranco;
//	rgbbranco = sensorD.raw();
//	while(!ev3dev::button::enter.process()){};
//
//	cout<<"cal dir \n verde"<<endl;
//	while(!ev3dev::button::enter.process()){};
//	std::tuple<int, int, int> rgbverde;
//	rgbverde = sensorD.raw();
//	while(!ev3dev::button::enter.process()){};
//
//
//	r = ( get<0>(rgbpreto) + get<0>(rgbbranco) )/2;
//	g = ( get<1>(rgbpreto) + get<1>(rgbverde) )/2;
//	b = ( get<2>(rgbpreto) + get<2>(rgbbranco) )/2;
//	cout<<r<<";"<<g<<";"<<b<<endl;
//	while(!ev3dev::button::enter.process()){};
//	while(!ev3dev::button::enter.process()){};
//	cout<<endl<<endl<<endl<<endl;
//}
