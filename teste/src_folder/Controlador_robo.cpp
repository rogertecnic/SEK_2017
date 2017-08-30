#include "Controlador_robo.h"

void nao_cai_porra () {
	Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.808, 0.3*0.826666667, true,  "debug_lego_E.m");
	Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04058, 0.79, 0.3, true,  "debug_lego_D.m");
	Sensor_cor  sensorE;
	Sensor_cor sensorD;

	rodaE.inicializa_thread();
	rodaD.inicializa_thread();

	rodaE.set_velo(0.3);
	rodaD.set_velo(0.3);

	usleep(1000*1000);
	while (true){
		if (sensorE.ler_cor()==nda || sensorD.ler_cor()==nda ) {

		}
	}


	rodaE.finaliza_thread();
	rodaD.finaliza_thread();

}
