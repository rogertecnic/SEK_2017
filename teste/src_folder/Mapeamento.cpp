#include "Mapeamento.h"

void Mapeamento::mapeamento(Controlador_robo *robo, Sensor_cor *sensor){
	robo->andar(30);
	status_atual = status::em_frente;
	sentido_navegacao = 1;

	while(true){

		if(delay > 10){
			//TODO funcao mapeamento: se delay > 10
		}


		if(sensor->ler_cor_D() == Cor::branco && sensor->ler_cor_E() == Cor::branco)
			continue;

		else if(sensor->ler_cor_D() == Cor::fora || sensor->ler_cor_E() == Cor::fora){
			//TODO funcao mapeamento: se ler_cor_D ou E identificar fora
		}

		else if(sensor->ler_cor_D() == Cor::nda || sensor->ler_cor_E() == Cor::nda){
			//TODO funcao mapeamento: se ler_cor_D ou E identificar nda
		}

		else{
			if(sensor->ler_cor_D() == sensor->ler_cor_E()){
				if(sensor->ler_cor_D() != Cor::preto && sensor->ler_cor_E() != Cor::preto)
					interseccao = true;

				if(!mapeamento_intersec(robo, sensor)){
					finalizar_threads_ultra();
					arq_map->arquivo_map(cp, no);

					break;
				}
			}
		}

	}

}

bool Mapeamento::mapeamento_intersec(Controlador_robo *robo, Sensor_cor *sensor) {

	/*Fim da cidade*/
	//TODO detectar fim da cidade, muito facil dar merda, (melhorar logica, testar)
	if(sensor->ler_cor_D() == Cor::vermelho && sensor->ler_cor_E() == Cor::vermelho) {
		usleep(delay_f*100000);
		if(sensor->ler_cor_D() == Cor::azul && sensor->ler_cor_E() == Cor::azul){
			usleep(delay_f*100000);
			if(sensor->ler_cor_D() == Cor::verde && sensor->ler_cor_E() == Cor::verde){
				usleep(delay_f*100000);
				if(sensor->ler_cor_D() == Cor::branco && sensor->ler_cor_E() == Cor::branco){
					sentido_navegacao = -1;
					return false;
				}
			}
		}
	}

	/*Primeira intersecção*/
	if(cor_atual == checkpoint::nda){
		cor_atual = sensor->ler_cor_D(); //TODO ATENCAO: se o sensor cor ler errado aqui, da merda
		map_boneco_inicio = true;
		inicializar_threads_ultra();
	}



	/*Depois da primeira intersecção*/
	else{

		/*Dead-end*/
		if(sensor->ler_cor_D() == Cor::preto && sensor->ler_cor_E() == Cor::preto){
			robo->andar(-30); //TODO voltando do deadend eh necessario alinhar?

			//voltar para a intersecção e mudar status_atual
			while(true){
				if(sensor->ler_cor_D() == cor_atual && sensor->ler_cor_E() == cor_atual){
					usleep(0.2*1000000);
					if (status_atual == status::em_frente) {
						status_atual = status::direita;
						virar_direita(sentido_navegacao);
						break;
					}
					else if(status_atual == status::direita){
						status_atual = status::esquerda;
						virar_esquerda(sentido_navegacao); //TODO lembre-se que aqui ele tem que virar 180 graus
						break;
					}
				}

			}
		}

		/*Encontro de outra intersecção*/
		else{


			/* Se a proxima cor for a mesma da anterior
			 * não será necessario passar por todos os testes de caminho
			 */
			if (sensor->ler_cor_D() == sensor->ler_cor_E() && sensor->ler_cor_D() == cor_atual){
				if (!confirmacao_status){
					if(cor_atual == checkpoint::vermelho)
						cp.checkpoint_vermelho = status_atual;
					else if(cor_atual == checkpoint::verde)
						cp.checkpoint_verde = status_atual;
					else if (cor_atual == checkpoint::azul)
						cp.checkpoint_azul = status_atual;

					confirmacao_status = true;

				}
				caminho_certo(robo, sensor);
			}


			if (sensor->ler_cor_D() == sensor->ler_cor_E()){
				if(cor_atual == checkpoint::vermelho)
					cp.checkpoint_vermelho = status_atual;
				else if(cor_atual == checkpoint::verde)
					cp.checkpoint_verde = status_atual;
				else if (cor_atual == checkpoint::azul)
					cp.checkpoint_azul = status_atual;


				cor_atual = sensor->ler_cor_D();
				status_atual = status::em_frente;
				confirmacao_status = false;
			}
		}
	}

	interseccao = false;
	it_no_anterior++;
	return true;
}


void Mapeamento::caminho_certo (Controlador_robo *robo,Sensor_cor *sensor){

	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		ajeita_quadrado(robo);
		if (cp.checkpoint_vermelho == status::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_vermelho == status::esquerda)
			virar_esquerda(sentido_navegacao);
	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		ajeita_quadrado(robo);
		if (cp.checkpoint_verde == status::direita)
			virar_direita(sentido_navegacao);
		else if (cp.checkpoint_verde == status::esquerda)
			virar_esquerda(sentido_navegacao);
	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		ajeita_quadrado(robo);
		if (cp.checkpoint_azul == status::direita)
			virar_direita(sentido_navegacao);

		else if (cp.checkpoint_azul == status::esquerda)
			virar_esquerda(sentido_navegacao);
	}
}



void Mapeamento::ajeita_quadrado(Controlador_robo *robo){
	posicao_inicial = robo->get_distancia();
	while(robo->get_distancia() < posicao_inicial+0.19);
	robo->parar();
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

		/*Primeiro nó(intersecção)*/
		if(interseccao && map_boneco_inicio){
			(*it_no_atual).pre = false;
			map_boneco_inicio = false;
		}

		/*Se chegar numa intersecção sem bonecos detectados no caminho*/
		else if(!map_boneco_inicio && interseccao && !leu_boneco){
			it_no_atual++;
			(*it_no_atual).pre = false;
			(*it_no_anterior).pos = false;
		}

		/*Se chegar numa intersecção com um boneco detectado no caminho até lá*/
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

		/*Entre intersecções*/
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
	/*Primeiro nó(intersecção)*/
	if(interseccao && map_boneco_inicio){
		(*it_no_atual).pre = false;
		map_boneco_inicio = false;
	}

	/*Se chegar numa intersecção sem bonecos detectados no caminho*/
	else if(!map_boneco_inicio && interseccao && !leu_boneco){
		(*it_no_atual).pre = false;
		(*it_no_anterior).pos = false;
	}

	/*Se chegar numa intersecção com um boneco detectado no caminho até lá*/
	else if(!map_boneco_inicio && interseccao && leu_boneco){

		posicao_intersec = robo->get_distancia();

		(*it_no_atual).pre = true;

		j = (*it_no_anterior).posicao_pos_d.size() - 1;
		for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_d.size(); i++){
			(*it_no_atual).posicao_pre_d[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_d[j] );
			j--;

		}
	}

	/*Entre intersecções*/
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
