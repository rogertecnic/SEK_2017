#include "Mapeamento.h"



//********************VARIAVEIS GLOBAIS DECLARADAS DENTRO DO CONST**********//
int sentido_navegacao = 0;
vector<no_intersec> no;
direcao_checkpoint cp = {direcao::ndDirecao, direcao::ndDirecao, direcao::ndDirecao};



Mapeamento::Mapeamento(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD)
{ arq_map = new Arquivos_mapeamento; }


void Mapeamento::mapear(){
	direcao_atual = direcao::ndDirecao;

	int count_intersec = 0;
	int count_branco_apos_intersec = 0;
	Cor corE_corD_iguais = Cor::ndCor;
	int mudanca_cor_fim_cidade = 0;

	while(estd_map != estados_arena::terminado){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();

		switch (estd_map){
		case estados_arena::leu_fora:
			cout << "fora:";

			count_intersec = 0;
			count_branco_apos_intersec = 0;
			corE_corD_iguais = Cor::ndCor;
			mudanca_cor_fim_cidade = 0;

			if(cor_E == Cor::fora) realinha(direcao::esquerda);
			else if (cor_D == Cor::fora) realinha(direcao::direita);
			else estd_map = estados_arena::atencao;

			break;

		case estados_arena::intersec:
			cout << "intersec?";

			while(true){
				if(sensor->ler_cor_E() != cor_E) realinha(direcao::esquerda);
				else if(sensor->ler_cor_D() != cor_D) realinha(direcao::direita);
				else break;

				cout << "nao, loop infinito?" << endl;
				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}
			cout << "SIM" << endl;

			intersec();

			while( !(sensor->ler_cor_E() == Cor::branco) || !(sensor->ler_cor_D() == Cor::branco)) {
				count_branco_apos_intersec ++;
				if(count_branco_apos_intersec >= 5){
					robo->parar();
					usleep(1000000*10);
					estd_map = estados_arena::faixa;
				}
			}
			estd_map = estados_arena::faixa;

			break;

		case estados_arena::faixa:
			count_intersec = 0;
			count_branco_apos_intersec = 0;
			corE_corD_iguais = Cor::ndCor;
			mudanca_cor_fim_cidade = 0;

			robo->andar(70);
			if (cor_E != Cor::branco || cor_D != Cor::branco)
				estd_map = estados_arena::atencao;

			break;

		case estados_arena::atencao:
			if(count_intersec == 0)
				robo->andar(30);

			if(cor_E == Cor::branco && cor_D == Cor::branco) {
				estd_map = estados_arena::faixa;
				break;
			}

			if (cor_E == Cor::fora || cor_D == Cor::fora) {
				estd_map = estados_arena::leu_fora;
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
					count_intersec = 0;
				}
				else count_intersec ++;

				switch(mudanca_cor_fim_cidade){
				case 1:
					if(count_intersec >= 6) // robo desacelerando, passa a cor rapido
						estd_map = estados_arena::intersec;

					break;
				case 2:
					if(count_intersec >= 18) // robo ja devagar, demora mais pra passar a cor
						estd_map = estados_arena::intersec;
					break;
				case 3:
					estd_map = estados_arena::terminado;
					cout << "MAP TERMINADO!!" << endl;
					break;
				}
				usleep(1000000*0.08);
			}

			break;


		}
	}

	cout << "Fim da arena" << endl;
	robo->parar();
	usleep(1000000*8);
}

void Mapeamento::intersec() {
	// Fazer esse alinhamento somente se não for preto, se for preto já entrar no mapeamento_intersec
	if(cor_E != Cor::preto){
		robo->alinhar(sensor, direcao::traz);
		robo->andar(50, 0.15 + robo->get_pintao());
		if(!dead_end){
			if(!map_boneco_inicio) {
				no.push_back(aux);
				destructor_vector();
			}
			interseccao = true;
		}

	}
	cor_E = sensor->ler_cor_E();
	cor_D = sensor->ler_cor_D();
	if(!fim_da_cidade()) mapeamento_intersec();
	else {
		no.push_back(aux);
		destructor_vector();
		interseccao = true;

		robo->parar();
		usleep(1000000*0.8);

		interseccao = false;
		robo->reset_distancia_absoluta();
		it_no_anterior++;
		usleep(1000000*0.8);

		arq_map->arquivo_map(cp, no);
		estd_map = estados_arena::terminado;
	}
}


