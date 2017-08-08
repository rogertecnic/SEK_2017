/*
 * Main.cpp
 *
 *  Created on: 8 de ago de 2017
 *      Author: pcsek
 */

#include <iostream>
#include <tuple>
#include "ev3dev.h"

using namespace std;

enum Cor{preto, branco, verde};
enum Parte_prova{segueLinha, desvia, busca};
enum Direcao{esquerda, direita};
int r,g,b;
Parte_prova estado;

Cor linhaE();
Cor linhaD();
void segueLinha();
void frente();
void traz();
void stop();
void girar(int);
void calibraLinha();
void desviaObstaculo();
void intersecao();

ev3dev::large_motor rodaD(ev3dev::OUTPUT_A);
ev3dev::large_motor rodaE(ev3dev::OUTPUT_B);
ev3dev::color_sensor sensorD(ev3dev::INPUT_1);
ev3dev::color_sensor sensorE(ev3dev::INPUT_2);
ev3dev::ultrasonic_sensor us(ev3dev::INPUT_3);

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");

	calibraLinha();

	while(!ev3dev::button::back.process()){
		std::tuple<int, int, int> sample;
		sample = sensorD.raw();
		cout<<linhaE()<<";"<<linhaD()<<endl;
	}
}


Cor linhaE(){
	std::tuple<int, int, int> sample;
	sample = sensorE.raw();
	if(get<0>(sample)>r)
		return Cor::branco; //cout<<"branco"<<endl;
	else if (get<1>(sample)>g)
		return Cor::verde ; //cout<<"verde"<<endl;
	else return Cor::preto; //cout<<"preto"<<endl;
}

Cor linhaD(){
	std::tuple<int, int, int> sample;
	sample = sensorD.raw();
	if(get<0>(sample)>r)
		return Cor::branco; //cout<<"branco"<<endl;
	else if (get<1>(sample)>g)
		return Cor::verde ; //cout<<"verde"<<endl;
	else return Cor::preto; //cout<<"preto"<<endl;
}

void segueLinha(){
	rodaE.reset();
	rodaD.reset();
	rodaE.set_speed_sp(360);
	rodaD.set_speed_sp(360);

	estado = Parte_prova::segueLinha;
	rodaE.run_forever();
	rodaD.run_forever();
	while(estado == Parte_prova::segueLinha){

		if(linhaE() == Cor::preto){

		}
		if(linhaD() == Cor::preto){

		}

		if(linhaE() == Cor::verde){

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

void girar(Direcao dir){

}

bool calibraLinha(){
	cout<<"cal dir \n linha preta"<<endl;
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
}
