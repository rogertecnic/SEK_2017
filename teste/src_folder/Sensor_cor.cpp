/*
 * Sensorcor.cpp
 *
 *  Created on: 29 de ago de 2017
 *      Author: pcsek
 */

#include "Sensor_cor.h"
#include <math.h>
#include <list>
#include <vector>

Sensor_cor::Sensor_cor(string sensor_port_E, string sensor_port_D)
:sensor_E(ev3dev::color_sensor(sensor_port_E)),
 sensor_D(ev3dev::color_sensor(sensor_port_D))
{
}


Cor Sensor_cor::ler_cor_E(){
	std::tuple<int, int, int> sample;
	sample = sensor_E.raw();
	if(corEstaEntre(sample, limites_Preto_E)) return Cor::preto;
	if(corEstaEntre(sample, limites_Branco_E)) return Cor::branco;
	if(corEstaEntre(sample, limites_Vermelho_E)) return Cor::vermelho;
	if(corEstaEntre(sample, limites_Verde_E)) return Cor::verde;
	if(corEstaEntre(sample, limites_Azul_E)) return Cor::azul;
	if(corEstaEntre(sample, limites_Fora_E)) return Cor::fora;
	else{
		return Cor::nda;
	}
}


Cor Sensor_cor::ler_cor_D(){
	std::tuple<int, int, int> sample;
	sample = sensor_D.raw();
	if(corEstaEntre(sample, limites_Preto_D)) return Cor::preto;
	if(corEstaEntre(sample, limites_Branco_D)) return Cor::branco;
	if(corEstaEntre(sample, limites_Vermelho_D)) return Cor::vermelho;
	if(corEstaEntre(sample, limites_Verde_D)) return Cor::verde;
	if(corEstaEntre(sample, limites_Azul_D)) return Cor::azul;
	if(corEstaEntre(sample, limites_Fora_D)) return Cor::fora;
	else{
		return Cor::nda;
	}
}


void Sensor_cor::calibra(){
	cout<<"calibra preto"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_preto, &limites_Preto_E, sensor_E);
	calibra_cor(RangesCor::range_preto, &limites_Preto_D, sensor_D);
	while(!ev3dev::button::enter.process()){};

	cout<<"calibra branco"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_branco, &limites_Branco_E, sensor_E);
	calibra_cor(RangesCor::range_branco, &limites_Branco_D, sensor_D);
	while(!ev3dev::button::enter.process()){};

	cout<<"calibra red"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_vermelho, &limites_Vermelho_E, sensor_E);
	calibra_cor(RangesCor::range_vermelho, &limites_Vermelho_D, sensor_D);
	while(!ev3dev::button::enter.process()){};

	cout<<"calibra green"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_verde, &limites_Verde_E, sensor_E);
	calibra_cor(RangesCor::range_verde, &limites_Verde_D, sensor_D);
	while(!ev3dev::button::enter.process()){};

	cout<<"calibra blue"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_azul, &limites_Azul_E, sensor_E);
	calibra_cor(RangesCor::range_azul, &limites_Azul_D, sensor_D);
	while(!ev3dev::button::enter.process()){};

	cout<<"calibra fora"<<endl;
	while(!ev3dev::button::enter.process()){};
	calibra_cor(RangesCor::range_fora, &limites_Fora_E, sensor_E);
	calibra_cor(RangesCor::range_fora, &limites_Fora_D, sensor_D);
	while(!ev3dev::button::enter.process()){};
}

//TODO metodo define_ranges tem que ser feito, nao utilizalo
void Sensor_cor::define_ranges(){
	list<vector<int>> valores_E,
	valores_D;
	list<vector<int>>::iterator it1;

	double med_E[3] = {0,0,0},
			med_D[3]= {0,0,0},
			var_E[3] = {0,0,0},
			var_D[3] = {0,0,0};
	int i = 0;
	std::tuple<int, int, int> sample;
	vector<int> valores;
	usleep(1000.500);
	cout<<"calc ranges"<<endl;
	while(!ev3dev::button::enter.process()){
		i++;
		sample = sensor_E.raw();
		valores = {get<0>(sample), get<1>(sample), get<2>(sample)};
		valores_E.push_back(valores);
		med_E[0] += get<0>(sample);
		med_E[1] += get<1>(sample);
		med_E[2] += get<2>(sample);
		sample = sensor_D.raw();
		valores = {get<0>(sample), get<1>(sample), get<2>(sample)};
		valores_D.push_back(valores);
		med_D[0] += get<0>(sample);
		med_D[1] += get<1>(sample);
		med_D[2] += get<2>(sample);
	}
	usleep(1000*500);
	ev3dev::button::enter.process();
	med_E[0] = med_E[0]/i;
	med_E[1] = med_E[1]/i;
	med_E[2] = med_E[2]/i;
	med_D[0] = med_D[0]/i;
	med_D[1] = med_D[1]/i;
	med_D[2] = med_D[2]/i;

	valores = {0,0,0};
	for(int j = i ; j > 0 ; j--){
		it1 = valores_E.begin();
		valores = *it1;
		valores_E.erase(it1);
		if(var_E[0] < abs(med_E[0] - valores[0]))
			var_E[0] = abs(med_E[0] - valores[0]);
		if(var_E[1] < abs(med_E[1] - valores[1]))
			var_E[1] = abs(med_E[1] - valores[1]);
		if(var_E[2] < abs(med_E[2] - valores[2]))
			var_E[2] = abs(med_E[2] - valores[2]);

		it1 = valores_D.begin();
		valores = *it1;
		valores_D.erase(it1);
		if(var_D[0] < abs(med_D[0] - valores[0]))
			var_D[0] = abs(med_D[0] - valores[0]);
		if(var_D[1] < abs(med_D[1] - valores[1]))
			var_D[1] = abs(med_D[1] - valores[1]);
		if(var_D[2] < abs(med_D[2] - valores[2]))
			var_D[2] = abs(med_D[2] - valores[2]);
	}
	cout<<var_E[0]<<";"<<var_D[0]<<endl;
	cout<<var_E[1]<<";"<<var_D[1]<<endl;
	cout<<var_E[2]<<";"<<var_D[2]<<endl;
}


bool Sensor_cor::corEstaEntre(std::tuple<int, int, int> sample, RangesCor ranges){
	if(get<0>(sample) > ranges.r[0] && get<0>(sample) < ranges.r[1] &&				// verifica se o red ta dentro do range
			get<1>(sample) > ranges.g[0] && get<1>(sample) < ranges.g[1] &&			// verifica se o green ta denrto do range
			get<2>(sample) > ranges.b[0] && get<2>(sample) < ranges.b[1])			// verifica se o blue ta dentro do range
		return true;
	else return false;
}


void Sensor_cor::calibra_cor(int range_cor, RangesCor *limites, ev3dev::color_sensor sensor){
	std::tuple<int, int, int> sample;
	sample = sensor.raw();
	limites->r[0] = get<0>(sample) - range_cor;
	limites->r[1] = get<0>(sample) + range_cor;
	limites->g[0] = get<1>(sample) - range_cor;
	limites->g[1] = get<1>(sample) + range_cor;
	limites->b[0] = get<2>(sample) - range_cor;
	limites->b[1] = get<2>(sample) + range_cor;
}