void Mapeamento::mapeamento_intersec() {
	if(cor_E != cor_D){
		robo->parar();
		cout << "mapeamento_intersec sensores com leituras diferentes, press enter" << endl;
		while(!ev3dev::button::enter.process());
		usleep(1000000*0.1);
		while(!ev3dev::button::enter.process());
	}

	/*Primeira intersecção*/
	if(cor_atual == Cor::ndCor){
		map_boneco_inicio = true;
		inicializar_threads_ultra();
		interseccao = true;

		cout << "primeira intersec" << endl;

		cor_atual = sensor->ler_cor_D();
		direcao_atual = direcao::direita;
		robo->girar(-90);
		while(robo->get_estado() == flag_aceleracao::girar);
		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000000*0.3);

		interseccao = false;
		robo->reset_distancia_absoluta();
		map_boneco_inicio = false;
	}
	/*Depois da primeira intersecção*/
	else{

		/*Enconro de Dead-end*/
		if(cor_E == Cor::preto || cor_D == Cor::preto){
			dead_end = true;

			cout << "cheguei Dead-end" << endl;

			robo->girar(180);
			while(robo->get_estado() == flag_aceleracao::girar);
			robo->alinhar(sensor, direcao::traz);
			robo->andar(30);
			while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
			usleep(1000000*0.5);
		}

		/*Encontro de outra intersecção*/
		else{

			/*
			 * Se não for a primeira intersecção nem dead-end, nem a volta de um dead-end tempos três opções:
			 * 1- Se for a volta de um dead-end tratar o "giro" para a proxima opção de caminho
			 * 2- se for uma cor que ainda não foi mapeada, pode ser a atual ou uma nova, setar a direcao da cor atual
			 *    e mapear cor nova, ou seguir caminho certo se for a mesma cor atual
			 * 3- se for uma cor conhecida (mapeada), setar a direcao da cor atual e seguir para cor certa
			 *
			 * Setar corretamente a confirmacao_status
			 */

			/*voltando de um dead-end*/
			if(dead_end){
				cout << "voltei Dead-end" << endl;
				robo->girar(-90);
				while(robo->get_estado() == flag_aceleracao::girar);

				if (direcao_atual == direcao::direita)
					direcao_atual = direcao::frente;
				else if(direcao_atual == direcao::frente)
					direcao_atual = direcao::esquerda;

				robo->andar(70);
				while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
				usleep(1000000*0.3);
				dead_end = false;
			}

			/*Cor nao mapeada, pode ser igual a atual ou outra cor*/
			else if(!cor_ja_mapeada()) {
				if(!confirmacao_status){ // se cor atual nao foi mapeada preveamente mapeia
					if(cor_atual == Cor::vermelho)
						cp.checkpoint_vermelho = direcao_atual;
					else if(cor_atual == Cor::verde)
						cp.checkpoint_verde = direcao_atual;
					else if (cor_atual == Cor::azul)
						cp.checkpoint_azul = direcao_atual;
					qnt_cores_mapeadas ++;
				}

				/* Se sim, seta a ultima direcao quando terminar de mapear a segunda
				 * funcionamento: cada direcao eh um numero de 1 a 3,
				 * se eu somar as duas primeiras direcoes ja conhecidas e diminuir de 6
				 * eu saberei qual a direcao que falta, pra eu saber qual cor que nao foi
				 * mapeada eh so eu pegar o endereco da cor que nao foi mapeada
				 * entao se a cor for mapeada eu somo, se nao foi mapeada eu pego o endereco dela
				 * no final eu verifico se realmente falta somente uma direcao/cor e atribuo a direcao
				 *
				 */
				if(automapear_3_checkpoint && qnt_cores_mapeadas == 2){
					int soma_direcoes = 0; // define qual direcao nao foi mapeada
					direcao * ultimo_checkpoint = nullptr;
					if( cp.checkpoint_vermelho != direcao::ndDirecao)
						soma_direcoes += cp.checkpoint_vermelho;

					else  ultimo_checkpoint = &cp.checkpoint_vermelho;


					if( cp.checkpoint_verde != direcao::ndDirecao)
						soma_direcoes += cp.checkpoint_verde;

					else  ultimo_checkpoint = &cp.checkpoint_verde;


					if( cp.checkpoint_azul != direcao::ndDirecao)
						soma_direcoes += cp.checkpoint_azul;

					else  ultimo_checkpoint = &cp.checkpoint_azul;


					*ultimo_checkpoint = (direcao)(6-soma_direcoes);

					usleep(1000000*5);
					caminho_certo();

					confirmacao_status = true;
					qnt_cores_mapeadas ++;
				}

				else if(cor_E != cor_atual){
					cout << "cor desconhecida" << endl;

					cor_atual = cor_E;
					direcao_atual = direcao::direita;
					robo->girar(-90);
					while(robo->get_estado() == flag_aceleracao::girar);
					confirmacao_status = false;
				}
				else{
					cout << "mesma cor, seguir" << endl;

					caminho_certo();
					confirmacao_status = true;
				}

				robo->andar(70);
				while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
				usleep(1000000*0.3);
				dead_end = false;

				interseccao = false;
				robo->reset_distancia_absoluta();
				it_no_anterior++;
			}

			/*Cor ja mapeada, pode ser igual a atual ou outra cor*/
			else {
				if (!confirmacao_status){
					if(cor_atual == Cor::vermelho)
						cp.checkpoint_vermelho = direcao_atual;
					else if(cor_atual == Cor::verde)
						cp.checkpoint_verde = direcao_atual;
					else if (cor_atual == Cor::azul)
						cp.checkpoint_azul = direcao_atual;
					qnt_cores_mapeadas ++;
				}

				if(cor_E != cor_atual) cor_atual = cor_E;


				caminho_certo();
				confirmacao_status = true;

				robo->andar(70);
				while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
				usleep(1000000*0.3);
				dead_end = false;
				estd_map = estados_arena::faixa;

				interseccao = false;
				robo->reset_distancia_absoluta();
				it_no_anterior++;
			}
		}
	}
}


