#ifndef ULTRA_SOM_H_
#define ULTRA_SOM_H_

#include <thread>
#include <chrono>
#include <unistd.h>
#include <math.h>
#include <M_arquivos.h>
#include "ev3dev.h"

class Ultra_Som {
public:
	Ultra_Som(string sensor_port, double dist_max, double dist_min);
	double le_distancia();

private:
	thread thread_US; // objeto da thread
	ev3dev::ultrasonic_sensor US;
	double dist_max;
	double dist_min;
};
#endif /* ULTRA_SOM_H_ */
