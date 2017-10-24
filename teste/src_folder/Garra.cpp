#include "Garra.h"


Garra::Garra(string motor_port, int graus_abertura_do_motor)
:garra(ev3dev::large_motor(motor_port)), graus_abertura_do_motor(graus_abertura_do_motor)
{
	garra.reset();
	garra.set_stop_action("hold");
	garra.set_speed_sp(180);
}

void Garra::abrir() {
	if(!garra_aberta){
		garra.set_stop_action("hold");
		garra_aberta = true;
		garra.set_position_sp(-graus_abertura_do_motor);
		garra.run_to_abs_pos();
	}
}

void Garra::fechar() {
	if(garra_aberta){
		garra.set_stop_action("coast");
		garra_aberta = false;
		garra.set_position_sp(0);
		garra.run_to_abs_pos();
	}
}
