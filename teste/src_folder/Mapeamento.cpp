#include "Mapeamento.h"



/****************VARIAVEIS GLOBAIS DECLARADAS DENTRO DO CONST**********/
int sentido_navegacao = 1;
vector<no_intersec> no;
direcao_checkpoint cp = {direcao::ndDirecao, direcao::ndDirecao, direcao::ndDirecao};



Mapeamento::Mapeamento(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD)
{	arq_map = new Arquivos_mapeamento;	}


void Mapeamento::mapear(){
	direcao_atual = direcao::ndDirecao;

	int count_intersec = 0;
	int count_branco_apos_intersec = 0;
	Cor corE_corD_iguais = Cor::ndCor;
	int mudanca_cor_fim_cidade = 0;
	int count_fora = 0;

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


			/*
			while(true){
				if(sensor->ler_cor_E() != cor_E) realinha(direcao::esquerda);
				else if(sensor->ler_cor_D() != cor_D) realinha(direcao::direita);
				else break;

				cout << "nao, loop infinito?" << endl;
				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}
			cout << "SIM" << endl;
			 */


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

			if((cor_E == Cor::preto && cor_D == Cor::branco) || (cor_D == Cor::preto && cor_E == Cor::branco)){
				count_fora++;
				if (count_fora >= 5) {
					estd_map = estados_arena::leu_fora;
					count_fora = 0;
				}
				break;
			}

			break;


		}
	}

	cout << "Fim da arena" << endl;

	while(sensor->ler_cor_E() != Cor::branco && sensor->ler_cor_D() != Cor::branco);
	robo->alinhar(sensor, direcao::traz);
	robo->girar(180);
	while(robo->get_estado() == flag_aceleracao::girar);
	robo->parar();

	no.at(it_no).pos = false;
	//finalizar_threads_ultra();
	cout << "Gerando arquivo" << endl;
	arq_map->arquivo_map(cp, no);


	usleep(1000000*8);
}


