#include "Mapeamento.h"

void Mapeamento::mapeamento(Controlador_robo *robo, Sensor_cor_hsv *sensor){
	robo->andar(70);
	status_atual = status::em_frente;
	sentido_navegacao = 1;

	while(estd != estados::terminado){

		switch (estd){
		/* Caso estiver andando na faixa*/
		case 0:
			robo->andar(70);
			cout << "Estado: " << estd << endl;
			if (sensor->ler_cor_E() == Cor::ndCor || sensor->ler_cor_D() == Cor::ndCor)
				estd = estados::leu_nda;
			else if( (sensor->ler_cor_E() != Cor::fora && sensor->ler_cor_E() != Cor::ndCor && sensor->ler_cor_E() != Cor::branco) ||
					(sensor->ler_cor_D() != Cor::fora && sensor->ler_cor_D() != Cor::ndCor && sensor->ler_cor_D() != Cor::branco) )
				estd = estados::intersec;
			else if (sensor->ler_cor_E() == Cor::fora || sensor->ler_cor_D() == Cor::fora)
				estd = estados::leu_fora;


			break;

			/* Caso ler nada*/
		case 1:
			cout << "Estado: " << estd << endl;
			if(sensor->ler_cor_D() == Cor::ndCor || sensor->ler_cor_E() == Cor::ndCor)
				count_nda ++;

			else{
				count_nda = 0;

				if (sensor->ler_cor_E() == Cor::fora || sensor->ler_cor_D() == Cor::fora)
					estd = estados::leu_fora;
				else if( (sensor->ler_cor_E() != Cor::fora && sensor->ler_cor_E() != Cor::ndCor && sensor->ler_cor_E() != Cor::branco) ||
						(sensor->ler_cor_D() != Cor::fora && sensor->ler_cor_D() != Cor::ndCor && sensor->ler_cor_D() != Cor::branco) )
					estd = estados::intersec;
				else if(sensor->ler_cor_E() == Cor::branco || sensor->ler_cor_D() == Cor::branco)
					estd = estados::faixa;

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
		case 2:
			cout << "Estado: " << estd << endl;
			if(sensor->ler_cor_E() == Cor::fora && sensor->ler_cor_D() == Cor::branco){
				cout<<"saiu E"<<endl;
				robo->parar();
				robo->andar(-40);
				usleep(1000*1000);
				robo->girar(-10);
				while(robo->get_estado() == flag_aceleracao::girar);
				robo->andar(70);
			}


			else if(sensor->ler_cor_D() == Cor::fora && sensor->ler_cor_E() == Cor::branco){
				cout<<"saiu D"<<endl;
				robo->parar();
				robo->andar(-40);
				usleep(1000*1000);
				robo->girar(10);
				while(robo->get_estado() == flag_aceleracao::girar);
				robo->andar(70);
			}

			estd = estados::faixa;

			break;

			/* Caso entrar em uma intersecção*/
		case 3:
			cout << "Estado: " << estd << endl;
			robo->andar(30);
			usleep(1000*100);
			cor_E = sensor->ler_cor_E();
			cor_D = sensor->ler_cor_D();

			if( (sensor->ler_cor_E() != Cor::fora && sensor->ler_cor_E() != Cor::ndCor && sensor->ler_cor_E() != Cor::branco) ||
					(sensor->ler_cor_D() != Cor::fora && sensor->ler_cor_D() != Cor::ndCor && sensor->ler_cor_D() != Cor::branco) )
			{
				cout<<"viu cor"<<endl;

				posicao_inicial = robo->get_distancia();
				while(robo->get_distancia() < posicao_inicial + 0.04);
				robo->parar();

				if(sensor->ler_cor_E() != cor_E){
					cout<<"saiu E"<<endl;
					robo->parar();
					robo->andar(-40);
					usleep(1000*2000);
					robo->girar(-10);
					while(robo->get_estado() == flag_aceleracao::girar);
					robo->andar(70);
				}

				else if(sensor->ler_cor_D() != cor_D){
					cout<<"saiu D"<<endl;
					robo->parar();
					robo->andar(-40);
					usleep(1000*2000);
					robo->girar(10);
					while(robo->get_estado() == flag_aceleracao::girar);
					robo->andar(70);
				}


				else{ // esta dentro
					robo->alinhar_para_traz(sensor);
					robo->andar(50, 0.195);
					cor_E = sensor->ler_cor_E();
					cor_D = sensor->ler_cor_D();
					if(!fim_da_cidade(robo, sensor)) mapeamento_intersec(robo, sensor);
					else {
						sentido_navegacao = -1;
						//finalizar_threads_ultra();
						arq_map->arquivo_map(cp);
						estd = estados::terminado;
						break;
					}
					/*
					robo->girar(90);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(70);
					while(sensor->ler_cor_E() != Cor::branco && sensor->ler_cor_D() != Cor::branco);
					robo->alinhar_para_traz(sensor);
					robo->andar(70);
					while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
					usleep(1000*500);

					estd = estados::faixa;
					 */
				}
			}

			break;

		}
	}
}



void Mapeamento::mapeamento_intersec(Controlador_robo *robo, Sensor_cor_hsv *sensor) {
	/*Primeira intersecção*/
	if(cor_atual == Cor::ndCor){
		if((colorido((*sensor), "esquerdo")) || (colorido((*sensor), "direito"))){
			if(colorido((*sensor), "esquerdo")) cor_atual = sensor->ler_cor_E();
			else cor_atual = sensor->ler_cor_D();
		}

		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar_para_traz(sensor);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

		estd = estados::faixa;
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
			robo->alinhar_para_traz(sensor);
			robo->andar(70);
			while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
			usleep(1000*500);

			dead_end = true;
			estd = estados::faixa;
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

				if (status_atual == status::em_frente) {
					status_atual = status::direita;
					virar_direita(sentido_navegacao);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(50, 0.19);
					robo->alinhar_para_traz(sensor);
					robo->andar(70);
					while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
					usleep(1000*500);
				}
				else if(status_atual == status::direita){
					status_atual = status::esquerda;
					virar_esquerda(sentido_navegacao);
					virar_esquerda(sentido_navegacao);
					while(robo->get_estado() == flag_aceleracao::girar);
					usleep(1000*800);
					robo->andar(50, 0.19);
					robo->alinhar_para_traz(sensor);
					robo->andar(70);
					while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
					usleep(1000*500);
				}

				estd = estados::faixa;
				dead_end = false;

			}

			else if((!dead_end)&&
					((!cor_ja_vista((*sensor), "esquerdo")) || (!cor_ja_vista((*sensor), "direito") )))
			{
				if(cor_atual == Cor::vermelho)
					cp.checkpoint_vermelho = status_atual;
				else if(cor_atual == Cor::verde)
					cp.checkpoint_verde = status_atual;
				else if (cor_atual == Cor::azul)
					cp.checkpoint_azul = status_atual;


				if((colorido((*sensor), "esquerdo")) || (colorido((*sensor), "direito"))){
					if(colorido((*sensor), "esquerdo")) cor_atual = sensor->ler_cor_E();
					else cor_atual = sensor->ler_cor_D();
				}

				status_atual = status::em_frente;
				confirmacao_status = false;
				estd = estados::faixa;

			}

			else if(!dead_end){
				if((colorido((*sensor), "esquerdo")) || (colorido((*sensor), "direito"))){
					if(colorido((*sensor), "esquerdo") && sensor->ler_cor_E() == cor_atual){
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = status_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = status_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = status_atual;

							confirmacao_status = true;

						}
						caminho_certo(robo, sensor);
						estd = estados::faixa;

					}

					else if(colorido((*sensor), "direito") && sensor->ler_cor_D() == cor_atual) {
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = status_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = status_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = status_atual;

							confirmacao_status = true;

						}
						caminho_certo(robo, sensor);
						estd = estados::faixa;

					}

					else if((!cor_ja_vista((*sensor), "esquerdo")) || (!cor_ja_vista((*sensor), "direito") ))
					{
						if (!confirmacao_status){
							if(cor_atual == Cor::vermelho)
								cp.checkpoint_vermelho = status_atual;
							else if(cor_atual == Cor::verde)
								cp.checkpoint_verde = status_atual;
							else if (cor_atual == Cor::azul)
								cp.checkpoint_azul = status_atual;

							confirmacao_status = true;

						}
						caminho_certo(robo, sensor);
						estd = estados::faixa;

					}
				}

			}
		}
	}

	interseccao = false;
	//it_no_anterior++;
}


