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
#include "src_folder/Ultrassom_nxt.h"
using namespace std;

//Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.248, true,  "debug_lego_E.m");
//Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");
//Sensor_cor sensorE(ev3dev::INPUT_1);
//Sensor_cor sensorD(ev3dev::INPUT_2);
Ultrassom_nxt US_Esquerdo(Ultrassom_nxt::INPUT_3);
Ultrassom_nxt US_Direito(Ultrassom_nxt::INPUT_4);

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, true,  "debug_lego_E.m");
	Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");




	rodaE.inicializa_thread();
	rodaD.inicializa_thread();

	//rodaE.set_velo(0.3);
	//rodaD.set_velo(0.3);

	//sensorE.calibra();
	//sensorD.calibra();
	//cout<<"iniciou teste"<<endl;
	while(!ev3dev::button::enter.process()){
		//cout<<US_Esquerdo.le_centimetro()<<"<>"<<US_Direito.le_centimetro()<<endl;
	}


	rodaE.finaliza_thread();
	rodaD.finaliza_thread();

}
