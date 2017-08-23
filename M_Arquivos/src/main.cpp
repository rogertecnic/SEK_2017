#include "MArquivos.h"

int main() {
	M_arquivos arq;
	arq.elementos_arq (tamanho_coluna, 2.3, 3.23, 4.0, 5.1);
	arq.elementos_arq (tamanho_coluna, 1.2, 2.4, 10.1, 7.7);
	arq.arquivo();
	cout << "OK" <<endl;
	return 0;
}
