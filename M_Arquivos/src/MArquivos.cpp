#include "MArquivos.h"


void M_arquivos::elementos_arq (int tam, double xn, ...) {
	double valor;
	va_list elem;
	va_start (elem, xn);

	elementos.push_back(xn);
	for (int i = 0; i < tam-1; i++) {
		valor = va_arg(elem, double);
		elementos.push_back(valor);
	}
	va_end(elem);
}

void M_arquivos::arquivo () {
	ofstream outfile;
	outfile.open (nome_arquivo);
	int tamanho_linha = elementos.size()/tamanho_coluna;
	int count = 0;

	if (outfile.is_open()) {

		outfile << "t = [";
		while(elementos.size() > 0) {
			it1 = elementos.begin();
			it2 = elementos.begin();
			if (count > 0) outfile << "x" << count << " = [";
			for (int i = 0; i < tamanho_linha; i++) {
				if (i == tamanho_linha-1) outfile << *it2;
				else outfile << *it2 << ' ';
				advance (it2, tamanho_coluna-count);
				it1 = elementos.erase(it1);
				it1 = it2;
			}
			outfile << "];" << endl;
			count++;

		}
	}
}
