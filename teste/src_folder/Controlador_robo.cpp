#include "Controlador_robo.h"


Controlador_robo::Controlador_robo(bool debug, string nome_arquivo)
:fator_croda(fator_croda),
 nome_arquivo(nome_arquivo),
 debug(debug)
{
	rodaE = new ev3dev::large_motor(ev3dev::OUTPUT_A);
	rodaD = new ev3dev::large_motor(ev3dev::OUTPUT_B);

	rodaE->reset();
	rodaD->reset();
	rodaE->set_stop_action("hold");
	rodaD->set_stop_action("hold");


	if(debug){
		arquivo = new M_arquivos(nome_arquivo, 5); // tempo, ang_rodaE, ang_rodaD, erro, pwm Direito
		arquivo_aberto = true;
	}
}


void Controlador_robo::andar (int pwm_sp){
	estado = flag_aceleracao::linha_reta;
	usleep(1000*10);
	this->pwm_sp = -pwm_sp;
}


void Controlador_robo::parar () {
	estado = flag_aceleracao::parar;

}


void Controlador_robo::girar(int angulo_robo_graus){
	this->angulo_robo_graus = angulo_robo_graus;
	estado = flag_aceleracao::girar;
	usleep(1000*100);
}


bool Controlador_robo::inicializar_thread_aceleracao(){
	thread_controle_velocidade = thread(&Controlador_robo::loop_controle_aceleracao, this);
	thread_controle_velocidade.detach();
	usleep(1000*100);
	return thread_rodando;
}


bool Controlador_robo::finalizar_thread_aceleracao(){
	if(thread_rodando){
		thread_rodando = false;
		while(arquivo_aberto){}
	}
	usleep(1000*100);
	return true;
}

flag_aceleracao Controlador_robo::get_estado(){
	return estado;
}

/*
 *  retorna a media entre os 2 tacometros das rodas
 * ATENCAO: toda vez que o robo girar os tacometros sao resetados
 */
int Controlador_robo::get_tacometro(){
	return(rodaE->position() + rodaD->position())/2;
}


double Controlador_robo::get_distancia(){
	//cout<<distancia_linha_reta<<endl;
	return distancia_linha_reta;
}


void Controlador_robo::loop_controle_aceleracao(){
	thread_rodando = true;
	t_inicial = Time::now();
	while (thread_rodando) {
		t_final = Time::now();
		delta_t = t_final - t_inicial;
		t_inicial = t_final;
		tempo += delta_t.count();
		arquivo->elementos_arq(tempo, (double)rodaE->position(), (double)rodaD->position(), erro, pwm);


		switch(estado){
		case flag_aceleracao::nd :
			usleep(1000*50);
			break;

		case flag_aceleracao::linha_reta :
			distancia_linha_reta =
					-(rodaE->position()+rodaD->position())/2 *3.141592/180*relacao_engrenagem*raio_roda;
			erro = rodaE->position()*fator_croda - rodaD->position();
			pid = kp*erro + kd*(erro-erro_anterior);
			erro_anterior = erro;

			pwm = pwm_retardada - pid;
			if(pwm > 100) pwm = 100;
			if(pwm < -100) pwm = -100;
			rodaE->set_duty_cycle_sp((int)pwm);

			pwm = pwm_retardada + pid;
			if(pwm > 100) pwm = 100;
			if(pwm < -100) pwm = -100;
			rodaD->set_duty_cycle_sp((int)pwm);
			usleep(1000*5);

			rodaE->run_direct();
			rodaD->run_direct();

			if(pwm_retardada < pwm_sp-aceleracao*delay/1000)
				pwm_retardada += aceleracao*delay/1000;
			if(pwm_retardada > pwm_sp+aceleracao*delay/1000)
				pwm_retardada -= aceleracao*delay/1000;
			break;

		case flag_aceleracao::girar :
			rodaE->stop();
			rodaD->stop();
			pwm_sp = 0;
			pwm_retardada = 0.0;
			pwm = 0;
			usleep(1000*300);
			rodaE->set_position_sp((angulo_robo_graus*raio_robo/raio_roda)/relacao_engrenagem);
			rodaD->set_position_sp(-(angulo_robo_graus*raio_robo/raio_roda)/relacao_engrenagem);
			rodaE->set_speed_sp(400);
			rodaD->set_speed_sp(400);
			rodaE->run_to_rel_pos();
			rodaD->run_to_rel_pos();
			usleep(1000*100);
			while((rodaE->speed() > 2 || rodaE->speed() < -2) &&
					estado == flag_aceleracao::girar){ }

			rodaE->run_forever(); // so funciona se chamar o run_forever
			rodaD->run_forever(); // antes de parar, caso contrario o robo fica louco
			rodaE->stop();
			rodaD->stop();
			usleep(1000*300);
			if(estado == flag_aceleracao::girar) // se o giro terminar a thread fica ociosa
				estado = flag_aceleracao::nd; // se for interrompida, vai para a proxima acao
			rodaE->reset();
			rodaD->reset();
			rodaE->set_stop_action("hold");
			rodaD->set_stop_action("hold");
			break;

		case flag_aceleracao::parar :
			rodaE->stop();
			rodaD->stop();
			pwm_sp = 0;
			pwm_retardada = 0.0;
			pwm = 0;
			erro = 0;
			estado = flag_aceleracao::nd;
			break;
		}
	}
	rodaE->stop();
	rodaD->stop();
	if(arquivo_aberto){
		arquivo->fecha_arq();
		arquivo->string_arq("plot(t,x4);");
		arquivo_aberto = false;
	}
}
