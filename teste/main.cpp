#include "src_folder/Sensor_cor_hsv.h"
#include "src_folder/Controlador_robo.h"
#include "src_folder/Mapeamento.h"
#include "src_folder/Garra.h"
#include "src_folder/Resgate.h"
#include <fstream>


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

		if(corE == Cor::azul && corD == Cor::azul){
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
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);
	Ultrassom_nxt ultraD(Ultrassom_nxt::INPUT_4);

	robo.inicializar_thread_aceleracao();
	robo.calibra_sensor_cor(&cor);
	cout << "Fazer teste mapeamento" << endl;
	while(!ev3dev::button::enter.process());

	Mapeamento map(&robo, &cor, &ultraE, &ultraD);
	map.mapear();

}


void ler_cor(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Sensor_cor_hsv cor(ev3dev::INPUT_1, ev3dev::INPUT_2);

	robo.inicializar_thread_aceleracao();
	robo.calibra_sensor_cor(&cor);
	system ("clear");
	cout << "Fazer teste cor" << endl;
	while(!ev3dev::button::enter.process());

	while(true){
		cout << "Cor: " << cor.ler_cor_D() << "\t" << cor.ler_cor_E() << endl;
		usleep(1000);
	}
}


void teste_garra(){
	Garra garra(ev3dev::OUTPUT_C, 45, "cancela");

	cout << "Teste garra!!!" << endl;
	while(true){
		if(ev3dev::button::up.process()) garra.abrir();
		else if(ev3dev::button::down.process()) garra.fechar();
		else if(ev3dev::button::back.process()) break;
	}
}


void teste_leitura_ultra(){
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);

	while(!ev3dev::button::back.process()){
		cout<< "  " << ultraE.le_centimetro() << endl;
		usleep(1000000*0.5);
	}
}


void teste_pega_boneco(){
	Controlador_robo robo(true, "debug posicao direto no pwm.m");
	Garra garra(ev3dev::OUTPUT_D, 135, "garra");
	Garra cancela(ev3dev::OUTPUT_C, 45, "cancela");
	Ultrassom_nxt ultraE(Ultrassom_nxt::INPUT_3);

	ofstream outfile;
	outfile.open("ultra.txt", ios::out);

	robo.inicializar_thread_aceleracao();

	cout << "Fazer teste pega_boneco" << endl;
	while(!ev3dev::button::enter.process());

	while(!ev3dev::button::up.process()){
		robo.andar(70);
		outfile << ultraE.le_centimetro() << endl;

		if(ultraE.le_centimetro() <= 20){
			usleep(1000000*0.15);
			robo.parar();
			robo.andar(-30);
			while(ultraE.le_centimetro() > 20);
			robo.parar();

			cancela.abrir();
			garra.abrir();
			usleep(1000000*0.3);
			robo.girar(90);
			while(robo.get_estado() == flag_aceleracao::girar);

			robo.andar(30, 0.13);

			for(unsigned int i = 0; i < 3; i++){
				garra.fechar();
				robo.andar(20);
				usleep(1000000);
				garra.abrir();
				usleep(1000000);
			}
			robo.parar();
			garra.fechar();
			usleep(1000000*0.3);
			cancela.fechar();

			robo.andar(-30, 0.15);
			robo.girar(-90);
			while(robo.get_estado() == flag_aceleracao::girar);
			break;
		}
		usleep(1000000*0.2);
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

	//teste_alinhamento_rampa();
	//ler_cor();
	//teste_map();
	//teste_garra();
    //teste_raio_roda();
	//teste_distancia_entre_rodas();
	//teste_pega_boneco();
	teste_leitura_ultra();

	cout << "Teste finalizado. Bye!" << endl;
	usleep (1000000);
	return 0;
}
