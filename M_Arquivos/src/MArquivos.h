#ifndef MARQUIVOS_H_
#define MARQUIVOS_H_
#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <list>
#include <string>

using namespace std;

class M_arquivos {
public:
	M_arquivos (string nome, int tamanho)
		: nome_arquivo (nome), tamanho_coluna (tamanho) {}

	void elementos_arq (double xn, ...);
	void fecha_arq ();
	void string_arq (string str);

private:
	list<double> elementos;
	list<double>::iterator it1, it2; //Iterador de controle da lista de elementos
	ofstream outfile; //Variável de controle do arquivo
	string nome_arquivo;
	int tamanho_coluna; //Quantidade de vetores

};

#endif /*MARQUIVOS_H_ */
