#include "Mapeamento.h"

Mapeamento::Mapeamento(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD)
{
	arq_map = new Arquivos_mapeamento;

}


void Mapeamento::mapear(){
	cout << endl << endl << endl << "bora mapear!!" << endl;
	direcao_atual = direcao::frente;
	sentido_navegacao = 1;
	Cor corE, corD;
	int count_ndCor = 0;

	while(estd != estados_Mapeamento::terminado){
		corE = sensor->ler_cor_E();
		corD = sensor->ler_cor_D();

		switch (estd){
		/* Caso estiver andando na faixa*/
		case estados_Mapeamento::faixa:
			count_ndCor = 0;
			robo->andar(70);
			cout << "Estado: " << "rua" << endl;
			if (corE != Cor::branco || corD == Cor::branco)
				estd = estados_Mapeamento::atencao;
			//				estd = estados_Mapeamento::leu_nda;
			//			else if( (corE != Cor::fora && corE != Cor::ndCor && corE != Cor::branco) ||
			//					(corD != Cor::fora && corD != Cor::ndCor && corD != Cor::branco) )
			//				estd = estados_Mapeamento::intersec;
			//			else if (corE == Cor::fora || corD == Cor::fora)
			//				estd = estados_Mapeamento::leu_fora;

			break;

		case estados_Mapeamento::atencao: // algum sensor viu algo que nao era branco
			robo->andar(30);
			if(corE == Cor::branco && corD == Cor::branco)
			{
				estd = estados_Mapeamento::faixa;
				break;
			}
			if (corE == Cor::fora || corD == Cor::fora)
			{
				estd = estados_Mapeamento::leu_fora;
				break;
			}
			/* se os 2 nao foram branco e um ou outro n foi fora entao um deles
			 * viu cor (rgbp) ou ndCor, logo qual deles nao viu branco?
			 */
			if(corE == Cor::ndCor || corD == Cor::ndCor){
				break;
			}
			if (corE == Cor::ndCor || corD == Cor::ndCor)
				estd = estados_Mapeamento::leu_nda;

			estd = estados_Mapeamento::intersec;
			break;


			/* Caso ler nada*/
		case estados_Mapeamento::leu_nda:
			cout << "Estado: " << estd << endl;
			if(corD == Cor::ndCor || corE == Cor::ndCor)
				count_nda ++;

			else{
				count_nda = 0;

				if (corE == Cor::fora || corD == Cor::fora)
					estd = estados_Mapeamento::leu_fora;
				else if( (corE != Cor::fora && corE != Cor::ndCor && corE != Cor::branco) ||
						(corD != Cor::fora && corD != Cor::ndCor && corD != Cor::branco) )
					estd = estados_Mapeamento::intersec;
				else if(corE == Cor::branco || corD == Cor::branco)
					estd = estados_Mapeamento::faixa;

			}

			if(count_nda >= 10){
				cout<<"viu nda 10"<<endl;
				robo->parar();
				robo->andar(-100);
				usleep(1000*800);
				robo->girar(30);
				robo->andar(70);
			}

			break;


			/* Caso ler fora*/
		case estados_Mapeamento::leu_fora:
			count_ndCor = 0;
			cout << "Estado: " << "fora" << endl;
			if(corE == Cor::fora && corD == Cor::branco){
				cout<<"saiu E"<<endl;
				robo->parar();
				robo->andar(-40);
				usleep(1000*1000);
				robo->girar(-10);
				while(robo->get_estado() == flag_aceleracao::girar);
				robo->andar(70);
			}


			else if(corD == Cor::fora && corE == Cor::branco){
				cout<<"saiu D"<<endl;
				robo->parar();
				robo->andar(-40);
				usleep(1000*1000);
				robo->girar(10);
				while(robo->get_estado() == flag_aceleracao::girar);
				robo->andar(70);
			}

			estd = estados_Mapeamento::faixa;

			break;

			/* Caso entrar em uma intersecção*/
		case estados_Mapeamento::intersec:
			cout << "Estado: " << estd << endl;
			robo->andar(30);
			usleep(1000*100);
			cor_E = corE;
			cor_D = corD;

			if( (corE != Cor::fora && corE != Cor::ndCor && corE != Cor::branco) ||
					(corD != Cor::fora && corD != Cor::ndCor && corD != Cor::branco) )
			{
				cout<<"viu cor"<<endl;

				posicao_inicial = robo->get_distancia();
				while(robo->get_distancia() < posicao_inicial + 0.04);
				robo->parar();

				if(corE != cor_E){
					cout<<"saiu E"<<endl;
					robo->parar();
					robo->andar(-40);
					usleep(1000*2000);
					robo->girar(-10);
					while(robo->get_estado() == flag_aceleracao::girar);
					robo->andar(70);
				}

				else if(corD != cor_D){
					cout<<"saiu D"<<endl;
					robo->parar();
					robo->andar(-40);
					usleep(1000*2000);
					robo->girar(10);
					while(robo->get_estado() == flag_aceleracao::girar);
					robo->andar(70);
				}


				else{ // esta dentro
					robo->alinhar(sensor, direcao::traz);
					robo->andar(50, 0.195);
					cor_E = corE;
					cor_D = corD;
					if(!fim_da_cidade()) mapeamento_intersec();
					else {
						sentido_navegacao = -1;
						//finalizar_threads_ultra();
						arq_map->arquivo_map(cp);
						estd = estados_Mapeamento::terminado;
						break;
					}
					/*
					robo->girar(90);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(70);
					while(corE != Cor::branco && corD != Cor::branco);
					robo->alinhar(sensor, direcao::traz);
					robo->andar(70);
					while(corE == cor_E || corD == cor_D);
					usleep(1000*500);

					estd = estados::faixa;
					 */
				}
			}

			break;
		}
	}
}