void Mapeamento::caminho_certo (Controlador_robo *robo, Sensor_cor_hsv *sensor){

	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		if (cp.checkpoint_vermelho == status::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_vermelho == status::esquerda)
			virar_esquerda(sentido_navegacao);

		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar_para_traz(sensor);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		if (cp.checkpoint_verde == status::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_verde == status::esquerda)
			virar_esquerda(sentido_navegacao);

		robo->girar(90);
		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar_para_traz(sensor);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);

	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		if (cp.checkpoint_azul == status::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_azul == status::esquerda)
			virar_esquerda(sentido_navegacao);

		robo->girar(90);
		while(robo->get_estado() == flag_aceleracao::girar);
		usleep(1000*800);
		robo->andar(50, 0.19);
		robo->alinhar_para_traz(sensor);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000*500);
	}
}


bool Mapeamento::fim_da_cidade(Controlador_robo *robo, Sensor_cor_hsv *sensor){
	robo->andar(30, 0.040);
	if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();
		robo->andar(30, 0.040);
		if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D) return true;
	}
	robo->alinhar_para_traz(sensor);
	robo->andar(50, 0.195);
	return false;
}


bool Mapeamento::colorido(Sensor_cor_hsv cor, string lado){
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

/* Vermelho == 3
 * Verde == 4
 * Azul == 5
 */
bool Mapeamento::cor_ja_vista(Sensor_cor_hsv cor, string lado){
	if(lado == "esquerdo"){
		switch(cor.ler_cor_E()){
		case 3:
			if( cp.checkpoint_vermelho != status::ndStatus) return true;
			break;

		case 4:
			if( cp.checkpoint_verde != status::ndStatus) return true;
			break;

		case 5:
			if( cp.checkpoint_azul != status::ndStatus) return true;
			break;

		}
	}

	else if(lado == "direito"){
		switch(cor.ler_cor_D()){
		case 3:
			if( cp.checkpoint_vermelho != status::ndStatus) return true;
			break;

		case 4:
			if( cp.checkpoint_verde != status::ndStatus) return true;
			break;

		case 5:
			if( cp.checkpoint_azul != status::ndStatus) return true;
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

	return thread_rodando;
}


bool Mapeamento::finalizar_threads_ultra(){
	if(thread_rodando)
		thread_rodando = false;

	usleep(1000*100);
	return true;
}


void Mapeamento::loop_mapeamento_bonecoE(Controlador_robo *robo, Ultrassom_nxt *ultraE){
	while(thread_rodando){
		if(estd == estados::intersec) interseccao = true;
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


void Mapeamento::loop_mapeamento_bonecoD(Controlador_robo *robo, Ultrassom_nxt *ultraD){
	//Primeiro nó(intersecção)
	if(estd == estados::intersec) interseccao = true;
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


