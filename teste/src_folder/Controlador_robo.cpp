#include "Controlador_robo.h"


Controlador_robo::Controlador_robo(double fator_croda, bool debug, string nome_arquivo)
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
		arquivo = new M_arquivos(nome_arquivo, 5); // tempo, ang_rodaE, ang_rodaD, erro, pwm
		arquivo_aberto = true;
	}
}


void Controlador_robo::andar (int pwm_sp){
	estado = flag_aceleracao::linha_reta;
	usleep(1000*10);
	this->pwm_sp = pwm_sp;
}


void Controlador_robo::parar () {
	estado = flag_aceleracao::parar;

}


void Controlador_robo::girar(int angulo_robo_graus){
	this->angulo_robo_graus = angulo_robo_graus;
	estado = flag_aceleracao::girar;
	usleep(1000*10);
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
			usleep(1000*100);
			break;

		case flag_aceleracao::linha_reta :
			erro = rodaE->position()*fator_croda - rodaD->position();
			pid = kp*erro + kd*(erro-erro_anterior);

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
			cout<<pwm_sp<<endl;
			break;

		case flag_aceleracao::girar :
			rodaE->stop();
			rodaD->stop();
			pwm_sp = 0;
			pwm_retardada = 0.0;
			pwm = 0;
			usleep(1000*300);
			rodaE->set_position_sp(-(angulo_robo_graus*raio_robo/raio_roda));
			rodaD->set_position_sp((angulo_robo_graus*raio_robo/raio_roda));
			rodaE->set_speed_sp(120);
			rodaD->set_speed_sp(120);
			rodaE->run_to_rel_pos();
			rodaD->run_to_rel_pos();
			usleep(1000*1000);
			while((rodaE->speed() > 5 || rodaE->speed() < -5) &&
					estado == flag_aceleracao::girar){}

			if(estado == flag_aceleracao::girar) // se o giro terminar a thread fica ocios,
				estado = flag_aceleracao::nd;
			else {
				rodaE->stop(); // se o giro for interrompido os motores param
				rodaD->stop(); // bruscamente e a thread vai rodar o que interrompeu o giro
			usleep(1000*300);
			}
			break;

		case flag_aceleracao::parar :
			rodaE->stop();
			rodaD->stop();
			pwm_sp = 0;
			pwm_retardada = 0.0;
			pwm = 0;
			estado = flag_aceleracao::nd;
			break;
		}
	}
	if(arquivo_aberto){
		arquivo->fecha_arq();
		arquivo->string_arq("plot(t,x4);");
		arquivo_aberto = false;
	}
}
