#include "Mapeamento.h"
//********************VARIAVEIS GLOBAIS DECLARADAS DENTRO DO CONST**********//
int sentido_navegacao = 0;
list<no_intersec> no;
direcao_checkpoint cp = {direcao::ndDirecao, direcao::ndDirecao, direcao::ndDirecao};

Mapeamento::Mapeamento(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD)
{
	arq_map = new Arquivos_mapeamento;

}

/* rotina de mapeamento, controla o robo inteiro enquanto mapeia as cores
 * e faz o controle das threads de mapear os bonecos
 */
void Mapeamento::mapear(){
	//TODO ROGERIO: verificar se eh necessario tratar os problemas a mais
	cout << endl << endl << endl << "bora mapear!!" << endl;
	direcao_atual = direcao::ndDirecao;

	int count_ndCor = 0; //FIXME count_ndCor nao usado, vai ser necessario?
	int cout_intersec = 0;
	int count_branco_apos_intersec = 0;

	while(estd != estados_arena::terminado){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();

		switch (estd){
		case estados_arena::leu_fora:
			cout << "fora:";
			cout_intersec = 0;
			if(cor_E == Cor::fora)
				realinha(direcao::esquerda);
			else if (cor_D == Cor::fora)
				realinha(direcao::direita);
			else estd = estados_arena::atencao;

			break;


		case estados_arena::intersec: // TODO testar nova verificacao de entrada de intersec
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
				cout << "nao, loop infinito?" << endl;
				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}
			cout << "SIM" << endl << endl;
			intersec();
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
			break;


		case estados_arena::faixa:
			//cout << "faixa" << endl;
			cout_intersec = 0;
			robo->andar(70);
			if (cor_E != Cor::branco || cor_D != Cor::branco)
				estd = estados_arena::atencao;

			break;


		case estados_arena::atencao:
			/* TODO fazer tratamento do caso que sensor fique muito tempo em uma interface
			 */
			//cout << "atencao" << endl;
			if(cout_intersec == 0)
				robo->andar(30);
			if(cor_E == Cor::branco && cor_D == Cor::branco)
			{
				estd = estados_arena::faixa;
				break;
			}
			if (cor_E == Cor::fora || cor_D == Cor::fora)
			{
				estd = estados_arena::leu_fora;
				break;
			}
			if((cor_E == Cor::vermelho && cor_D == Cor::vermelho) ||
					(cor_E == Cor::verde && cor_D == Cor::verde) ||
					(cor_E == Cor::azul && cor_D == Cor::azul) ||
					(cor_E == Cor::preto && cor_D == Cor::preto))
			{
				robo->andar(20);
				cout_intersec ++;
				if(cout_intersec >= 3)
					estd = estados_arena::intersec;
				usleep(1000000*0.1); // para o robo entrar um pouquinho na intersecao
				break;
			}

			break;
		}
	}
}

/* quando o robo entra em uma intersecao dentro do metodo mapear chama-se esse metodo.
 * considera-se dentro da intersecao quando os 2 sensores detectarem a mudanca de cor e fizer
 * 3 leituras para confirmar essa intersecao, equivale aos sensores de cor entrar dentro da incersecao
 * mais ou menos 2 cm na velocidade de atencao (30) e as rodas ainda continuam pra fora, o robo pode estar
 * desalinhado
 */
void Mapeamento::intersec() {
	//fazer esse alinhamento somente se nao for preto, se for preto ja entrar no mapeamento_intersec
	if(cor_E != Cor::preto){
		robo->alinhar(sensor, direcao::traz);
		robo->andar(50, 0.15 + robo->get_pintao()); // vai pro meio do quadrado
		if(!dead_end)
			interseccao = true;
	}
	cor_E = sensor->ler_cor_E();
	cor_D = sensor->ler_cor_D();
	if(!fim_da_cidade())
		mapeamento_intersec();// verificar se eh o fim da cidade
	else {
		interseccao = true;
		robo->parar();
		usleep(1000000*0.8);
		interseccao = false;
		it_no_anterior++;
		usleep(1000000*0.8);
		arq_map->arquivo_map(cp, no);
		estd = estados_arena::terminado;
	}
}


/* mapeia aquela cor e faz a proxima movimentacao do robo, se for a primeira intersecao inicializa a thread
 * de boneco, se nao, verifica se eh um dead-end ou outra intersecao, se for outra intersecao mapeia a atual
 * se necessario e verifica se ja eh uma intersecao conhecida ou nao, se for conhecida segue pro caminho certo,
 * se nao for conhecida a nova intersecao passa a ser a atual e o robo comeca a mapea-la, se mapear duas intersecoes
 * a terceira ja eh mapeada automaticamente
 */
