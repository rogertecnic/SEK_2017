#include "src_folder/Sensor_cor_hsv.h"
#include "src_folder/Controlador_robo.h"
#include "src_folder/Garra.h"
#include "src_folder/Ultrassom_nxt.h"
#include "src_folder/Mapeamento.h"


using namespace std;


typedef chrono::high_resolution_clock Time;


void teste_alinhamento_rampa(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2);

	int corE, corD;
	double teta = 45;
	double AB = 7.5/tan(teta);
	double R = 7.5/cos(teta);
	double deslocacao;

	robo.inicializar_thread_aceleracao();
	robo.calibra_sensor_cor(&cor);


	cout << "Fazer teste alinhamento" << endl;
	while(!ev3dev::button::enter.process());
	robo.andar(70);

	while(true){
		corE = cor.ler_cor_E();
		corD = cor.ler_cor_D();

		if(corE == Cor::amarelo && corD == Cor::amarelo){
			break;
		}
	}

	robo.andar(30);
	while(cor.ler_cor_E() != Cor::branco || cor.ler_cor_D() != Cor::branco);
	robo.parar();
	robo.alinhar(&cor, direcao::traz);
	robo.andar(50, 0.17);

	robo.parar();


	robo.girar(teta);
	while(robo.get_estado() == flag_aceleracao::girar);

	while(cor.ler_cor_E() != Cor::branco){
		robo.andar(-30, 0.05);
		robo.girar(-teta);
		while(robo.get_estado() == flag_aceleracao::girar);
		if(cor.ler_cor_E() == branco) break;
		robo.girar(teta);
		while(robo.get_estado() == flag_aceleracao::girar);
	}

	robo.andar(30);
	while(cor.ler_cor_E() == Cor::branco);
	robo.parar();
	deslocacao = R - robo.get_pintao();
	robo.andar(-30, deslocacao/100);
	robo.girar(-teta);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(70);





}


void teste_map(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2);

	robo.inicializar_thread_aceleracao();
	cout << "Fazer teste mapeamento" << endl;
	while(!ev3dev::button::enter.process());

	Mapeamento map(&robo, &cor);
	map.mapear();

}



void ler_cor(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2, true, "cores");

	robo.inicializar_thread_aceleracao();
	robo.calibra_sensor_cor(&cor);
	system ("clear");
	cout << "Fazer teste cor" << endl;
	while(!ev3dev::button::enter.process());

	while(!ev3dev::button::up.process()){
		cout << "Cor: " << cor.ler_cor_E() << "\t" << cor.ler_cor_D() << endl;
		usleep(1000);
	}
	cor.fecha_arquivo();
}


void teste_garra(){
	Garra garra(ev3dev::OUTPUT_D, 27, "garra");
	Garra cancela(ev3dev::OUTPUT_C, 37, "cancela");

	cout << "Teste garra!!!" << endl;
	while(true){
		if(ev3dev::button::up.process()) garra.abrir();
		else if(ev3dev::button::down.process()) garra.fechar();
		else if(ev3dev::button::left.process()) cancela.abrir();
		else if(ev3dev::button::right.process()) cancela.fechar();
		else if(ev3dev::button::back.process()) break;
	}
}


void teste_leitura_ultra(){
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_4);

	while(!ev3dev::button::back.process()){
		cout<<ultraE.le_centimetro()<< endl;
		usleep(1000000*0.5);
	}
}


void teste_pega_boneco(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Garra garra(ev3dev::OUTPUT_D, 135, "garra");
	Garra cancela(ev3dev::OUTPUT_C, 45, "cancela");
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);

	robo.inicializar_thread_aceleracao();

	cout << "Fazer teste pega_boneco" << endl;
	while(!ev3dev::button::enter.process());

	robo.andar(25);
	while(!ev3dev::button::up.process());


	usleep(1000000*0.3);
	robo.parar();


	cancela.abrir();
	garra.abrir();

	robo.girar(90);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(30, robo.get_pintao()+0.02);

	garra.fechar();

	robo.andar(20);
	cout << "Vou entrar no for" << endl;
	for(unsigned count_resg = 0; count_resg < 1; count_resg++){
		garra.abrir();
		usleep(1000000);
		garra.fechar();
	}
	cout << "saindo do for" << endl;

	robo.parar();

	garra.fechar();
	cancela.fechar();

	robo.andar(-30, 0.135);
	robo.girar(-90);
	while(robo.get_estado() == flag_aceleracao::girar);



}


