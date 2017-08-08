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

enum Cor{preto, branco, verde};
enum Parte_prova{linha, desvia, busca};
enum Direcao{esquerda, direita};
int r,g,b;
Parte_prova estado;

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


Cor linhaE(){
	std::tuple<int, int, int> sample;
	sample = sensorE.raw();
	if(get<0>(sample)>r && get<1>(sample)>g &&get<2>(sample)>b)
		return Cor::branco;
	if(get<0>(sample)<r && get<1>(sample)>g &&get<2>(sample)<b)
		return Cor::verde;
	else return Cor::preto;
}

Cor linhaD(){
	std::tuple<int, int, int> sample;
	sample = sensorD.raw();

	if(get<0>(sample)>r && get<1>(sample)>g &&get<2>(sample)>b)
		return Cor::branco;
	if(get<0>(sample)<r && get<1>(sample)>g &&get<2>(sample)<b)
		return Cor::verde;
	else return Cor::preto;
	//	if(get<0>(sample)>r)
	//			return Cor::branco; //cout<<"branco"<<endl;
	//		else if (get<1>(sample)>g)
	//			return Cor::verde; //cout<<"verde"<<endl;
	//		else return Cor::preto; //cout<<"preto"<<endl;
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

		if(linhaE() == Cor::verde){
			stop();
			usleep(10000);
			if(linhaE() == Cor::verde){
				cout<<"intrcect esq"<<endl;
				usleep(500000);
				rodaD.set_speed_sp(80);
				rodaE.set_speed_sp(80);
				frente();
				bool rodaEparou = false;
				bool rodaDparou = false;
				while(true){
					if(linhaE() == Cor::preto && !rodaEparou){
						rodaE.stop();
						rodaEparou = true;
					}
					if(linhaD() == Cor::preto){
						rodaD.stop();
						rodaDparou = true;
					}
					if(rodaEparou && rodaDparou)break;
				}
				girar(Direcao::esquerda, 500);
				frente();
				usleep(400*1000);
				stop();
				cout<<"conseguiu terminar o cruzamento"<<endl;
				usleep(2000000);
			}
		}

		if(linhaD() == Cor::verde){

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


		r = ( get<0>(rgbpreto) + get<0>(rgbbranco) )/2;
		g = ( get<1>(rgbpreto) + get<1>(rgbverde) )/2;
		b = ( get<2>(rgbpreto) + get<2>(rgbbranco) )/2;
		cout<<r<<";"<<g<<";"<<b<<endl;
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
