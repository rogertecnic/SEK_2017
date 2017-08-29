/*
 * Sensorcor.cpp
 *
 *  Created on: 29 de ago de 2017
 *      Author: pcsek
 */

#include "Sensorcor.h"

Sensor_cor::Sensor_cor() {
	// TODO Auto-generated constructor stub

}

Sensor_cor::~Sensor_cor() {
	// TODO Auto-generated destructor stub
}



bool Sensor_cor::corEstaEntre(std::tuple<int, int, int> sample, RangesCor ranges){
	if(get<0>(sample) > ranges.r[0] && get<0>(sample) < ranges.r[1] &&				// verifica se o red ta dentro do range
			get<1>(sample) > ranges.g[0] && get<1>(sample) < ranges.g[1] &&			// verifica se o green ta denrto do range
			get<2>(sample) > ranges.b[0] && get<2>(sample) < ranges.b[1])			// verifica se o blue ta dentro do range
		return true;
	else return false;
}
