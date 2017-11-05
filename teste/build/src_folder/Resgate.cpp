#include "Resgate.h"


Resgate::Resgate(Controlador_robo *robo, Sensor_cor_hsv *sensor, Ultrassom_nxt *ultraE, Ultrassom_nxt *ultraD)
:robo(robo), sensor(sensor), ultraE(ultraE), ultraD(ultraD)
{
	cancela = new Garra(ev3dev::OUTPUT_C, -42, "cancela");
	garra = new Garra(ev3dev::OUTPUT_D, 90, "garra");
}


void Resgate::resgatar(){
	sentido_navegacao = -1;
	inverter_ponteiros();

	while(estd_resgate != estados_arena::terminado){
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();

		switch (estd_resgate){
		case estados_arena::leu_fora:
			cout << "fora:";

			if(cor_E == Cor::fora) realinha(direcao::esquerda);
			else if (cor_D == Cor::fora) realinha(direcao::direita);
			else estd_resgate = estados_arena::atencao;

			break;


		case estados_arena::intersec:
			cout << "intersec?";
			while(true){
				if(sensor->ler_cor_E() != cor_E) realinha(direcao::esquerda);
				else if(sensor->ler_cor_D() != cor_D) realinha(direcao::direita);
				else break;

				robo->alinhar(sensor, direcao::frente);
				robo->andar(30, 0.02);
			}



			if(sentido_navegacao == -1) intersec();
			else {
				it++;
				if(!fim_da_cidade()) {
					if(cor_E != Cor::preto){
						robo->alinhar(sensor, direcao::traz);
						robo->andar(50, 0.15 + robo->get_pintao()); // vai pro meio do quadrado
					}
					cor_E = sensor->ler_cor_E();
					cor_D = sensor->ler_cor_D();
					caminho_certo();
				}
				else goto_plaza();

			}


			if(cor_E == Cor::branco && cor_D == Cor::branco) {
				robo->reset_distancia_absoluta();
				estd_resgate = estados_arena::faixa;
				break;
			}

			break;


		case estados_arena::faixa:
			cout << "faixa" << endl;

			robo->andar(70);

			if(gdistancia_boneco != -1){
				if (gdistancia_boneco == robo->get_distancia_absoluta())
					just_do_it();
			}

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
				break;
			}

			break;
		}

	}
}


void Resgate::intersec() {
	if(!inicio_resg) it--;
	else inicio_resg = false;

	if(!inicio_da_cidade()){
		//fazer esse alinhamento somente se nao for preto
		if(cor_E != Cor::preto){
			robo->alinhar(sensor, direcao::traz);
			robo->andar(50, 0.15 + robo->get_pintao()); // vai pro meio do quadrado
		}
		cor_E = sensor->ler_cor_E();
		cor_D = sensor->ler_cor_D();

		caminho_certo();
		robo->andar(70);
		get_distancia_boneco();
	}
	else {
		sentido_navegacao = 1;
		robo->girar(180);
		while(robo->get_estado() == flag_aceleracao::girar);
		fim_resg = true;
	}
}


void Resgate::get_distancia_boneco(){
	if((*it).pos){
		if((*it).posicao_pos_e != NULL && (*it).posicao_pos_d != NULL){
			if((*it).posicao_pos_e[pos_e] < ((*it).posicao_pos_d[pos_d])){
				gdistancia_boneco = (*it).posicao_pos_e[pos_e] - 2;
				lado = lado_ultra::esq;
			}
			else{
				gdistancia_boneco = (*it).posicao_pos_d[pos_e] - 2;
				lado = lado_ultra::dir;
			}
		}

		else{
			if((*it).posicao_pos_e != NULL){
				gdistancia_boneco = (*it).posicao_pos_e[pos_e] - 2;
				lado = lado_ultra::esq;
			}
			else{
				gdistancia_boneco = (*it).posicao_pos_d[pos_e] - 2;
				lado = lado_ultra::dir;
			}
		}
	}
	else gdistancia_boneco = -1;
}


void Resgate::just_do_it(){
	robo->andar(30);

	if(lado == lado_ultra::esq){
		while(ultraE->le_centimetro() > distancia_boneco);
		usleep(1000000*0.15);
		robo->parar();
		robo->andar(-30);
		while(ultraE->le_centimetro() > distancia_boneco);
		robo->parar();
		robo->girar(90);
		while(robo->get_estado() == flag_aceleracao::girar);
		cancela->abrir();
		//TODO andar uma distancia x
		cancela->fechar();
		//TODO voltar uma distancia x
		contador_bonecos++;

		if(contador_bonecos == BONECOS){
			robo->girar(90);
			while(robo->get_estado() == flag_aceleracao::girar);
			sentido_navegacao = 1;
		}
		else{
			robo->girar(-90);
			while(robo->get_estado() == flag_aceleracao::girar);
			get_distancia_boneco();
		}

		robo->andar(70);


		(*it).posicao_pos_e.erase((*it).posicao_pos_e.end());
		pos_e--;

	}
	else{
		while(ultraE->le_centimetro() > distancia_boneco);
		usleep(1000000*0.15);
		robo->parar();
		robo->andar(-30);
		while(ultraE->le_centimetro() > distancia_boneco);
		robo->parar();
		robo->girar(-90);
		while(robo->get_estado() == flag_aceleracao::girar);
		cancela->abrir();
		//TODO andar uma distancia x
		cancela->fechar();
		//TODO voltar uma distancia x
		contador_bonecos++;

		if(contador_bonecos == BONECOS){
			robo->girar(-90);
			while(robo->get_estado() == flag_aceleracao::girar);
			sentido_navegacao = 1;
		}
		else {
			robo->girar(90);
			while(robo->get_estado() == flag_aceleracao::girar);
			get_distancia_boneco();
		}

		robo->andar(70);


		(*it).posicao_pos_e.erase((*it).posicao_pos_e.end());
		pos_e--;


	}
}



