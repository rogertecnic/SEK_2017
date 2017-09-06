/*
 * MainContaTempo.cpp
 *
 *  Created on: 15 de ago de 2017
 *      Author: pcsek
 *
 *      Classe feita utilizando conceitos basicos de medição de tempo
 *      de execução, funcionam em tempo real no brick, mas foi testando
 *      sem nenhum outro processamento paralelo, somente este codigo
 */
#include <iostream>
#include <chrono>
#include "src_folder/ev3dev.h"

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;
typedef std::chrono::duration<float> fsec;

int main1(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	while(!ev3dev::button::enter.process()){
	}
	while(!ev3dev::button::enter.process()){
	}

	auto t0 = Time::now();
	std::cout<<"iniciado"<<std::endl;

	while(true){
		while(!ev3dev::button::enter.process()){
		}
		while(!ev3dev::button::enter.process()){
		}

		auto t1 = Time::now();
		fsec fs = t1 - t0;
		//std::chrono::milliseconds d = std::chrono::duration_cast<std::chrono::milliseconds>(fs);
		std::cout << fs.count() << "s\n";
		//std::cout << d.count() << "ms\n";

	}
}