void Mapeamento::caminho_certo (){
	if (cor_E == Cor::vermelho && cor_D == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita)
			robo->girar(-90);

		else if (cp.checkpoint_vermelho == direcao::esquerda)
			robo->girar(90);

	}

	else if (cor_E == Cor::verde && cor_D == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita)
			robo->girar(-90);

		else if (cp.checkpoint_verde == direcao::esquerda)
			robo->girar(90);

	}

	else if (cor_E == Cor::azul && cor_D == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita)
			robo->girar(-90);

		else if (cp.checkpoint_azul == direcao::esquerda)
			robo->girar(90);
	}

	while(robo->get_estado() == flag_aceleracao::girar);
}

bool Mapeamento::fim_da_cidade(){
	if(iterador_fim_cidade < 4)
		iterador_fim_cidade ++;

	return false;
}

bool Mapeamento::cor_ja_mapeada(){
	switch(cor_E){
	case Cor::vermelho:
		if( cp.checkpoint_vermelho != direcao::ndDirecao) return true;
		break;

	case Cor::verde:
		if( cp.checkpoint_verde != direcao::ndDirecao) return true;
		break;

	case Cor::azul:
		if( cp.checkpoint_azul != direcao::ndDirecao) return true;
		break;

	}
	return false;
}