void Mapeamento::mapeamento_intersec() {
	/*Primeira intersecção*/
	if(cor_atual == Cor::ndCor){
		if((colorido("esquerdo")) || (colorido("direito"))){
			if(colorido("esquerdo")) cor_atual = sensor->ler_cor_E();
			else cor_atual = sensor->ler_cor_D();
		}

		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar(sensor, direcao::traz);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

		estd = estados_Mapeamento::faixa;
		//map_boneco_inicio = true;
		//inicializar_threads_ultra();
	}



	/*Depois da primeira intersecção*/
	else{

		/*Dead-end*/
		if(sensor->ler_cor_D() == Cor::preto || sensor->ler_cor_E() == Cor::preto){
			robo->girar(360);
			while(robo->get_estado() == flag_aceleracao::girar);
			usleep(1000*800);
			robo->andar(50, 0.19);
			robo->alinhar(sensor, direcao::traz);
			robo->andar(70);
			while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
			usleep(1000*500);

			dead_end = true;
			estd = estados_Mapeamento::faixa;
		}

		/*Encontro de outra intersecção*/
		else{

			/*
			 * Se não for a primeira intersecção nem dead-end, nem a volta de um dead-end tempos três opções:
			 * 1- De ser uma cor que não foi "enxergada" antes (OK)
			 * 2- De ser a mesma cor que a anterior (OK)
			 * 3- De já ser uma cor conhecida (OK)
			 *
			 * Se for a volta de um dead-end tratar o "giro" para a proxima opção de caminho (OK)
			 *
			 */
			if(dead_end){
				robo->girar(360);
				while(robo->get_estado() == flag_aceleracao::girar);
				usleep(1000*800);

				if (direcao_atual == direcao::frente) {
					direcao_atual = direcao::direita;
					virar_direita(sentido_navegacao);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(50, 0.19);
					robo->alinhar(sensor, direcao::traz);
					robo->andar(70);
					while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
					usleep(1000*500);
				}
				else if(direcao_atual == direcao::direita){
					direcao_atual = direcao::esquerda;
					virar_esquerda(sentido_navegacao);
					virar_esquerda(sentido_navegacao);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(50, 0.19);
					robo->alinhar(sensor, direcao::traz);
					robo->andar(70);
					while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
					usleep(1000*500);
				}

				estd = estados_Mapeamento::faixa;
				dead_end = false;

			}

			else if((!dead_end)&&
					((!cor_ja_vista("esquerdo")) || (!cor_ja_vista("direito") )))
			{
				if(cor_atual == Cor::vermelho)
					cp.checkpoint_vermelho = direcao_atual;
				else if(cor_atual == Cor::verde)
					cp.checkpoint_verde = direcao_atual;
				else if (cor_atual == Cor::azul)
					cp.checkpoint_azul = direcao_atual;


				if((colorido("esquerdo")) || (colorido("direito"))){
					if(colorido("esquerdo")) cor_atual = sensor->ler_cor_E();
					else cor_atual = sensor->ler_cor_D();
				}

				direcao_atual = direcao::frente;
				confirmacao_status = false;
				estd = estados_Mapeamento::faixa;

			}

			else if(!dead_end){
				if((colorido("esquerdo")) || (colorido("direito"))){
					if(colorido("esquerdo") && sensor->ler_cor_E() == cor_atual){
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = direcao_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = direcao_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = direcao_atual;

							confirmacao_status = true;

						}
						caminho_certo();
						estd = estados_Mapeamento::faixa;

					}

					else if(colorido("direito") && sensor->ler_cor_D() == cor_atual) {
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = direcao_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = direcao_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = direcao_atual;

							confirmacao_status = true;

						}
						caminho_certo();
						estd = estados_Mapeamento::faixa;

					}

					else if((!cor_ja_vista("esquerdo")) || (!cor_ja_vista("direito") ))
					{
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = direcao_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = direcao_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = direcao_atual;

							confirmacao_status = true;

						}
						caminho_certo();
						estd = estados_Mapeamento::faixa;

					}
				}

			}
		}
	}

	interseccao = false;
	//it_no_anterior++;
}


