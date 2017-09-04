#include <Ultra_Som.h>

//TODO terminar o .cpp do ultrassom
Ultra_Som::Ultra_Som(string sensor_port, double dist_max, double dist_min)
: dist_max(dist_max),
  dist_min(dist_min),
  US(ev3dev::ultrasonic_sensor(sensor_port))
{
}

double Ultra_Som::le_distancia(){
	return US.distance_centimeters();
}
