#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include "M_arquivos.h"
#include "ev3dev.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Controlador_robo.h"


using namespace std;

enum estados{faixa, leu_nda, leu_fora, intersec};

typedef chrono::high_resolution_clock Time;

bool colorido(Sensor_cor cor, string lado){
	if(lado == "esquerdo"){
		if(cor.ler_cor_E() != Cor::branco && cor.ler_cor_E() != Cor::fora && cor.ler_cor_E() != Cor::nda)
			return true;
	}
	else {
		if(cor.ler_cor_D() != Cor::branco && cor.ler_cor_D() != Cor::fora && cor.ler_cor_D() != Cor::nda)
			return true;
	}
	return false;
}


void teste_luana_alinhamento(){
	Controlador_robo robot(true, "fodase.m");
	Sensor_cor cor(ev3dev::INPUT_1, ev3dev::INPUT_2);
	robot.inicializar_thread_aceleracao();

	int count_nda = 0;
	estados estd;

	int cor_E, cor_D;
	double 	dist = 0, ang_robo = 0, posicao_inicial = 0, posicao_final = 0;

	robot.andar(70);
	estd = estados::faixa;
	while(!ev3dev::button::back.process()){
		switch (estd){
		case 0: //Caso estiver andando na faixa
			robot.andar(70);
			cout << "Estado: " << estd << endl;
			if (cor.ler_cor_E() == Cor::nda || cor.ler_cor_D() == Cor::nda)
				estd = estados::leu_nda;
			else if (cor.ler_cor_E() == Cor::fora || cor.ler_cor_D() == Cor::fora)
				estd = estados::leu_fora;
			else if( colorido(cor, "esquerdo") || (colorido(cor, "direito")))
				estd = estados::intersec;

			break;

		case 1: //Caso ler nada
			cout << "Estado: " << estd << endl;
			if(cor.ler_cor_D() == Cor::nda || cor.ler_cor_E() == Cor::nda)
				count_nda ++;

			else{
				count_nda = 0;
				if(cor.ler_cor_E() == Cor::branco && cor.ler_cor_D() == Cor::branco)
					estd = estados::faixa;
				else if (cor.ler_cor_E() == Cor::fora || cor.ler_cor_D() == Cor::fora)
					estd = estados::leu_fora;
				else if( (colorido(cor, "esquerdo")) && (colorido(cor, "direito")) )
					estd = estados::intersec;

			}

			if(count_nda >= 10){
				cout<<"viu nda 10"<<endl;
				robot.parar();
				robot.andar(-100);
				usleep(1000*800);
				robot.girar(30);
				robot.andar(70);
			}

			break;

		case 2: //Caso ler fora
			cout << "Estado: " << estd << endl;
			if(cor.ler_cor_E() == Cor::fora && cor.ler_cor_D() == Cor::branco){
				cout<<"saiu E"<<endl;
				robot.parar();
				robot.andar(-40);
				usleep(1000*1000);
				robot.girar(-10);
				while(robot.get_estado() == flag_aceleracao::girar);
				robot.andar(70);
			}


			else if(cor.ler_cor_D() == Cor::fora && cor.ler_cor_E() == Cor::branco){
				cout<<"saiu D"<<endl;
				robot.parar();
				robot.andar(-40);
				usleep(1000*1000);
				robot.girar(10);
				while(robot.get_estado() == flag_aceleracao::girar);
				robot.andar(70);
			}

			estd = estados::faixa;

			break;

		case 3: //Caso entrar em uma intersecção
			cout << "Estado: " << estd << endl;
			robot.andar(30);
			usleep(1000*100);
			cor_E = cor.ler_cor_E();
			cor_D = cor.ler_cor_D();

			if( (colorido(cor, "esquerdo")) || (colorido(cor, "esquerdo"))) {
				cout<<"viu cor"<<endl;
				posicao_inicial = robot.get_distancia();
				while(robot.get_distancia() < posicao_inicial + 0.04);
				robot.parar();

				if(cor.ler_cor_E() != cor_E){
					cout<<"saiu E"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(-10);
					while(robot.get_estado() == flag_aceleracao::girar);
					robot.andar(70);
				}

				else if(cor.ler_cor_D() != cor_D){
					cout<<"saiu D"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(10);
					while(robot.get_estado() == flag_aceleracao::girar);
					robot.andar(70);
				}


				else{ // esta dentro
					robot.alinhar_para_traz(&cor);
					robot.andar(50, 0.195);
					robot.girar(90);
					while(robot.get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robot.andar(70);
					while(cor.ler_cor_E() != Cor::branco && cor.ler_cor_D() != Cor::branco);
					robot.alinhar_para_traz(&cor);
					robot.andar(70);
					while(cor.ler_cor_E() == cor_E || cor.ler_cor_D() == cor_D);
					usleep(1000*500);

					estd = estados::faixa;
				}
			}

			break;



		}
	}

	robot.finalizar_thread_aceleracao();
	usleep(0.5*1000000);
}



void teste_rogerio_alinhamento(){
	Controlador_robo robot(true, "debug posicao direto no pwm.m"); // fator_croda = 1.005
	Sensor_cor cor(ev3dev::INPUT_1, ev3dev::INPUT_2);
	robot.inicializar_thread_aceleracao();

	double 	dist = 0,
			ang_robo = 0,
			posicao_inicial = 0,
			posicao_final = 0;

	int i = 0; //Contador de identificação do estado de fora da pista

	robot.andar(70);
	while(!ev3dev::button::back.process()){
		if(cor.ler_cor_D() == Cor::nda || cor.ler_cor_E() == Cor::nda)
			i ++;

		else i = 0;

		if(i >=10){
			cout<<"viu nda 10"<<endl;
			robot.parar();
			robot.andar(-100);
			usleep(1000*800);
			robot.girar(30);
			robot.andar(70);
		}
		//***********termina verificacao da cor "nda"


		robot.andar(70);

		/******************************************/
		if(cor.ler_cor_E() == Cor::fora){
			cout<<"saiu E"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*1000);
			robot.girar(-10);
			while(robot.get_estado() == flag_aceleracao::girar);
			robot.andar(70);
		}


		if(cor.ler_cor_D() == Cor::fora){
			cout<<"saiu D"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*1000);
			robot.girar(10);
			while(robot.get_estado() == flag_aceleracao::girar);
			robot.andar(70);
		}
		/******************************************/

		/******************************************
		if((cor.ler_cor_E() == Cor::nda || cor.ler_cor_E() == Cor::fora) &&
				(cor.ler_cor_D() != Cor::branco && cor.ler_cor_D() != Cor::nda && cor.ler_cor_D() != Cor::fora))
		{
			usleep(0.5*1000000);
			if((cor.ler_cor_E() == Cor::nda || cor.ler_cor_E() == Cor::fora) &&
					(cor.ler_cor_D() != Cor::branco && cor.ler_cor_D() != Cor::nda && cor.ler_cor_D() != Cor::fora))
			{
				cout << "Correção intersec" << endl;
				robot.parar();
				robot.andar(-40);
				usleep(1000*1000);
				robot.girar(-10);
				while(robot.get_estado() == flag_aceleracao::girar);
				robot.andar(70);
			}
		}

		if((cor.ler_cor_D() == Cor::nda || cor.ler_cor_D() == Cor::fora)&&
				(cor.ler_cor_E() != Cor::branco || cor.ler_cor_E() != Cor::nda || cor.ler_cor_E() != Cor::fora))
		{

			usleep(0.5*1000000);
			if((cor.ler_cor_D() == Cor::nda || cor.ler_cor_D() == Cor::fora)&&
					(cor.ler_cor_E() != Cor::branco || cor.ler_cor_E() != Cor::nda || cor.ler_cor_E() != Cor::fora))
			{
				cout << "Correção intersec" << endl;
				robot.parar();
				robot.andar(-40);
				usleep(1000*1000);
				robot.girar(10);
				while(robot.get_estado() == flag_aceleracao::girar);
				robot.andar(70);
			}
		}

		/******************************************/


		// alinhar usando o metodo da classe robo
		int cor_E = cor.ler_cor_E();
		int cor_D = cor.ler_cor_D();

		if( (cor_E != Cor::branco && cor_E != Cor::fora && cor_E != Cor::nda) ||
				(cor_D != Cor::branco && cor_D != Cor::fora && cor_D != Cor::nda))
		{
			robot.andar(30);
			usleep(1000*100);
			cor_E = cor.ler_cor_E();
			cor_D = cor.ler_cor_D();
			if( (cor_E != Cor::branco && cor_E != Cor::fora && cor_E != Cor::nda) ||
					(cor_D != Cor::branco && cor_D != Cor::fora && cor_D != Cor::nda))
			{
				cout<<"viu cor"<<endl;
				posicao_inicial = robot.get_distancia();
				while(robot.get_distancia() < posicao_inicial + 0.04);
				robot.parar();

				if(cor.ler_cor_E() != cor_E){
					cout<<"saiu E"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(-10);
					while(robot.get_estado() == flag_aceleracao::girar);
					robot.andar(70);
				}

				else if(cor.ler_cor_D() != cor_D){
					cout<<"saiu D"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(10);
					while(robot.get_estado() == flag_aceleracao::girar);
					robot.andar(70);
				}


				else{ // esta dentro
					robot.alinhar_para_traz(&cor);
					robot.andar(50, 0.195);
					robot.girar(90);
					while(robot.get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					while(cor.ler_cor_E() != Cor::branco && cor.ler_cor_D() != Cor::branco) robot.andar(70);
					robot.alinhar_para_traz(&cor);
					robot.andar(70);
					while(cor.ler_cor_E() == cor_E || cor.ler_cor_D() == cor_D);
					usleep(1000*500);
				}
			}
		}





	}

	robot.finalizar_thread_aceleracao();
	usleep(0.5*1000000);
}



int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	teste_luana_alinhamento();


	cout << "Teste finalizado. Bye!" << endl;
	usleep (1000000);
	return 0;
}