void Mapeamento::caminho_certo (){

	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_vermelho == direcao::esquerda)
			virar_esquerda(sentido_navegacao);

		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar(sensor, direcao::traz);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_verde == direcao::esquerda)
			virar_esquerda(sentido_navegacao);

		robo->girar(90);
		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar(sensor, direcao::traz);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_azul == direcao::esquerda)
			virar_esquerda(sentido_navegacao);

		robo->girar(90);
		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar(sensor, direcao::traz);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);
	}
}


bool Mapeamento::fim_da_cidade(){
	robo->andar(30, 0.040);
	if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();
		robo->andar(30, 0.040);
		if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D) return true;
	}
	robo->alinhar(sensor, direcao::traz);
	robo->andar(50, 0.195);
	return false;
}


bool Mapeamento::colorido(string lado){
	if(lado == "esquerdo"){
		if((*sensor).ler_cor_E() != Cor::branco && (*sensor).ler_cor_E() != Cor::fora && (*sensor).ler_cor_E() != Cor::ndCor)
			return true;
	}
	else {
		if((*sensor).ler_cor_D() != Cor::branco && (*sensor).ler_cor_D() != Cor::fora && (*sensor).ler_cor_D() != Cor::ndCor)
			return true;
	}
	return false;
}

/* Vermelho == 3
 * Verde == 4
 * Azul == 5
 */
