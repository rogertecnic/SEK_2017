/*
 * main.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */




#include <M_arquivos.h>
#include <Sensor_cor.h>
#include <unistd.h>
#include "ev3dev.h"
#include "src_folder/Controlador_motor.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Ultra_Som.h"
using namespace std;

Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.248, true,  "debug_lego_E.m");
Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");
//Sensor_cor sensorE(ev3dev::INPUT_1);
//Sensor_cor sensorD(ev3dev::INPUT_2);
//Ultra_Som US_Esquerdo(ev3dev::INPUT_3,0,100);
//Ultra_Som US_Direito(ev3dev::INPUT_4,0,100);

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	//Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, true,  "debug_lego_E.m");
	//Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");




	//rodaE.inicializa_thread();
	//rodaD.inicializa_thread();

	//rodaE.set_velo(0.3);
	//rodaD.set_velo(0.3);

	//sensorE.calibra();
	//sensorD.calibra();
	//cout<<"iniciou teste"<<endl;
	ev3dev::ultrasonic_sensor nxt_us(ev3dev::INPUT_3);
	usleep(1000*1000*10);
	while(!ev3dev::button::enter.process()){
		cout<<nxt_us.distance_centimeters()<<endl;
		//cout<<US_Esquerdo.le_distancia()<<";"<<US_Direito.le_distancia()<<endl;
		//cout<<sensorE.ler_cor()<<";"<<sensorD.ler_cor()<<endl;
	}


	rodaE.finaliza_thread();
	rodaD.finaliza_thread();

}
