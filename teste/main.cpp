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

ev3dev::large_motor rodaD(ev3dev::OUTPUT_B); // roda esquerda



int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	rodaD.reset();
	rodaD.set_position_sp(360*25);
	rodaD.set_speed_sp(180*3);
	rodaD.set_ramp_up_sp(rodaD.max_speed()*1000/108); // em miliseg
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



	while(!ev3dev::button::enter.process()){
	}
	while(!ev3dev::button::enter.process()){
	}

}

