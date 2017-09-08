#include <M_arquivos.h>
#include <Sensor_cor.h>
#include <unistd.h>
#include "ev3dev.h"
#include "src_folder/Controlador_motor.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Ultrassom_nxt.h"
#include "src_folder/Controlador_robo.h"
using namespace std;

//Controlador_motor rodaE(ev3dev::OUTPUT_A, 0.0408, 0.7, true,  "debug_lego_E.m");
//Controlador_motor rodaD(ev3dev::OUTPUT_B, 0.04055, 0.7, true,  "debug_lego_D.m");

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");

	/*
	rodaE.inicializa_thread();
	rodaD.inicializa_thread();


	rodaE.set_velo(0.3);
	rodaD.set_velo(0.3);

	usleep(5*1000000);

	rodaE.set_velo(0);
	rodaD.set_velo(0);
	//
	//	robots.finalizar_thread();
	//
	rodaE.finaliza_thread();
	rodaD.finaliza_thread();
	*/

	Controlador_robo robots;
	robots.inicializar_threads_motor();
	robots.inicializar_thread_aceleracao();

	cout << "Threads inicializadas" << endl;
	usleep(5*1000000);
	//while(!ev3dev::button::enter.process()){}

	robots.frente(0.3);
	usleep(10*1000000);

	robots.parar();

	cout << "Esperando... Finalizar threads" << endl;
	while(!ev3dev::button::enter.process()){}
	robots.finalizar_thread_aceleracao();
	robots.finalizar_threads_motor();


}
