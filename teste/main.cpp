#include <iostream>
#include <unistd.h>
#include <chrono>
#include <string>
#include "M_arquivos.h"
#include "ev3dev.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Sensor_cor_hsv.h"
#include "src_folder/Controlador_robo.h"
#include "src_folder/Mapeamento.h"
#include "src_folder/Resgate.h"


using namespace std;

//enum estados{faixa, leu_nda, leu_fora, intersec};

typedef chrono::high_resolution_clock Time;



bool colorido(Sensor_cor_hsv cor, string lado){
	if(lado == "esquerdo"){
		if(cor.ler_cor_E() != Cor::branco && cor.ler_cor_E() != Cor::fora && cor.ler_cor_E() != Cor::ndCor)
			return true;
	}
	else {
		if(cor.ler_cor_D() != Cor::branco && cor.ler_cor_D() != Cor::fora && cor.ler_cor_D() != Cor::ndCor)
			return true;
	}
	return false;
}

bool colorido(int cor, string lado){
	if(lado == "esquerdo"){
		if(cor != Cor::branco && cor != Cor::fora && cor != Cor::ndCor)
			return true;
	}
	else {
		if(cor != Cor::branco && cor != Cor::fora && cor != Cor::ndCor)
			return true;
	}
	return false;
}


void teste_luana_alinhamento(){
	Controlador_robo robot(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2);

	robot.calibra_sensor_cor(&cor);
	robot.inicializar_thread_aceleracao();

	cout << "Calibração terminada!" << endl;
	usleep(2*1000000);
	while(!ev3dev::button::enter.process());
	usleep(0.3*1000000);
	ev3dev::button::enter.process();

	estados_arena estd;

	int count_nda = 0;

	int cor_E, cor_D;
	double 	dist = 0, ang_robo = 0, posicao_inicial = 0, posicao_final = 0;


	robot.andar(70);
	estd = estados_arena::faixa;

	while(!ev3dev::button::back.process()){
		switch (estd){
		case 0: //Caso estiver andando na faixa
			robot.andar(70);
			cout << "Estado: " << estd << endl;
			if (cor.ler_cor_E() == Cor::ndCor || cor.ler_cor_D() == Cor::ndCor)
				estd = estados_arena::leu_nda;
			else if( (cor.ler_cor_E() != Cor::fora && cor.ler_cor_E() != Cor::ndCor && cor.ler_cor_E() != Cor::branco) ||
					(cor.ler_cor_D() != Cor::fora && cor.ler_cor_D() != Cor::ndCor && cor.ler_cor_D() != Cor::branco) )
				estd = estados_arena::intersec;
			else if (cor.ler_cor_E() == Cor::fora || cor.ler_cor_D() == Cor::fora)
				estd = estados_arena::leu_fora;


			break;

		case 1: //Caso ler nada
			cout << "Estado: " << estd << endl;
			if(cor.ler_cor_D() == Cor::ndCor || cor.ler_cor_E() == Cor::ndCor)
				count_nda ++;

			else{
				count_nda = 0;

				if (cor.ler_cor_E() == Cor::fora || cor.ler_cor_D() == Cor::fora)
					estd = estados_arena::leu_fora;
				else if( (cor.ler_cor_E() != Cor::fora && cor.ler_cor_E() != Cor::ndCor && cor.ler_cor_E() != Cor::branco) ||
						(cor.ler_cor_D() != Cor::fora && cor.ler_cor_D() != Cor::ndCor && cor.ler_cor_D() != Cor::branco) )
					estd = estados_arena::intersec;
				else if(cor.ler_cor_E() == Cor::branco || cor.ler_cor_D() == Cor::branco)
					estd = estados_arena::faixa;

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

			estd = estados_arena::faixa;

			break;

		case 3: //Caso entrar em uma intersecção
			cout << "Estado: " << estd << endl;
			robot.andar(30);
			usleep(1000*100);
			cor_E = cor.ler_cor_E();
			cor_D = cor.ler_cor_D();

			if( (cor.ler_cor_E() != Cor::fora && cor.ler_cor_E() != Cor::ndCor && cor.ler_cor_E() != Cor::branco) ||
					(cor.ler_cor_D() != Cor::fora && cor.ler_cor_D() != Cor::ndCor && cor.ler_cor_D() != Cor::branco) ) {
				cout<<"viu cor"<<endl;
				posicao_inicial = robot.get_distancia_linha_reta();
				while(robot.get_distancia_linha_reta() < posicao_inicial + 0.04);
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
					robot.alinhar(&cor, direcao::traz);
					robot.andar(50, 0.195);
					robot.girar(90);
					while(robot.get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robot.andar(50, 0.19);
					robot.alinhar(&cor, direcao::traz);
					robot.andar(70);
					while(cor.ler_cor_E() == cor_E || cor.ler_cor_D() == cor_D);
					usleep(1000*500);

					estd = estados_arena::faixa;
				}
			}

			break;
		}
	}

	robot.finalizar_thread_aceleracao();
	usleep(0.2*1000000);
}



void teste_rogerio(){
	Controlador_robo robot(false, "debug posicao direto no pwm.m"); // fator_croda = 1.005
	//Sensor_cor cor(ev3dev::INPUT_1, ev3dev::INPUT_2);
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2,false,"leitura_sensor_cor_hsv");
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);
	Ultrassom_nxt ultraD(Ultrassom_nxt::INPUT_4);
	Mapeamento mapa(&robot, &cor, &ultraE, &ultraD);
	Resgate resgate(&robot, &cor, &ultraE, &ultraD, ev3dev::OUTPUT_D);


	robot.inicializar_thread_aceleracao();
	/*
	 * teste da classe controlador_robo
	 */
	//	while(!ev3dev::button::enter.process());
	//	usleep(1000000*0.1);
	//	while(!ev3dev::button::enter.process());
	//	robot.andar(50, 0.3);
	//	cout << robot.get_distancia_absoluta() << ";" << robot.get_distancia_linha_reta() << endl;
	//	usleep(1000000*5);
	//	robot.andar(50, 0.3);
	//	cout << robot.get_distancia_absoluta() << ";" << robot.get_distancia_linha_reta() << endl;
	//	while(!ev3dev::button::enter.process())
	//		cout<<robot.get_velocidade()<<endl;
	//	usleep(1000*2000);
	//	robot.girar(360*2);
	//	usleep(1000*2000);
	//	robot.andar(50);
	//	usleep(1000*2000);
	//	robot.andar(-50);
	//	usleep(1000*2000);
	//	robot.girar(-360);
	//while(!ev3dev::button::enter.process());

	/*
	 * teste classe controlador_robo medir o tamanho do pintao
	 */
	//	while(!ev3dev::button::enter.process());
	//	usleep(1000000*0.1);
	//	while(!ev3dev::button::enter.process());
	//	robot.andar(50);
	//	while(cor.ler_cor_E() == Cor::branco);
	//	robot.parar();
	//	robot.andar(20, 0.058); // o tamanho do pintao eh 0.058
	//	robot.andar(20, 0.15); // testando se ele vai parar no meio mesmo
	//	robot.girar(-90);

	/*
	 * teste classe mapeamento com classe sensor cor hsv
	 */
	//			robot.calibra_sensor_cor(&cor);
	//			while(!ev3dev::button::enter.process());
	//			usleep(1000000*0.1);
	//			while(!ev3dev::button::enter.process());
	//			mapa.mapear();
	//			resgate.resgatar();


	/*
	 * teste do alinhamento do portal
	 */
	robot.calibra_sensor_cor(&cor);
	while(!ev3dev::button::enter.process());
	usleep(1000000*0.1);
	while(!ev3dev::button::enter.process());
	robot.alinha_portal(&cor);


	/*
	 * teste classe Garra
	 */
	//	Garra garra(ev3dev::OUTPUT_D, 45);
	//	while(!ev3dev::button::enter.process());
	//	usleep(1000000*0.1);
	//	while(!ev3dev::button::enter.process());
	//	garra.abrir();
	//	usleep(1000000*1);
	//	garra.fechar();
	//	usleep(1000000*1);
	//	robot.andar(60);
	//	while(!ev3dev::button::enter.process());
	//	usleep(1000000*0.1);
	//	while(!ev3dev::button::enter.process());
	//	robot.parar();

	/*
	 * teste da classe sensor_cor
	 * imprime o que o sensor esta lendo usando a classe
	 */
	//cor.calibra();
	//	while(!ev3dev::button::up.process()){
	//		while(!ev3dev::button::enter.process()){}
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//		robot.andar(30);
	//		while(!ev3dev::button::enter.process())
	//			cout<<cor.ler_cor_E()<<";"<<cor.ler_cor_D()<<endl;
	//		robot.parar();
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//	}


	/*
	 * teste da classe sensor_cor_hsv
	 * pega alguns valores e guarda no arquivo
	 */
	//	while(!ev3dev::button::up.process()){
	//	while(!ev3dev::button::enter.process()){}
	//	usleep(1000*800);
	//	ev3dev::button::enter.process();
	//	robot.andar(30);
	//	while(!ev3dev::button::enter.process())
	//		cout<<cor.ler_cor_E()<<";"<<cor.ler_cor_D()<<endl;
	//	robot.parar();
	//	usleep(1000*800);
	//	ev3dev::button::enter.process();
	//	}
	//cor.fecha_arquivo();

	/*
	 * teste da classe de cor
	 * define os ranges de quanto a leitura da cor varia
	 */
	//	while(!ev3dev::button::back.process()){
	//		while(!ev3dev::button::enter.process()){}
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//		robot.andar(30);
	//		cor.define_ranges();
	//		robot.parar();
	//		usleep(1000*800);
	//		ev3dev::button::enter.process();
	//	}


	/*
	 * teste da classe de cor
	 * imprime a leitura rgb do sensor esquerdo
	 */
	//	while(!ev3dev::button::back.process()){
	//		cor.printa_rgb_E();
	//	}


	/*
	 * VERIFICA SE A VELOCIDADE LIDA EH A VELOCIDADE REAL
	 * RESULTADOS BEM SATISFATORIOS, PRECISAO DE 0.001 m/s
	 */
	//	double 	velo_robo = 0,
	//			dist = 0,
	//			ang_robo = 0,
	//			posicao_inicial = 0;
	//	chrono::system_clock::time_point t_inicial;
	//	chrono::system_clock::time_point t_final;
	//	chrono::duration<double> delta_t; //Usar dt = t1-t2 e dt.count() para pegar o tempo em seg
	//	t_inicial = Time::now();
	//	robot.andar(70);
	//	while(robot.get_distancia() < 3){}
	//	velo_robo = robot.get_velocidade();
	//	robot.parar();
	//	t_final = Time::now();
	//	delta_t = t_final - t_inicial;
	//	cout<<3/delta_t.count()<<endl;
	//	cout<<velo_robo<<endl;


	/*
	 * TESTE ANDAR RE
	 */
	//robot.andar(-50, 0.5);


	/*
	 * TESTE COM SENSOR TOUCH PARA VERIFICAR TEMPO
	 */
	//	ev3dev::touch_sensor toque(ev3dev::INPUT_1);
	//	while(!ev3dev::button::enter.process()){
	//		//while(!toque.is_pressed()){}
	//		t_inicial = Time::now();
	//		//toque.is_pressed();
	//		//toque.is_pressed();
	//		//toque.is_pressed();
	//		usleep(1000*1000);
	//		//while(toque.is_pressed()){}
	//		t_final = Time::now();
	//		delta_t = t_final - t_inicial;
	//		cout<<delta_t.count()<<endl;
	//		usleep(1000*3000);
	//	}

	while(!ev3dev::button::enter.process());
	robot.parar();
	usleep(1000*500);
	robot.finalizar_thread_aceleracao();
}


