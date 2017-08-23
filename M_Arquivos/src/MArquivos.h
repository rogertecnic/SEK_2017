#ifndef MARQUIVOS_H_
#define MARQUIVOS_H_
#include <iostream>
#include <stdarg.h>
#include <fstream>
#include <list>
#include <string>

#define tamanho_coluna 4

using namespace std;

class M_arquivos {
	list<double> elementos;
	std::list<double>::iterator it1, it2;

public:
	void elementos_arq (int tam, double xn, ...);
	void arquivo ();

private:
	string nome_arquivo = "mat.m";

};

#endif /*MARQUIVOS_H_ */
