#include "src_folder/Ultrassom.h"

int main(){
	system("setfont Greek-TerminusBold20x10.psf.gz");

Ultrassom us("/sys/class/lego-sensor/sensor5/value0");
while(true){
	cout<<us.le()<<endl;
}
}
