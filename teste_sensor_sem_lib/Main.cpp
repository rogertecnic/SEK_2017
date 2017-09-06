#include <fstream>
#include <unistd.h>

#include "src_folder/Ultrassom_nxt.h"


int main(){

	system("setfont Greek-TerminusBold20x10.psf.gz");

	Ultrassom_nxt usE(Ultrassom_nxt::INPUT_3);
	Ultrassom_nxt usD(Ultrassom_nxt::INPUT_4);

	for(double i = -1000; i < 1000 ; i++ ){
		cout<<usE.le_centimetro()<<";"<<usD.le_centimetro()<<endl;
		usleep(1000*400);
	}

}
