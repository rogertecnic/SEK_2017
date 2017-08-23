//============================================================================
// Name        : M_Arquivos.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <list>

#define tamanho_coluna 4
using namespace std;

list<double> elementos;
std::list<double>::iterator it1, it2;

void elementos_arq (int tam, double xn, ...);
void arquivo ();

int main() {
	cout << "!!!Hello World!!!" << endl;
	elementos_arq (tamanho_coluna, 2.3, 3.23, 4.0, 5.1);
	elementos_arq (tamanho_coluna, 1.2, 2.4, 10.1, 7.7);
	arquivo();
	return 0;
}


void elementos_arq (int tam, double xn, ...) {
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

void arquivo () {
	ofstream outfile;
	outfile.open ("mat.m");
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