bool Mapeamento::cor_ja_vista(string lado){
	if(lado == "esquerdo"){
		switch((*sensor).ler_cor_E()){
		case 3:
			if( cp.checkpoint_vermelho != direcao::ndStatus) return true;
			break;

		case 4:
			if( cp.checkpoint_verde != direcao::ndStatus) return true;
			break;

		case 5:
			if( cp.checkpoint_azul != direcao::ndStatus) return true;
			break;

		}
	}

	else if(lado == "direito"){
		switch((*sensor).ler_cor_D()){
		case 3:
			if( cp.checkpoint_vermelho != direcao::ndStatus) return true;
			break;

		case 4:
			if( cp.checkpoint_verde != direcao::ndStatus) return true;
			break;

		case 5:
			if( cp.checkpoint_azul != direcao::ndStatus) return true;
			break;

		}

	}
	return false;
}


bool Mapeamento::inicializar_threads_ultra(){

	mapeamento_bonecoD = thread(&Mapeamento::loop_mapeamento_bonecoD, this);
	mapeamento_bonecoD.detach();
	usleep(100000);

	mapeamento_bonecoE = thread(&Mapeamento::loop_mapeamento_bonecoE, this);
	mapeamento_bonecoE.detach();
	usleep(100000);

	return thread_rodando_bonecos;
}


bool Mapeamento::finalizar_threads_ultra(){
	if(thread_rodando_bonecos)
		thread_rodando_bonecos = false;

	usleep(1000*100);
	return true;
}


void Mapeamento::loop_mapeamento_bonecoE(){
	while(thread_rodando_bonecos){
		if(estd == estados_Mapeamento::intersec) interseccao = true;
		else interseccao = false;

		//Primeiro nó(intersecção)
		if(interseccao && map_boneco_inicio){
			(*it_no_atual).pre = false;
			map_boneco_inicio = false;
		}

		//Se chegar numa intersecção sem bonecos detectados no caminho
		else if(!map_boneco_inicio && interseccao && !leu_boneco){
			it_no_atual++;
			(*it_no_atual).pre = false;
			(*it_no_anterior).pos = false;
		}

		//Se chegar numa intersecção com um boneco detectado no caminho até lá
		else if(!map_boneco_inicio && interseccao && leu_boneco){
			it_no_atual++;
			posicao_intersec = robo->get_distancia();

			(*it_no_atual).pre = true;

			j = (*it_no_anterior).posicao_pos_e.size() - 1;
			for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_e.size(); i++){
				(*it_no_atual).posicao_pre_e[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_e[j] );
				j--;

			}
		}

		//Entre intersecções
		else if(!map_boneco_inicio&& !interseccao){

			if(ultraE->le_centimetro() <= distancia_boneco){
				(*it_no_atual).posicao_pos_e.push_back(robo->get_distancia());
				if(!leu_boneco){
					leu_boneco = true;
					(*it_no_atual).pos = true;
				}
			}
		}
	}
}

void Mapeamento::loop_mapeamento_bonecoD(){
	//Primeiro nó(intersecção)
	if(estd == estados_Mapeamento::intersec) interseccao = true;
	else interseccao = false;

	if(interseccao && map_boneco_inicio){
		(*it_no_atual).pre = false;
		map_boneco_inicio = false;
	}
	//Se chegar numa intersecção sem bonecos detectados no caminho
	else if(!map_boneco_inicio && interseccao && !leu_boneco){
		(*it_no_atual).pre = false;
		(*it_no_anterior).pos = false;
	}

	//Se chegar numa intersecção com um boneco detectado no caminho até lá
	else if(!map_boneco_inicio && interseccao && leu_boneco){

		posicao_intersec = robo->get_distancia();

		(*it_no_atual).pre = true;

		j = (*it_no_anterior).posicao_pos_d.size() - 1;
		for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_d.size(); i++){
			(*it_no_atual).posicao_pre_d[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_d[j] );
			j--;

		}
	}

	//Entre intersecções
	else if(!map_boneco_inicio&& !interseccao){
		if(ultraD->le_centimetro() <= distancia_boneco){
			(*it_no_atual).posicao_pos_d.push_back(robo->get_distancia());
			if(!leu_boneco){
				leu_boneco = true;
				(*it_no_atual).pos = true;
			}
		}
	}
}


