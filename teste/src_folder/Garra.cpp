#include "Garra.h"


Garra::Garra(string motor_port, int graus_abertura_do_motor, string tipo)
:garra(ev3dev::large_motor(motor_port)), graus_abertura_do_motor(graus_abertura_do_motor), tipo(tipo)
{
	garra.reset();
	garra.set_stop_action("hold");

	if(tipo == "cancela")
		garra.set_speed_sp(360);

	else if(tipo == "garra")
		garra.set_speed_sp(360+360);


}

void Garra::abrir() {
	if(!garra_aberta){
		garra.set_stop_action("hold");
		garra_aberta = true;
		garra.set_position_sp(-graus_abertura_do_motor);
		garra.run_to_abs_pos();
	}
	usleep(1000000);

}

void Garra::fechar() {
	if(garra_aberta){
		if(tipo == "cancela"){
			garra.set_stop_action("hold");
			garra.set_position_sp(0);
		}
		else if(tipo == "garra"){
			garra.set_stop_action("hold");
			garra.set_position_sp(graus_abertura_do_motor);
		}
		garra_aberta = false;
		garra.run_to_abs_pos();
	}
	usleep(1000000);
}
