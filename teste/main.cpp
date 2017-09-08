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
//#include "src_folder/Controlador_robo.h"
using namespace std;

Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.7, true,  "debug_lego_E.m");
Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04055, 0.7, true,  "debug_lego_D.m");
Sensor_cor sensorE(ev3dev::INPUT_1);
Sensor_cor sensorD(ev3dev::INPUT_2);
//Ultrassom_nxt US_Esquerdo(Ultrassom_nxt::INPUT_3);
//Ultrassom_nxt US_Direito(Ultrassom_nxt::INPUT_4);

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	//	Controlador_robo robots;
	//	robots.inicializar_thread();
	//
	//	cout << "Waiting..."<<endl;
	//	while (!ev3dev::button::enter.process()){}
	//	robots.frente();
	//	usleep(2*1000000);
	//	robots.parar();
	//	usleep(0.5*1000000);
	//	robots.tras();
	//	usleep(2*1000000);
	//	robots.parar();
	//	usleep(0.5*1000000);
	//
	//	//Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, true,  "debug_lego_E.m");
	//	//Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");
	rodaE.inicializa_thread();
	rodaD.inicializa_thread();


	//**********************************************************
	//*****************************DELETAR**********************
	//**********************************************************
	//sensorE.calibra();
	double velo_retardada = 0;
	double velo_sp = 0.3;
	rodaE.set_velo(velo_sp);
	rodaD.set_velo(velo_sp);
	//	double aceleracao = 0.5;
	//	while(velo_retardada <= velo_sp){
	//		rodaE.set_velo(velo_retardada);
	//		rodaD.set_velo(velo_retardada);
	//		usleep(1000*5);
	//		velo_retardada += aceleracao*0.005;
	//	}


	//	while(sensorE.ler_cor() == Cor::branco){
	//		cout<<"nao para"<<endl;
	//	}
	//	cout<<"PARA"<<endl;
	//	velo_sp = 0;
	//	aceleracao = velo_retardada/0.005; // faz a velo_retardada ir a 0
	//	while(velo_retardada > velo_sp){
	//		rodaE.set_velo(velo_retardada);
	//		rodaD.set_velo(velo_retardada);
	//		usleep(1000*5);
	//		if(velo_retardada< aceleracao*0.01 && velo_retardada> -aceleracao*0.01 ){
	//			rodaE.set_velo(0);
	//			rodaD.set_velo(0);
	//
	//			velo_retardada = 0;
	//			usleep(1000*300);
	//			aceleracao = 0.5;
	//			break;
	//		}
	//		velo_retardada -= aceleracao*0.005;
	//	}












	//	//sensorE.calibra();
	//	//sensorD.calibra();
	//	//cout<<"iniciou teste"<<endl;
	//
	//	cout << "Waiting..."<<endl;
	while(!ev3dev::button::enter.process()){
		//		//cout<<US_Esquerdo.le_centimetro()<<"<>"<<US_Direito.le_centimetro()<<endl;
	}

	rodaE.set_velo(0);
	rodaD.set_velo(0);
	//
	//	robots.finalizar_thread();
	//
	rodaE.finaliza_thread();
	rodaD.finaliza_thread();

}