void Mapeamento::realinha(direcao lado_saindo) {
	double pwm_sp = robo->get_pwm_sp();
	int grau = 12;

	if(lado_saindo == direcao::esquerda){
		cout<<"saiu E"<<endl;
		robo->parar();
		robo->andar(-80,0.08);
		robo->girar(-grau);
		while(robo->get_estado() == flag_aceleracao::girar);
		robo->andar(pwm_sp);
	}

	else if(lado_saindo == direcao::direita){
		cout<<"saiu D"<<endl;
		robo->parar();
		robo->andar(-80,0.08);
		robo->girar(grau);
		while(robo->get_estado() == flag_aceleracao::girar);
		robo->andar(pwm_sp);
	}

	else{
		cout << "realinha argumento errado" << endl;
		robo->parar();
		usleep(1000000*5);
	}
	usleep(1000000*0.3);
}


void Mapeamento::destructor_vector(){
	aux.posicao_pre_e.clear();
	aux.posicao_pos_e.clear();
	aux.posicao_pre_d.clear();
	aux.posicao_pos_d.clear();
}



bool Mapeamento::inicializar_threads_ultra(){
	thread_rodando_bonecos = true;

	mapeamento_bonecoE = thread(&Mapeamento::loop_mapeamento_bonecoE, this);
	mapeamento_bonecoE.detach();

	/*
	mapeamento_bonecoD = thread(&Mapeamento::loop_mapeamento_bonecoD, this);
	mapeamento_bonecoD.detach();
	usleep(100000);
	*/

	return thread_rodando_bonecos;
}


bool Mapeamento::finalizar_threads_ultra(){
	if(thread_rodando_bonecos)
		thread_rodando_bonecos = false;

	usleep(1000*100);
	return true;
}


//TODO

void Mapeamento::loop_mapeamento_bonecoE(){
	while(thread_rodando_bonecos){
		if (map_boneco_inicio)
			aux.pre = false;
		//Se chegar numa intersecção sem bonecos detectados no caminho
		else if(!map_boneco_inicio && interseccao && !leu_boneco){
			aux.pre = false;
			(*it_no_anterior).pos = false;
		}

		//Se chegar numa intersecção com um boneco detectado no caminho até lá
		else if(!map_boneco_inicio && interseccao && leu_boneco){
			posicao_intersec = robo->get_distancia_absoluta();

			aux.pre = true;

			j = (*it_no_anterior).posicao_pos_e.size() - 1;
			for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_e.size(); i++){
				aux.posicao_pre_e[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_e[j] );
				j--;

			}
		}

		//Entre intersecções
		else if(!map_boneco_inicio && !interseccao){
			usleep(1000000*0.01);
			if(ultraE->le_centimetro() <= distancia_boneco){
				aux.posicao_pos_e.push_back(robo->get_distancia_absoluta());
				if(!leu_boneco){
					leu_boneco = true;
					aux.pos = true;
				}
			}
			while(ultraE->le_centimetro() <= distancia_boneco);
		}
	}
}

/*
void Mapeamento::loop_mapeamento_bonecoD(){
	//Primeiro nó(intersecção)
	while(thread_rodando_bonecos){
		if(map_boneco_inicio)
			(*it_no_atual).pre = false;

		//Se chegar numa intersecção sem bonecos detectados no caminho
		if(!map_boneco_inicio && interseccao && !leu_boneco){
			(*it_no_atual).pre = false;
			(*it_no_anterior).pos = false;
		}

		//Se chegar numa intersecção com um boneco detectado no caminho até lá
		else if(!map_boneco_inicio && interseccao && leu_boneco){

			posicao_intersec = robo->get_distancia_absoluta();

			(*it_no_atual).pre = true;

			j2 = (*it_no_anterior).posicao_pos_d.size() - 1;
			for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_d.size(); i++){
				(*it_no_atual).posicao_pre_d[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_d[j2] );
				j2--;

			}
		}

		//Entre intersecções
		else if(!map_boneco_inicio && !interseccao){
			if(ultraD->le_centimetro() <= distancia_boneco){
				(*it_no_atual).posicao_pos_d.push_back(robo->get_distancia_absoluta());
				if(!leu_boneco){
					leu_boneco = true;
					(*it_no_atual).pos = true;
				}
			}
			while(ultraD->le_centimetro() <= distancia_boneco);
		}
	}
}
*/