void teste_rogerio_alinhamento(){
	Controlador_robo robot(true, "debug posicao direto no pwm.m"); // fator_croda = 1.005
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2);

	robot.calibra_sensor_cor(&cor);
	robot.inicializar_thread_aceleracao();

	double 	dist = 0,
			ang_robo = 0,
			posicao_inicial = 0,
			posicao_final = 0;

	int i = 0;
	//cor.calibra();
	while(!ev3dev::button::back.process()){
		// verifica se o sensor esta vendo "nda" por um longo tempo
		if(cor.ler_cor_D() == Cor::ndCor || cor.ler_cor_E() == Cor::ndCor){
			i ++;
		}else i = 0;
		if(i >=10){
			cout<<"viu nda 10"<<endl;
			robot.parar();
			robot.andar(-100);
			usleep(1000*800);
			robot.girar(30);
		}
		//***********termina verificacao da cor "nda"


		robot.andar(70);
		// forma de alinhamento calculando o angulo que o robo ta torto
		// se o sensor E ver preto
		//		if(cor.ler_cor_E() == Cor::preto){ // eh preto mesmo ou fora, verificar
		//			posicao_inicial = robot.get_distancia();
		//			robot.andar(30); // velocidade de atencao
		//			cout<<"preto E?";
		//			while(true){
		//				if(cor.ler_cor_E() == Cor::fora){
		//					cout<<"nao;"<<endl<<"saiu E"<<endl;
		//					robot.parar();
		//					robot.andar(-40);
		//					usleep(1000*2000);
		//					robot.girar(-35);
		//					robot.andar(70);
		//					break;
		//				}else if(cor.ler_cor_D() == Cor::preto ||
		//						cor.ler_cor_D() == Cor::fora){
		//					posicao_final = robot.get_distancia();
		//					cout<<"SIM!"<<endl;
		//					while(robot.get_distancia() < posicao_final + 0.18){}
		//					robot.parar();
		//					dist = posicao_final - posicao_inicial;
		//					ang_robo = atan2(dist, 0.17);
		//					robot.girar(-90+ang_robo*180/3.141592);
		//					while(robot.get_estado() == flag_aceleracao::girar){}
		//					while(cor.ler_cor_E() != preto) robot.girar(-30);
		//					while(cor.ler_cor_D() != preto) robot.girar(30);
		//					robot.andar(70);
		//					while(cor.ler_cor_D() == Cor::preto ||
		//							cor.ler_cor_E() == Cor::preto){}
		//					break;
		//				}
		//			}
		//		}
		//
		//		// se o sensor D ver preto
		//		if(cor.ler_cor_D() == Cor::preto){ // eh preto mesmo ou fora, verificar
		//			posicao_inicial = robot.get_distancia();
		//			cout<<"preto D?";
		//			robot.andar(30); // velocidade de atencao
		//			while(true){
		//				if(cor.ler_cor_D() == Cor::fora){
		//					cout<<"nao;"<<endl<<"saiu D"<<endl;
		//					robot.parar();
		//					robot.andar(-40);
		//					usleep(1000*2000);
		//					robot.girar(-35);
		//					robot.andar(70);
		//					break;
		//				}else if(cor.ler_cor_E() == Cor::preto ||
		//						cor.ler_cor_E() == Cor::fora){
		//					posicao_final = robot.get_distancia();
		//					cout<<"SIM!"<<endl;
		//					while(robot.get_distancia() < posicao_final + 0.18){}
		//					robot.parar();
		//					dist = posicao_final - posicao_inicial;
		//					ang_robo = atan2(dist, 0.17);
		//					robot.girar(-90+ang_robo*180/3.141592);
		//					while(robot.get_estado() == flag_aceleracao::girar){}
		//					while(cor.ler_cor_E() != preto) robot.girar(-30);
		//					while(cor.ler_cor_D() != preto) robot.girar(30);
		//					robot.andar(70);
		//					while(cor.ler_cor_D() == Cor::preto ||
		//							cor.ler_cor_E() == Cor::preto){}
		//					break;
		//				}
		//			}
		//		}

		// alinhar usando o metodo da classe robo
		int cor_E = cor.ler_cor_E();
		int cor_D = cor.ler_cor_D();
		if( (cor_E != Cor::branco && cor_E != Cor::fora && cor_E != Cor::ndCor) ||
				(cor_D != Cor::branco && cor_D != Cor::fora && cor_D != Cor::ndCor)){
			robot.andar(30);
			usleep(1000*100);
			cor_E = cor.ler_cor_E();
			cor_D = cor.ler_cor_D();
			if( (cor_E != Cor::branco && cor_E != Cor::fora && cor_E != Cor::ndCor) ||
					(cor_D != Cor::branco && cor_D != Cor::fora && cor_D != Cor::ndCor)){
				cout<<"viu cor"<<endl;
				posicao_inicial = robot.get_distancia_linha_reta();
				while(robot.get_distancia_linha_reta() < posicao_inicial + 0.04){}
				robot.parar();

				if(cor.ler_cor_E() != cor_E){
					cout<<"saiu E"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(-10);
					while(robot.get_estado() == flag_aceleracao::girar){}
					robot.andar(70);
				}else if(cor.ler_cor_D() != cor_D){
					cout<<"saiu D"<<endl;
					robot.parar();
					robot.andar(-40);
					usleep(1000*2000);
					robot.girar(10);
					while(robot.get_estado() == flag_aceleracao::girar){}
					robot.andar(70);
				}
				else{ // esta dentro
					robot.alinhar(&cor, direcao::traz);
					robot.andar(50, 0.195);
					robot.girar(90);
					while(robot.get_estado() == flag_aceleracao::girar){}
					usleep(1000*800);
					robot.andar(50, 0.19);
					robot.alinhar(&cor, direcao::traz);
					robot.andar(70);
					while(cor.ler_cor_E() == cor_E ||
							cor.ler_cor_D() == cor_D){}
					usleep(1000*500);
				}
			}
		}




		if(cor.ler_cor_E() == Cor::fora){
			cout<<"saiu E"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*1000);
			robot.girar(-10);
			while(robot.get_estado() == flag_aceleracao::girar){}
			robot.andar(70);
		}
		if(cor.ler_cor_D() == Cor::fora){
			cout<<"saiu D"<<endl;
			robot.parar();
			robot.andar(-40);
			usleep(1000*1000);
			robot.girar(10);
			while(robot.get_estado() == flag_aceleracao::girar){}
			robot.andar(70);
		}
	}
}


void teste_luana_mapeamento(){
	Controlador_robo robot(false, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2, false, "");
	Ultrassom_nxt ultraE(ev3dev::INPUT_3);
	Ultrassom_nxt ultraD(ev3dev::INPUT_4);

	robot.calibra_sensor_cor(&cor);
	robot.inicializar_thread_aceleracao();

	while(!ev3dev::button::enter.process());
	usleep(0.3*1000000);
	ev3dev::button::enter.process();

	Mapeamento map(&robot, &cor, &ultraE, &ultraD);

	map.mapear();

}


int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	//teste_luana_alinhamento();
	teste_rogerio();
	//teste_rogerio_alinhamento();
	//teste_luana_mapeamento();

	cout << "Teste finalizado. Bye!" << endl;
	usleep (1000000);
	return 0;
}
