#include "Resgate.h"


Resgate::Resgate(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD, string porta_garra)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD),
 garra(Garra(porta_garra, 45)){}

void Resgate::resgatar(){
	sentido_navegacao = -1;
	cout << endl << endl << endl << "RESGATAR!!" << endl;
	robo->girar(180);
	while(robo->get_estado() == flag_aceleracao::girar);
	robo->alinhar(sensor, direcao::traz);
	robo->andar(pwm_busca);
	usleep(1000000*0.5); // esperar sair da cor que ele alinhou


	int dist_boneco_E = 999; // dist do ultra pro boneco_E
	int dist_boneco_D = 999; // dist do ultra pro boneco_D
	bool vi_boneco = false; // viu boneco na leitura do ultra
	bool vi_boneco0 = false; // viu boneco na leitura anterior do ultra
	int cont = 0; // contador para confirmar a leitura do ultra
	int count_intersec = 0; // contador para confirmar quando entrar na intersecao
	int count_branco_apos_intersec = 0; // contador para confirmar a saida da intersecao
	Cor corE_corD_iguais = Cor::ndCor; // quando os sensores lerem a mesma cor, para comparacao
	int mudanca_cor_fim_cidade = 0; // contador para confirmar o fim da cidade na mudanca da cor

	while(true){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();
		dist_boneco_E = ultraE->le_centimetro();
		dist_boneco_D = ultraD->le_centimetro();
		if((dist_boneco_E <= distancia_boneco || dist_boneco_D <= distancia_boneco) &&
				carga_bonecos <capacidade_bonecos)
		{
			cout << dist_boneco_E<<";" << dist_boneco_D<< "BONECO?" << endl;
			robo->andar(20);
			estd = estados_arena::captura;
		}

		switch (estd){
		case estados_arena::leu_fora:
			cout << "fora:";
			count_intersec = 0;
			count_branco_apos_intersec = 0;
			corE_corD_iguais = Cor::ndCor;
			mudanca_cor_fim_cidade = 0;
			if(cor_E == Cor::fora)
				realinha(direcao::esquerda);
			else if (cor_D == Cor::fora)
				realinha(direcao::direita);
			else {
				estd = estados_arena::atencao;
				cout << "ATENCAO!!" << endl;
			}

			break; // FIM CASE leu_fora:


		case estados_arena::intersec:
			cout << "intersec?";
			while(true){
				if(sensor->ler_cor_E() != cor_E){
					realinha(direcao::esquerda);
				}
				else if(sensor->ler_cor_D() != cor_D){
					realinha(direcao::direita);
				}
				else
					break;
				cout << "nao, loop infinito?" << endl;//FIXME tratar
				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}
			cout << "SIM" << endl;
			intersec();
			if(estd == estados_arena::terminado)
				break;
			while( !(sensor->ler_cor_E() == Cor::branco) || !(sensor->ler_cor_D() == Cor::branco))
			{
				count_branco_apos_intersec ++;
				if(count_branco_apos_intersec >=5){
					robo->parar();
					cout << "o robo terminou a intersecao e nao esta no branco" << endl;
					cout << "esperando 10 seg" << endl;
					usleep(1000000*10);
					estd = estados_arena::faixa;
				}
			}
			estd = estados_arena::faixa;
			cout << "FAIXA!!" << endl;

			break; // FIM CASE intersec:


		case estados_arena::faixa:
			count_intersec = 0;
			count_branco_apos_intersec = 0;
			corE_corD_iguais = Cor::ndCor;
			mudanca_cor_fim_cidade = 0;
			robo->andar(60);
			if (cor_E != Cor::branco || cor_D != Cor::branco){
				estd = estados_arena::atencao;
				cout << "ATENCAO!!" << endl;
			}

			break; // FIM CASE faixa:


		case estados_arena::atencao:
			/* TODO fazer tratamento do caso que sensor fique muito tempo em uma interface
			 */
			//cout << "atencao" << endl;
			if(count_intersec == 0)
				robo->andar(30);

			if(cor_E == Cor::branco && cor_D == Cor::branco)
			{
				estd = estados_arena::faixa;
				cout << "FAIXA!!" << endl;
				break;
			}
			if (cor_E == Cor::fora || cor_D == Cor::fora)
			{
				estd = estados_arena::leu_fora;
				cout << "FORA!!" << endl;
				break;
			}
			if((cor_E == Cor::vermelho && cor_D == Cor::vermelho) ||
					(cor_E == Cor::verde && cor_D == Cor::verde) ||
					(cor_E == Cor::azul && cor_D == Cor::azul) ||
					(cor_E == Cor::preto && cor_D == Cor::preto))
			{
				robo->andar(20);
				if(corE_corD_iguais != cor_E){
					corE_corD_iguais = cor_E;
					mudanca_cor_fim_cidade ++;
					cout << "INTERFACE" << endl;
					count_intersec = 0;
				}else
					count_intersec ++;

				switch(mudanca_cor_fim_cidade){
				case 1:
					if(count_intersec >= 8) // robo desacelerando, passa a cor rapido
						estd = estados_arena::intersec;

					break;
				case 2:
					if(carga_bonecos >=capacidade_bonecos)
						estd = estados_arena::salva;
					else{
						estd = estados_arena::terminado;
						cout << "RESGATE TERMINADO!!" << endl;
						cout << "FIM DA PISTA SEM BONECO" << endl;
						break;
					}
					usleep(1000000*0.08);
				}

				break; // 	FIM DO case estados_arena::atencao:


				case estados_arena::captura: // caso ver algum boneco
					dist_boneco_E = ultraE->le_centimetro();
					dist_boneco_D = ultraD->le_centimetro();
					if(dist_boneco_E <= distancia_boneco){ // viu boneco a esquerda
						cout << "ESQ!" << endl;
						robo->parar();
						robo->andar(-30, 0.1);
						garra.abrir();
						robo->girar(90);
						while(robo->get_estado() == flag_aceleracao::girar);
						robo->andar(30, 0.2);
						garra.fechar();
						robo->andar(-40, 0.2);
						carga_bonecos ++;
						if(carga_bonecos >=capacidade_bonecos){ // cheio, salva
							robo->girar(90);
							while(robo->get_estado() == flag_aceleracao::girar);
							sentido_navegacao = 1;
							estd = estados_arena::salva;
						}
						else{ // ainda nao cheio, continuar a procurar
							robo->girar(-90);
							while(robo->get_estado() == flag_aceleracao::girar);
							estd = estados_arena::faixa;
						}
					}
					else if(dist_boneco_D <= distancia_boneco){ // viu boneco a direita
						cout << "DIR!" << endl;
						robo->parar();
						robo->andar(-30, 0.1);
						garra.abrir();
						robo->girar(-90);
						while(robo->get_estado() == flag_aceleracao::girar);
						robo->andar(30, 0.2);
						garra.fechar();
						robo->andar(-40, 0.2);
						carga_bonecos ++;
						if(carga_bonecos >=capacidade_bonecos){ // cheio, saiva
							robo->girar(-90);
							while(robo->get_estado() == flag_aceleracao::girar);
							sentido_navegacao = 1;
							estd = estados_arena::salva;
						}
						else{ // ainda nao cheio, continuar a procurar
							robo->girar(90);
							while(robo->get_estado() == flag_aceleracao::girar);
							estd = estados_arena::faixa;
						}
					}
					else{
						cout << "NAO!" << endl;
						estd = estados_arena::faixa;
						break;
					}

					break; // FIM CASE captura:


				case estados_arena::salva: // caso esteja com boneco e ja esteja no fim
					robo->parar();
					cout << "HR DE SALVAR!!" << endl;
					usleep(1000000*10);
					break;// FIM CASE salva:


			} //FIM DO switch (estd){
		} // FIM DO while(){
	}
}

