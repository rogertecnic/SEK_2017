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

int r,g,b;
bool calibraLinha();
bool linhaE();
bool linhaD();
bool segueLinha();
bool desviaObstaculo();
bool intersecao();

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
		//linhaD();

		//cout<<get<1>(sample)<<";"<<g<<endl;
		if(get<0>(sample)>r)
			cout<<"branco"<<endl;
		else if (get<1>(sample)>g)
			cout<<"verde"<<endl;
		else cout<<"preto"<<endl;
	}


}

bool linhaE(){
	std::tuple<int, int, int> sample;
	sample = sensorE.raw();
	cout<<get<0>(sample)<<";"<<get<1>(sample)<<";"<<get<2>(sample)<<endl;
}

bool linhad(){
	std::tuple<int, int, int> sample;
	sample = sensorD.raw();
	cout<<get<0>(sample)<<";"<<get<1>(sample)<<";"<<get<2>(sample)<<endl;
}

bool segueLinha(){
	rodaE.reset();
	rodaD.reset();
	rodaE.set_speed_sp(360);
	rodaD.set_speed_sp(360);

	rodaE.run_forever();
	rodaD.run_forever();
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
