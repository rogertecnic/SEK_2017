#include <iostream>
#include "ev3dev.h"
#include "Motorcontrolado.h"
#include <unistd.h>
#include <thread>
#include <time.h>

using namespace std;

Motor_controlado motor(ev3dev::OUTPUT_A);

int main() {

	system("setfont Greek-TerminusBold20x10.psf.gz");
	//ev3dev::large_motor motor(ev3dev::OUTPUT_A);
	//motor.set_speed_sp(360);
	//motor.run_forever();
	//motor.run_velo_max();
	while(!ev3dev::button::enter.process()){
		cout<<clock()/CLOCKS_PER_SEC<<endl;
	}
	//usleep(3000000);

}
