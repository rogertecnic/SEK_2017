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

private:
	list<double> elementos;
	std::list<double>::iterator it1, it2;
	string nome_arquivo;
	int tamanho_coluna;

};

#endif /*MARQUIVOS_H_ */