void Mapeamento::intersec() {
	// Fazer esse alinhamento somente se não for preto, se for preto já entrar no mapeamento_intersec

	robo->alinhar(sensor, direcao::traz);
	if(cor_E != Cor::preto){
		robo->andar(50, 0.15 + robo->get_pintao());
		if(!dead_end) interseccao = true;
	}

	cor_E = sensor->ler_cor_E();
	cor_D = sensor->ler_cor_D();
	if(!fim_da_cidade()) mapeamento_intersec();
	else {
		interseccao = true;
		robo->parar();
		usleep(1000000*1);
		interseccao = false;
		usleep(1000000*1);

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
		interseccao = true;
		//inicializar_threads_ultra();


		cout << "primeira intersec" << endl;

		cor_atual = sensor->ler_cor_D();
		direcao_atual = direcao::direita;

		robo->girar(-90);
		while(robo->get_estado() == flag_aceleracao::girar);

		robo->andar(70);
		while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
		usleep(1000000*0.3);

		interseccao = false;
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

			robo->andar(30, robo->get_pintao());
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

				/* Se cor atual não foi mapeada previamente, mapeia */
				if(!confirmacao_status){
					if(cor_atual == Cor::vermelho)
						cp.checkpoint_vermelho = direcao_atual;

					else if(cor_atual == Cor::verde)
						cp.checkpoint_verde = direcao_atual;

					else if (cor_atual == Cor::azul)
						cp.checkpoint_azul = direcao_atual;


					qnt_cores_mapeadas ++;
				}

				/* Se sim, seta a ltima direcao quando terminar de mapear a segunda
				 * Funcionamento: cada direção é um número de 1 a 3,
				 * se eu somar as duas primeiras direções já conhecidas e diminuir de 6
				 * eu saberei qual a direção que falta, pra eu saber qual cor que não foi
				 * mapeada é so eu pegar o endereço da cor que não foi mapeada
				 * então se a cor for mapeada eu somo, se não foi mapeada eu pego o endereço dela
				 * no final eu verifico se realmente falta somente uma direcao/cor e atribuo a direção
				 *
				 */
				if(automapear_3_checkpoint && qnt_cores_mapeadas == 2){
					int soma_direcoes = 0; // define qual direção não foi mapeada
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
				novo_no_map_boneco();
			}

			/* Cor já mapeada, pode ser igual a atual ou outra cor*/
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
				novo_no_map_boneco();
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


bool Mapeamento::inicializar_threads_ultra(){
	thread_rodando_bonecos = true;

	no_intersec novo_no;
	no.push_back(novo_no);
	no.at(it_no).pre = false;



	mapeamento_bonecoE = thread(&Mapeamento::loop_mapeamento_bonecoE, this);
	mapeamento_bonecoE.detach();


	mapeamento_bonecoD = thread(&Mapeamento::loop_mapeamento_bonecoD, this);
	mapeamento_bonecoD.detach();

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
	bool estou_vendo_boneco = false;

	int dist_boneco;
	bool vi_boneco = false;
	bool vi_boneco0 = false;
	int cont = 0; // para confirmar a leitura do sensor
	while(thread_rodando_bonecos){

		/* Para excluir leituras falsas */
		while(true){
			dist_boneco = ultraE->le_centimetro();
			if(dist_boneco < distancia_boneco) vi_boneco = true;
			else vi_boneco = false;

			if(vi_boneco == vi_boneco0){
				cont ++;
				if(cont >= 3) break;
			}
			else{
				cont = 0;
				vi_boneco0 = vi_boneco;
			}
		}

		if(vi_boneco && interseccao){

			if(!estou_vendo_boneco){
				double dist_boneco_pos = robo->get_distancia_absoluta()-posicao_saiu_intersec;
				no.at(it_no).posicao_pos_d.push_back(dist_boneco_pos);
				leu_boneco = true;
			}
			estou_vendo_boneco = true;

		}
		else{
			if(estou_vendo_boneco) usleep(1000000*0.5);
			estou_vendo_boneco = false;
		}


		usleep(1000000*0.1);
	}
}



void Mapeamento::loop_mapeamento_bonecoD(){
	bool estou_vendo_boneco = false;

	int dist_boneco;
	bool vi_boneco = false;
	bool vi_boneco0 = false;
	int cont = 0;
	while(thread_rodando_bonecos){

		/* Para excluir leituras falsas */
		while(true){
			dist_boneco = ultraD->le_centimetro();
			if(dist_boneco < 16)
				vi_boneco = true;
			else
				vi_boneco = false;

			if(vi_boneco == vi_boneco0){
				cont ++;
				if(cont >= 3)
					break;
			}
			else{
				cont = 0;
				vi_boneco0 = vi_boneco;
			}
		}

		if(vi_boneco && interseccao){

			if(!estou_vendo_boneco){
				double dist_boneco_pos = robo->get_distancia_absoluta()-posicao_saiu_intersec;
				no.at(it_no).posicao_pos_d.push_back(dist_boneco_pos);
				leu_boneco = true;
			}
			estou_vendo_boneco = true;

		}
		else{
			if(estou_vendo_boneco) usleep(1000000*0.5);
			estou_vendo_boneco = false;
		}


		usleep(1000000*0.1);
	}
}


/* Método chamado sempre quando for necessário incrementar mais um nó, ou seja, mais uma intersecção */
void Mapeamento::novo_no_map_boneco(){
	no_intersec novo_no;
	no.push_back(novo_no);
	it_no ++;
	double dist_boneco_pre = 0;

	// vector pre salva do mais proximo ao mais distante tendo como referencia
	// o no pelo qual os bonecos sao pre posicionados
	for(int i = no.at(it_no-1).posicao_pos_e.size()-1 ; i >=0 ; i--){
		dist_boneco_pre = posicao_chegou_intersec - posicao_saiu_intersec; // dist entre intesrsec
		dist_boneco_pre = dist_boneco_pre - no.at(it_no-1).posicao_pos_e.at(i); // dist do ultimo boneco
		no.at(it_no).posicao_pre_e.push_back(dist_boneco_pre);
	}

	for(int i = no.at(it_no-1).posicao_pos_d.size()-1 ; i >=0 ; i--){
		dist_boneco_pre = posicao_chegou_intersec - posicao_saiu_intersec; // dist entre intesrsec
		dist_boneco_pre = dist_boneco_pre - no.at(it_no-1).posicao_pos_d.at(i); // dist do ultimo boneco
		no.at(it_no).posicao_pre_d.push_back(dist_boneco_pre);
	}

	if(leu_boneco){
		no.at(it_no).pre = true;
		no.at(it_no-1).pos = true;
	}

	else{
		no.at(it_no).pre = false;
		no.at(it_no-1).pos = false;
	}

	leu_boneco = false;
}




