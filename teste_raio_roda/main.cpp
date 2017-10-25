/* main.cpp:
 *
 */

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include "M_arquivos.h"
#include "ev3dev.h"
#include "src_folder/Controlador_robo.h"

using namespace std;


int main(){

	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	robo.inicializar_thread_aceleracao();
	robo.andar(30,2);
	robo.finalizar_thread_aceleracao();
}
