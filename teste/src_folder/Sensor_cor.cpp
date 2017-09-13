/*
 * Sensorcor.cpp
 *
 *  Created on: 29 de ago de 2017
 *      Author: pcsek
 */

#include "Sensor_cor.h"

Sensor_cor::Sensor_cor(string sensor_port)
:sensor(ev3dev::color_sensor(sensor_port))
{
}


Cor Sensor_cor::ler_cor(){
	std::tuple<int, int, int> sample;
		sample = sensor.raw();
		if(corEstaEntre(sample, r_Preto)) return Cor::preto;
		if(corEstaEntre(sample, r_Branco)) return Cor::branco;
		if(corEstaEntre(sample, r_Vermelho)) return Cor::vermelho;
		if(corEstaEntre(sample, r_Verde)) return Cor::verde;
		if(corEstaEntre(sample, r_Azul)) return Cor::azul;
		else{
			return Cor::nda;
		}
}


bool Sensor_cor::corEstaEntre(std::tuple<int, int, int> sample, RangesCor ranges){
	if(get<0>(sample) > ranges.r[0] && get<0>(sample) < ranges.r[1] &&				// verifica se o red ta dentro do range
			get<1>(sample) > ranges.g[0] && get<1>(sample) < ranges.g[1] &&			// verifica se o green ta denrto do range
			get<2>(sample) > ranges.b[0] && get<2>(sample) < ranges.b[1])			// verifica se o blue ta dentro do range
		return true;
	else return false;
}


void Sensor_cor::calibra(){
	std::tuple<int, int, int> rgb_Preto;
	std::tuple<int, int, int> rgb_Branco;
	std::tuple<int, int, int> rgb_Vermelho;
	std::tuple<int, int, int> rgb_Verde;
	std::tuple<int, int, int> rgb_Azul;

	cout<<"cal preto"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgb_Preto = sensor.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal branco"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgb_Branco = sensor.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal vermelho"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgb_Vermelho = sensor.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal  verde"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgb_Verde = sensor.raw();
	while(!ev3dev::button::enter.process()){};

	cout<<"cal azul"<<endl;
	while(!ev3dev::button::enter.process()){};
	rgb_Azul = sensor.raw();
	while(!ev3dev::button::enter.process()){};

	r_Preto.r[0] = get<0>(rgb_Preto) - RangesCor::range_preto;
	r_Preto.r[1] = get<0>(rgb_Preto) + RangesCor::range_preto;
	r_Preto.g[0] = get<1>(rgb_Preto) - RangesCor::range_preto;
	r_Preto.g[1] = get<1>(rgb_Preto) + RangesCor::range_preto;
	r_Preto.b[0] = get<2>(rgb_Preto) - RangesCor::range_preto;
	r_Preto.b[1] = get<2>(rgb_Preto) + RangesCor::range_preto;
	r_Branco.r[0] = get<0>(rgb_Branco) - RangesCor::range;
	r_Branco.r[1] = get<0>(rgb_Branco) + RangesCor::range;
	r_Branco.g[0] = get<1>(rgb_Branco) - RangesCor::range;
	r_Branco.g[1] = get<1>(rgb_Branco) + RangesCor::range;
	r_Branco.b[0] = get<2>(rgb_Branco) - RangesCor::range;
	r_Branco.b[1] = get<2>(rgb_Branco) + RangesCor::range;
	r_Vermelho.r[0] = get<0>(rgb_Vermelho) - RangesCor::range;
	r_Vermelho.r[1] = get<0>(rgb_Vermelho) + RangesCor::range;
	r_Vermelho.g[0] = get<1>(rgb_Vermelho) - RangesCor::range;
	r_Vermelho.g[1] = get<1>(rgb_Vermelho) + RangesCor::range;
	r_Vermelho.b[0] = get<2>(rgb_Vermelho) - RangesCor::range;
	r_Vermelho.b[1] = get<2>(rgb_Vermelho) + RangesCor::range;
	r_Verde.r[0] = get<0>(rgb_Verde) - RangesCor::range;
	r_Verde.r[1] = get<0>(rgb_Verde) + RangesCor::range;
	r_Verde.g[0] = get<1>(rgb_Verde) - RangesCor::range;
	r_Verde.g[1] = get<1>(rgb_Verde) + RangesCor::range;
	r_Verde.b[0] = get<2>(rgb_Verde) - RangesCor::range;
	r_Verde.b[1] = get<2>(rgb_Verde) + RangesCor::range;
	r_Azul.r[0] = get<0>(rgb_Azul) - RangesCor::range;
	r_Azul.r[1] = get<0>(rgb_Azul) + RangesCor::range;
	r_Azul.g[0] = get<1>(rgb_Azul) - RangesCor::range;
	r_Azul.g[1] = get<1>(rgb_Azul) + RangesCor::range;
	r_Azul.b[0] = get<2>(rgb_Azul) - RangesCor::range;
	r_Azul.b[1] = get<2>(rgb_Azul) + RangesCor::range;


	while(!ev3dev::button::enter.process()){};
	while(!ev3dev::button::enter.process()){};
	cout<<endl<<endl<<endl<<endl;
}
