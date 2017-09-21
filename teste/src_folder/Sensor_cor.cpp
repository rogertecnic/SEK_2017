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
{}


int Sensor_cor::ler_cor_E(){
	std::tuple<int, int, int> sample;
	sample = sensor_E.raw();
	int cor;
	if(corEstaEntre(sample, limites_Fora_E)) cor = Cor::fora; // 6
	else if(corEstaEntre(sample, limites_Preto_E)) cor = Cor::preto; // 1
	else if(corEstaEntre(sample, limites_Branco_E)) cor = Cor::branco; // 2
	else if(corEstaEntre(sample, limites_Vermelho_E)) cor = Cor::vermelho; // 3
	else if(corEstaEntre(sample, limites_Verde_E)) cor = Cor::verde; // 4
	else if(corEstaEntre(sample, limites_Azul_E)) cor = Cor::azul; // 5
	else return Cor::nda; //0

	for(int i = 2; i <= vezer_leu ; i++){
		usleep(1000*50);
		sample = sensor_E.raw();
		switch(cor){
		case Cor::fora:
			if(!corEstaEntre(sample, limites_Fora_E)) return Cor::nda;
			break;
		case Cor::preto:
			if(!corEstaEntre(sample, limites_Preto_E)) return Cor::nda;
			break;
		case Cor::branco:
			if(!corEstaEntre(sample, limites_Branco_E)) return Cor::nda;
			break;
		case Cor::vermelho:
			if(!corEstaEntre(sample, limites_Vermelho_E)) return Cor::nda;
			break;
		case Cor::verde:
			if(!corEstaEntre(sample, limites_Verde_E)) return Cor::nda;
			break;
		case Cor::azul:
			if(!corEstaEntre(sample, limites_Azul_E)) return Cor::nda;
			break;
		}
	}
	return cor;
}


int Sensor_cor::ler_cor_D(){
	std::tuple<int, int, int> sample;
	sample = sensor_D.raw();
	int cor;
	if(corEstaEntre(sample, limites_Fora_D)) cor = Cor::fora; // 6
	else if(corEstaEntre(sample, limites_Preto_D)) cor = Cor::preto; // 1
	else if(corEstaEntre(sample, limites_Branco_D)) cor = Cor::branco; // 2
	else if(corEstaEntre(sample, limites_Vermelho_D)) cor = Cor::vermelho; // 3
	else if(corEstaEntre(sample, limites_Verde_D)) cor = Cor::verde; // 4
	else if(corEstaEntre(sample, limites_Azul_D)) cor = Cor::azul; // 5
	else return Cor::nda; //0

	for(int i = 2; i <= vezer_leu ; i++){
		usleep(1000*50);
		sample = sensor_D.raw();
		switch(cor){
		case Cor::fora:
			if(!corEstaEntre(sample, limites_Fora_D)) return Cor::nda;
			break;
		case Cor::preto:
			if(!corEstaEntre(sample, limites_Preto_D)) return Cor::nda;
			break;
		case Cor::branco:
			if(!corEstaEntre(sample, limites_Branco_D)) return Cor::nda;
			break;
		case Cor::vermelho:
			if(!corEstaEntre(sample, limites_Vermelho_D)) return Cor::nda;
			break;
		case Cor::verde:
			if(!corEstaEntre(sample, limites_Verde_D)) return Cor::nda;
			break;
		case Cor::azul:
			if(!corEstaEntre(sample, limites_Azul_D)) return Cor::nda;
			break;
		}
	}
	return cor;
}


void Sensor_cor::calibra(){
	//*************************************************************************
	cout<<"calibra preto"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Preto_E, sensor_E);
		coleta_cor(&limites_Preto_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_preto, &limites_Preto_E);
	calibra_cor(RangesCor::range_preto, &limites_Preto_D);


	//*************************************************************************
	cout<<endl<<endl<<"calibra branco"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Branco_E, sensor_E);
		coleta_cor(&limites_Branco_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_branco, &limites_Branco_E);
	calibra_cor(RangesCor::range_branco, &limites_Branco_D);


	//*************************************************************************
	cout<<endl<<endl<<"calibra red"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Vermelho_E, sensor_E);
		coleta_cor(&limites_Vermelho_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_vermelho, &limites_Vermelho_E);
	calibra_cor(RangesCor::range_vermelho, &limites_Vermelho_D);


	//*************************************************************************
	cout<<endl<<endl<<"calibra green"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Verde_E, sensor_E);
		coleta_cor(&limites_Verde_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_verde, &limites_Verde_E);
	calibra_cor(RangesCor::range_verde, &limites_Verde_D);


	//*************************************************************************
	cout<<endl<<endl<<"calibra blue"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Azul_E, sensor_E);
		coleta_cor(&limites_Azul_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_azul, &limites_Azul_E);
	calibra_cor(RangesCor::range_azul, &limites_Azul_D);


	//*************************************************************************
	cout<<endl<<endl<<"calibra fora"<<endl;
	for(int i = 1; i <= vezes_calibrou;i++){
		cout<<i<<" vez;"<<endl;
		while(!ev3dev::button::enter.process()){};
		coleta_cor(&limites_Fora_E, sensor_E);
		coleta_cor(&limites_Fora_D, sensor_D);
		while(!ev3dev::button::enter.process()){};
		usleep(1000*200);
	}
	calibra_cor(RangesCor::range_fora, &limites_Fora_E);
	calibra_cor(RangesCor::range_fora, &limites_Fora_D);
}


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


void Sensor_cor::printa_rgb_E(){
	std::tuple<int, int, int> sample;
	sample = sensor_E.raw();
	cout<<get<0>(sample)<<";"<<get<1>(sample)<<";"<<get<2>(sample)<<endl;
}


bool Sensor_cor::corEstaEntre(std::tuple<int, int, int> sample, LimitesCor ranges){
	if(get<0>(sample) > ranges.r[0] && get<0>(sample) < ranges.r[1] &&				// verifica se o red ta dentro do range
			get<1>(sample) > ranges.g[0] && get<1>(sample) < ranges.g[1] &&			// verifica se o green ta denrto do range
			get<2>(sample) > ranges.b[0] && get<2>(sample) < ranges.b[1])			// verifica se o blue ta dentro do range
		return true;

	else return false;
}


void Sensor_cor::coleta_cor(LimitesCor *limites, ev3dev::color_sensor sensor){
	std::tuple<int, int, int> sample;
	sample = sensor.raw();
	limites->r[2] += get<0>(sample); // salvar os valores lidos somente
	limites->g[2] += get<1>(sample); // no primeiro elemento do vetor pois
	limites->b[2] += get<2>(sample); // esse valor eh a media, nao os ranges
}


void Sensor_cor::calibra_cor(const int  *range_cor, LimitesCor *limites){
	// calcular os ranges pegando a media da leitura do sensor em cada faixa (r,g,b)
	// que esta no primeiro elemento do vetor de range
	limites->r[0] = limites->r[2]/vezes_calibrou - range_cor[0];
	limites->r[1] = limites->r[2]/vezes_calibrou + range_cor[0];
	limites->g[0] = limites->g[2]/vezes_calibrou - range_cor[1];
	limites->g[1] = limites->g[2]/vezes_calibrou + range_cor[1];
	limites->b[0] = limites->b[2]/vezes_calibrou - range_cor[2];
	limites->b[1] = limites->b[2]/vezes_calibrou + range_cor[2];
}