void Mapeamento::mapeamento_intersec() {
	if(cor_E != cor_D) // verifica se esta como previsto, se nao da um aviso e nao continua
	{
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

		//estd = estados_arena::faixa;
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
			robo->alinhar(sensor, direcao::traz);
			robo->andar(30);
			while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
			usleep(1000000*0.5);
			//estd = estados_arena::faixa;
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
				//estd = estados_arena::faixa;
				dead_end = false;
			}

			/*Cor nao mapeada, pode ser igual a atual ou outra cor*/
			else if(!cor_ja_mapeada())
			{
				if(!confirmacao_status){ // se cor atual nao foi mapeada preveamente mapeia
					if(cor_atual == Cor::vermelho)
						cp.checkpoint_vermelho = direcao_atual;
					else if(cor_atual == Cor::verde)
						cp.checkpoint_verde = direcao_atual;
					else if (cor_atual == Cor::azul)
						cp.checkpoint_azul = direcao_atual;
					qnt_cores_mapeadas ++;
				}

				// se sim, seta a ultima direcao quando terminar de mapear a segunda
				// funcionamento: cada direcao eh um numero de 1 a 3,
				// se eu somar as duas primeiras direcoes ja conhecidas e diminuir de 6
				// eu saberei qual a direcao que falta, pra eu saber qual cor que nao foi
				// mapeada eh so eu pegar o endereco da cor que nao foi mapeada
				// entao se a cor for mapeada eu somo, se nao foi mapeada eu pego o endereco dela
				// no final eu verifico se realmente falta somente uma direcao/cor e atribuo a direcao
				if(automapear_3_checkpoint && qnt_cores_mapeadas == 2){
					int soma_direcoes = 0; // define qual direcao nao foi mapeada
					direcao * ultimo_checkpoint = nullptr;
					if( cp.checkpoint_vermelho != direcao::ndDirecao){
						soma_direcoes += cp.checkpoint_vermelho;
					}
					else  ultimo_checkpoint = &cp.checkpoint_vermelho;

					if( cp.checkpoint_verde != direcao::ndDirecao){
						soma_direcoes += cp.checkpoint_verde;
					}
					else  ultimo_checkpoint = &cp.checkpoint_verde;

					if( cp.checkpoint_azul != direcao::ndDirecao){
						soma_direcoes += cp.checkpoint_azul;
					}
					else  ultimo_checkpoint = &cp.checkpoint_azul;

					*ultimo_checkpoint = (direcao)(6-soma_direcoes);
					cout <<endl << endl << endl << "ultima direcao:"<< *ultimo_checkpoint << endl;
					usleep(1000000*5);
					cout << "ultima cor, seguir" << endl;
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
				//estd = estados_arena::faixa;
				interseccao = false;
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

				if(cor_E != cor_atual){
					cout << "cor conhecida" << endl;
					cor_atual = cor_E;
				}
				else{
					cout << "mesma cor, seguir" << endl;
				}

				caminho_certo();
				confirmacao_status = true;

				robo->andar(70);
				while(sensor->ler_cor_E() == cor_E || sensor->ler_cor_D() == cor_D);
				usleep(1000000*0.3);
				dead_end = false;
				estd = estados_arena::faixa;
				interseccao = false;
				it_no_anterior++;
			}
		}
	}
}

/* O robo gira pra direcao certa da nova cor, ou seja, intersecao que
 * ele esta sobre quando o metodo eh chamado, lembrando que a variavel cor_atual guarda a cor
 * da intersecao que o robo esta vindo e esta mapeando no momento
 */
void Mapeamento::caminho_certo (){
	if (cor_E == Cor::vermelho && cor_D == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita){
			robo->girar(-90);;
		}

		else if (cp.checkpoint_vermelho == direcao::esquerda){
			robo->girar(90);
		}
	}

	else if (cor_E == Cor::verde && cor_D == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita){
			robo->girar(-90);
		}

		else if (cp.checkpoint_verde == direcao::esquerda){
			robo->girar(90);
		}
	}

	else if (cor_E == Cor::azul && cor_D == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita){
			robo->girar(-90);
		}

		else if (cp.checkpoint_azul == direcao::esquerda){
			robo->girar(90);
		}
	}

	while(robo->get_estado() == flag_aceleracao::girar);
}

/* TODO verifica se eh o final da cidade, se for entao retorna true, temos um problema
 * esse metodo eh chamado quando o robo detecta uma intersecao, se ele chegar na rampa e nao detectar
 * uma intersecao por causa da inclinacao da rampa (distancia entre o chao e o sensor de cor varia)
 * isso deve ser tratado dentro da deteccao da intersecao dentro do metodo mapear
 */
bool Mapeamento::fim_da_cidade(){
	if(iterador_fim_cidade < 4){
		iterador_fim_cidade ++;
		return false;
	} else{
		//cout <<endl << endl << "fim city moda foca" << endl;
		//usleep(1000000*5);
		return false;
	}
	//	robo->andar(30, 0.040);
	//	if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D){
	//		cor_E = sensor->ler_cor_E();
	//		cor_D = sensor->ler_cor_D();
	//		robo->andar(30, 0.040);
	//		if (sensor->ler_cor_E() != cor_E && sensor->ler_cor_D() != cor_D) return true;
	//	}
	//	robo->alinhar(sensor, direcao::traz);
	//	robo->andar(50, 0.195);
	//	return false;
}

