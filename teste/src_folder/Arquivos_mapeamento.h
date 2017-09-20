#ifndef ARQUIVOS_MAPEAMENTO_H_
#define ARQUIVOS_MAPEAMENTO_H_
#include <iostream>
#include <fstream>
#include "Mapeamento.h"

using namespace std;

class Arquivos_mapeamento {
public:
	Arquivos_mapeamento ()
	: nome_arquivo (""){}

	Arquivos_mapeamento(string nome)
	: nome_arquivo (nome){}

	void arq_intersec(int, int, int);

private:
	ofstream outfile;
	string nome_arquivo;

};



#endif
