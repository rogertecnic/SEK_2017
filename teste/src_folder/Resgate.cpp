#include "Resgate.h"


Resgate::Resgate(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD){}

void Resgate::resgatar(){

	while(estd_resgate != estados_arena::terminado){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();

		switch (estd_resgate){
		case estados_arena::leu_fora:
			cout << "fora:";

			if(cor_E == Cor::fora)
				realinha(direcao::esquerda);
			else if (cor_D == Cor::fora)
				realinha(direcao::direita);
			else estd_resgate = estados_arena::atencao;

			break;


		case estados_arena::intersec:
			cout << "intersec?";
			while(true){
				if(sensor->ler_cor_E() != cor_E)
					realinha(direcao::esquerda);

				else if(sensor->ler_cor_D() != cor_D)
					realinha(direcao::direita);

				else break;

				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}

			intersec();
			if(cor_E == Cor::branco && cor_D == Cor::branco) {
				estd_resgate = estados_arena::faixa;
				break;
			}
			else {
				robo->parar();
				cout << "o robo terminou a intersecao e nao esta no branco" << endl;
			}

			break;


		case estados_arena::faixa:
			cout << "faixa" << endl;

			robo->andar(70);
			if (cor_E != Cor::branco || cor_D != Cor::branco)
				estd_resgate = estados_arena::atencao;

			break;


		case estados_arena::atencao:
			cout << "atencao" << endl;

			if(cor_E == Cor::branco && cor_D == Cor::branco) {
				estd_resgate = estados_arena::faixa;
				break;
			}

			if (cor_E == Cor::fora || cor_D == Cor::fora){
				estd_resgate = estados_arena::leu_fora;
				break;
			}

			if((cor_E == Cor::vermelho && cor_D == Cor::vermelho) ||
					(cor_E == Cor::verde && cor_D == Cor::verde) ||
					(cor_E == Cor::azul && cor_D == Cor::azul) ||
					(cor_E == Cor::preto && cor_D == Cor::preto))
			{
				robo->andar(20);
				estd_resgate = estados_arena::intersec;
				usleep(1000000*0.7); // para o robo entrar um pouquinho na intersecao
				break;
			}

			break;
		}

	}
}

void Resgate::intersec() {
	//fazer esse alinhamento somente se nao for preto
	if(cor_E != Cor::preto){
		robo->alinhar(sensor, direcao::traz);
		robo->andar(50, 0.15 + robo->get_pintao()); // vai pro meio do quadrado
	}
	cor_E = sensor->ler_cor_E();
	cor_D = sensor->ler_cor_D();
	/*
	if(true) mapeamento_intersec();// verificar se eh o fim da cidade
	else {
		arq_map->arquivo_map(cp);
		estd = estados_arena::terminado;
	}
	*/
}



void Resgate::realinha(direcao lado_saindo) {
	double pwm_sp = robo->get_pwm_sp();
	int grau = 12;
	if(lado_saindo == direcao::esquerda)
	{
		cout<<"saiu E"<<endl;
		robo->parar();
		robo->andar(-80,0.08);
		robo->girar(-grau);
		while(robo->get_estado() == flag_aceleracao::girar);
		//robo->andar(80, 0.07); // anda pra frente necessario?
		robo->andar(pwm_sp);
	}

	else if(lado_saindo == direcao::direita)
	{
		cout<<"saiu D"<<endl;
		robo->parar();
		robo->andar(-80,0.08);
		robo->girar(grau);
		while(robo->get_estado() == flag_aceleracao::girar);
		//robo->andar(80, 0.08); // anda pra frente necessario?
		robo->andar(pwm_sp);
	}
	else{
		cout << "realinha argumento errado" << endl;
		robo->parar();
		usleep(1000000*5);
	}
	usleep(1000000*0.3);
}




void Resgate::inverter_ponteiros_pos(){


}


void Resgate::caminho_certo (){
	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita){
			robo->girar(-90);;
		}

		else if (cp.checkpoint_vermelho == direcao::esquerda){
			robo->girar(90);
		}
	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita){
			robo->girar(-90);
		}

		else if (cp.checkpoint_verde == direcao::esquerda){
			robo->girar(90);
		}
	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita){
			robo->girar(-90);
		}

		else if (cp.checkpoint_azul == direcao::esquerda){
			robo->girar(90);
		}
	}

	while(robo->get_estado() == flag_aceleracao::girar);
}