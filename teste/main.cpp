/*
 * main.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */




#include <Sensor_cor.h>
#include <unistd.h>
#include "ev3dev.h"
#include "src_folder/Controlador_motor.h"
#include "src_folder/MArquivos.h"
#include "src_folder/Sensor_cor.h"
using namespace std;

Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.248, true,  "debug_lego_E.m");
Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");
Sensor_cor sensorE(ev3dev::INPUT_1);
Sensor_cor sensorD(ev3dev::INPUT_2);

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");


	rodaE.inicializa_thread();
	rodaD.inicializa_thread();

	//rodaE.set_velo(0.3);
	//rodaD.set_velo(0.3);

	//sensorE.calibra();
	//sensorD.calibra();


	while(!ev3dev::button::enter.process()){
		//cout<<sensorE.ler_cor()<<";"<<sensorD.ler_cor()<<endl;
	}


	rodaE.finaliza_thread();
	rodaD.finaliza_thread();

}