//TODO
void Resgate::goto_plaza(){
	int corE, corD;
	double teta = 45;
	double deslocacao;

	while(true){
		corE = sensor->ler_cor_E();
		corD = sensor->ler_cor_D();

		if((corE == Cor::vermelho && corD == Cor::vermelho) ||
				(corE == Cor::verde && corD == Cor::verde) ||
				(corE == Cor::azul && corD == Cor::azul))
		{
			robo->andar(30);
			while(sensor->ler_cor_E() != Cor::branco || sensor->ler_cor_D() != Cor::branco);
			robo->alinhar(sensor, direcao::traz);
			robo->andar(50, 0.1);

			break;
		}
	}

	robo->parar();


	robo->girar(teta);
	while(robo->get_estado() == flag_aceleracao::girar);

	while(sensor->ler_cor_E() != Cor::branco){
		robo->andar(-30, 0.02);
		robo->girar(-teta);
		while(robo->get_estado() == flag_aceleracao::girar);

		robo->girar(teta);
		while(robo->get_estado() == flag_aceleracao::girar);
	}

	robo->andar(30);
	while(sensor->ler_cor_E() == Cor::branco);
	robo->parar();
	deslocacao = (7.5/cos(teta)) - robo->get_pintao();
	robo->andar(-30, deslocacao/100);
	//TODO método que gira com uma roda travada

}


void Resgate::realinha(direcao lado_saindo) {
	double pwm_sp = robo->get_pwm_sp();
	int grau = 12;
	if(lado_saindo == direcao::esquerda) {
		cout<<"saiu E"<<endl;
		robo->parar();
		robo->andar(-80,0.08);
		robo->girar(-grau);
		while(robo->get_estado() == flag_aceleracao::girar);
		robo->andar(pwm_sp);
	}

	else if(lado_saindo == direcao::direita) {
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

void Resgate::caminho_certo (){
	if (sensor->ler_cor_E() == Cor::vermelho && sensor->ler_cor_D() == Cor::vermelho){
		if (cp.checkpoint_vermelho == direcao::direita){
			if (sentido_navegacao == 1)robo->girar(-90);
			else robo->girar(90);
		}

		else if (cp.checkpoint_vermelho == direcao::esquerda){
			if (sentido_navegacao == 1) robo->girar(90);
			else robo->girar(-90);
		}

	}

	else if (sensor->ler_cor_E() == Cor::verde && sensor->ler_cor_D() == Cor::verde){
		if (cp.checkpoint_verde == direcao::direita){
			if (sentido_navegacao == 1) robo->girar(-90);
			else robo->girar(90);
		}


		else if (cp.checkpoint_verde == direcao::esquerda){
			if (sentido_navegacao == 1) robo->girar(90);
			else robo->girar(-90);
		}

	}

	else if (sensor->ler_cor_E() == Cor::azul && sensor->ler_cor_D() == Cor::azul){
		if (cp.checkpoint_azul == direcao::direita){
			if (sentido_navegacao == 1) robo->girar(-90);
			else robo->girar(90);
		}


		else if (cp.checkpoint_azul == direcao::esquerda){
			if (sentido_navegacao == 1) robo->girar(90);
			else robo->girar(-90);
		}

	}

	while(robo->get_estado() == flag_aceleracao::girar);
}

void Resgate::inverter_ponteiros(){
	bool aux;
	it = no.begin();

	while(it <= no.end()){
		aux = (*it).pos;
		(*it).pos = (*it).pre;
		(*it).pre = aux;

		(*it).posicao_pre_e.swap((*it).posicao_pos_e);
		(*it).posicao_pre_d.swap((*it).posicao_pos_d);
		it++;
	}


	if(sentido_navegacao == 1){
		it = no.begin();
		pos_e = 0;
		pre_e = 0;
		pos_d = 0;
		pre_d = 0;
	}
	else{
		it = no.end();
		pos_e = (*it).posicao_pos_e.end();
		pre_e = (*it).posicao_pre_e.end();
		pos_d = (*it).posicao_pos_d.end();
		pre_d = (*it).posicao_pre_d.end();
	}


}

bool Resgate::fim_da_cidade(){
	if((*it) == no.end())  return true;
	return false;
}

bool Resgate::inicio_da_cidade(){
	if((*it) == no.begin())  return true;
	return false;
}