void Resgate::intersec() {
	if(sentido_navegacao == -1)
		qnt_cruzamentos --;
	else
		qnt_cruzamentos ++;

	if(qnt_cruzamentos >0){
		robo->alinhar(sensor, direcao::traz);
		robo->andar(50, 0.15 + robo->get_pintao()); // vai pro meio do quadrado
		caminho_certo();
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000000*0.3);
		estd = estados_arena::faixa;
	}else{
		estd = estados_arena::terminado;
	}
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






void Resgate::caminho_certo (){
	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita){
			if(sentido_navegacao == -1)
				robo->girar(90);
			else
				robo->girar(-90);
		}

		else if (cp.checkpoint_vermelho == direcao::esquerda){
			if(sentido_navegacao == -1)
				robo->girar(-90);
			else
				robo->girar(90);
		}
	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita){
			if(sentido_navegacao == -1)
				robo->girar(90);
			else
				robo->girar(-90);
		}

		else if (cp.checkpoint_verde == direcao::esquerda){
			if(sentido_navegacao == -1)
				robo->girar(-90);
			else
				robo->girar(90);
		}
	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita){
			if(sentido_navegacao == -1)
				robo->girar(90);
			else
				robo->girar(-90);
		}

		else if (cp.checkpoint_azul == direcao::esquerda){
			if(sentido_navegacao == -1)
				robo->girar(-90);
			else
				robo->girar(90);
		}
	}

	while(robo->get_estado() == flag_aceleracao::girar);
}
