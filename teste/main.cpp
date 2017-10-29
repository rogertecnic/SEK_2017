#include "M_arquivos.h"
#include "src_folder/Sensor_cor.h"
#include "src_folder/Sensor_cor_hsv.h"
#include "src_folder/Controlador_robo.h"
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


int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");
	//teste_alinhamento_rampa();
	teste_map();
	cout << "Teste finalizado. Bye!" << endl;
	usleep (1000000);
	return 0;
}
