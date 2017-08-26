#include "MArquivos.h"

int main() {
	M_arquivos arq("mat.m", 2);

	for (int i = 0; i < 10; i++){
		arq.elementos_arq((double)i, (double)i*100.0);
	}

	arq.fecha_arq();
	arq.string_arq("Hello");
	cout << "OK" <<endl;
	return 0;
}
