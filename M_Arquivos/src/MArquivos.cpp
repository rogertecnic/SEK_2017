#include "MArquivos.h"



/*
 * Função elementos_arq extrai todas as variáveis passadas por parâmetro
 * e coloca numa lista de elementos.
 * A lista estará no modelo:
 * t1,x1.1, x2.1, ..., xn.1, ..., tm, xm.1, xm.2, ..., xm.n
 */

void M_arquivos::elementos_arq (double xn, ...) {
	double valor;
	va_list elem;
	va_start (elem, xn);

	elementos.push_back(xn);
	for (int i = 0; i < tamanho_coluna-1; i++) {
		valor = va_arg(elem, double);
		elementos.push_back(valor);
	}
	va_end(elem);
}



/*
 *  Função fecha_arq pega a lista gerada a cada chamada da função elemntos_arq
 *  e escreve em um arquivo.
 *  O arquivo estará no modelo:
 *
 *  t = [t1 t2 t3 ... tn];
 *  x1 = [x1.1 x1.2 x1.3 ... x1.n];
 *  ...
 *  xm = [xm.1 xm.2 xm.3 ... xmn];
 */

void M_arquivos::fecha_arq() {
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