void realinha(Controlador_robo *robo, direcao lado_saindo) {
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

	else {
		cout << "realinha argumento errado" << endl;
		robo->parar();
		usleep(1000000*5);
	}
	usleep(1000000*0.3);
}


void teste_go_to_plaza(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv sensor(ev3dev::INPUT_1, ev3dev::INPUT_2, true, "cores");
	Garra garra(ev3dev::OUTPUT_D, 90, "garra");
	Garra cancela(ev3dev::OUTPUT_C, -35, "cancela");
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);

	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;
	int count_nwhite = 0;


	robo.inicializar_thread_aceleracao();
	robo.calibra_sensor_cor(&sensor);

	cout << "Fazer teste Go_To_Plaza" << endl;
	while(!ev3dev::button::enter.process());



	robo.andar(40);
	while(true){
		if(count_nwhite >= 10) break;

		if(sensor.ler_cor_E() != Cor::branco && sensor.ler_cor_D() != Cor::branco)
			count_nwhite++;
		else count_nwhite = 0;

	}
	robo.parar();

	robo.andar(40, 0.3);

	cancela.abrir();
	garra.abrir();

	robo.andar(-30);
	while(sensor.ler_cor_E() != Cor::branco || sensor.ler_cor_D() != Cor::branco);
	usleep(100000);

	robo.parar();
	garra.fechar();
	cancela.fechar();

	//robo.alinhar(&sensor, direcao::frente);

	garra.fechar();
	cancela.fechar();

	robo.girar(-90);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(50, 0.40);

	robo.girar(-90);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(50, 0.5);

	robo.girar(-90);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(30);
	while(ultraE.le_centimetro() < 30);
	robo.parar();
	robo.girar(90);
	while(robo.get_estado() == flag_aceleracao::girar);

	robo.andar(40);
	while(true){
		cor_E = sensor.ler_cor_E();
		cor_D = sensor.ler_cor_D();

		if(cor_E == Cor::fora)
			realinha(&robo, direcao::esquerda);
		else if (cor_D == Cor::fora)
			realinha(&robo, direcao::direita);

		if((cor_E == Cor::vermelho && cor_D == Cor::vermelho) ||
				(cor_E == Cor::verde && cor_D == Cor::verde))
		{
			robo.alinhar(&sensor, direcao::traz);
			robo.andar(30);
			while(sensor.ler_cor_E() != Cor::branco || sensor.ler_cor_D() != Cor::branco);
			usleep(1000000*0.5);
			robo.alinhar(&sensor, direcao::traz);
			robo.parar();
			break;
		}



	}
}



void teste_raio_roda(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	robo.inicializar_thread_aceleracao();
	robo.andar(30,2);
	robo.finalizar_thread_aceleracao();

}


void teste_distancia_entre_rodas(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	robo.inicializar_thread_aceleracao();
	robo.girar(360);

	/*
	while(true){
		robo.girar(90);
		while(robo.get_estado() == flag_aceleracao::girar);
		usleep(1000000*0.5);
	}
	robo.finalizar_thread_aceleracao();
	 */
	robo.finalizar_thread_aceleracao();
}


int main(){
	system ("clear");
	system("setfont Greek-TerminusBold20x10.psf.gz");

	//teste_go_to_plaza();
	//teste_alinhamento_rampa();
	//ler_cor();
	teste_map();
	//teste_garra();
	//teste_raio_roda();
	//teste_distancia_entre_rodas();
	//teste_pega_boneco();
	//teste_leitura_ultra();

	cout << "Teste finalizado. Bye!" << endl;
	usleep (1000000);
	return 0;
}