/*
 * Retorna true se a nova cor ja estiver sido mapeada anteriormente, se for a mesma da cor atual
 * o metodo vai retornar false, pois a atribuicao de direcao eh feito depois do uso desse metodo
 * logo duas cores iguais seguidas mas nao mapeada anteriormente retorna false
 */
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

/*
 * quando eh necessario voltar e girar pra casos como o robo saindo da pista por exemplo
 * isso nao implica que o robo vai ficar reto, ele so "para de sair" da pista
 */
void Mapeamento::realinha(direcao lado_saindo) {
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



bool Mapeamento::inicializar_threads_ultra(){
	thread_rodando_bonecos = true;
	//	mapeamento_bonecoD = thread(&Mapeamento::loop_mapeamento_bonecoD, this);
	//	mapeamento_bonecoD.detach();
	//	usleep(100000);

	mapeamento_bonecoE = thread(&Mapeamento::loop_mapeamento_bonecoE, this);
	mapeamento_bonecoE.detach();
	usleep(1000000*1);

	return thread_rodando_bonecos;
}


bool Mapeamento::finalizar_threads_ultra(){
	if(thread_rodando_bonecos)
		thread_rodando_bonecos = false;

	usleep(1000*100);
	return true;
}


//TODO Arrumar forma como pego a distancia da interseccao ao boneco

void Mapeamento::loop_mapeamento_bonecoE(){
	while(thread_rodando_bonecos){
		if (map_boneco_inicio)
			(*it_no_atual).pre = false;
		//Se chegar numa intersecção sem bonecos detectados no caminho
		else if(!map_boneco_inicio && interseccao && !leu_boneco){
			it_no_atual++;
			(*it_no_atual).pre = false;
			(*it_no_anterior).pos = false;
		}

		//Se chegar numa intersecção com um boneco detectado no caminho até lá
		else if(!map_boneco_inicio && interseccao && leu_boneco){
			it_no_atual++;
			posicao_intersec = robo->get_distancia_absoluta();

			(*it_no_atual).pre = true;

			j = (*it_no_anterior).posicao_pos_e.size() - 1;
			for(unsigned i = 0; i < (*it_no_anterior).posicao_pos_e.size(); i++){
				(*it_no_atual).posicao_pre_e[i] = ( posicao_intersec - (*it_no_anterior).posicao_pos_e[j] );
				j--;

			}
		}

		//Entre intersecções
		else if(!map_boneco_inicio && !interseccao){
			usleep(1000000*0.01);
			if(ultraE->le_centimetro() <= distancia_boneco){
				(*it_no_atual).posicao_pos_e.push_back(robo->get_distancia_absoluta());
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
		}
	}
}









void PROBLEMAS_NO_MAPEAMENTO(){

	/* dentro do mapeamento_intersec quando ve o deadend esta girando 360 e alinhando para traz
	 *
	 * numa intercecao o robo alinha 2 vezes antes de sair dela
	 *
	 * quando o robo volta de um deadend ele gira 360 denovo
	 *
	 * na proxima intersecao ele nao virou pro lado certo
	 *
	 *
	 *
	 * no metodo intersec verifica se os sensores
	 * estao vendo cor para ver se o robo esta mesmo numa intersecao,
	 * logo em baixo dentro do metodo mapeamento_intersec verifica denovo,
	 * tambem esta alinhando 2 vezes e com 2 movimentos para frente antes do alinhamento
	 *
	 * pra arrumar isso eu tirei a verificacao de dentro do metodo intersec, e passei pra dentro
	 * do mapear, quando chamar o metodo intersec eh porque o eh certeza
	 * do robo estar dentro do quadrado,
	 *
	 *  eu tirei o alinhar dentro do mapeamento_intersec tbm, quando chamar o mapeamento_intersec
	 *  eh porque o robo esta no meio do quadrado ja
	 *
	 *
	 * sujestao, quando o robo voltar do deadend ja fazer um giro pra ele parar onde deve parar
	 * e mapear pela direita, pois assim sempre que ele voltar de um deadend ele vira a mesma coisa
	 *
	 * o metodo fim da cidade nao verifica apenas se eh o fim da cidade, ele faz o robo andar e alinhar
	 * isso ta fazendo o robo alinhar e andar 2 vezes dentro da intersecao dentro do metodo intersec
	 * eu so coloquei um true no lugar, mas para arrumar vc pode verificar dentro do metodo se ja passou
	 * pelo menos 6 intersec, so depois de 6 intersec o robo comeca a procurar se eh o final da cidade
	 *
	 *
	 * dentro do metodo intersec no inicio ele garante que o robo esteja no quadrado antes de continuar o codigo
	 */

	/*
	 * ja arrumei o tamanho do pintao
	 *
	 * pra arrumar aquela trela que ele sai loucao depois de alinhar eh so chamar o metodo
	 * reset_motores no case parar dentro do loop controlador do robo no lugar daqueles motores stop
	 */
}




