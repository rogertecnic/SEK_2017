#include "Arquivos_mapeamento.h"

void Arquivos_mapeamento::arq_intersec(int vermelho, int verde, int azul){
	outfile.open("intersec.txt", ios::out);

	if (outfile.is_open()) {
		outfile << vermelho << endl;
		outfile << verde << endl;
		outfile << azul << endl;

	}

	outfile.close();
}
