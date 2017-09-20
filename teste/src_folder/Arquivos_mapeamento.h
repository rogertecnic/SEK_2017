#ifndef ARQUIVOS_MAPEAMENTO_H_
#define ARQUIVOS_MAPEAMENTO_H_
#include <iostream>
#include <fstream>
#include "Mapeamento.h"

using namespace std;

class Arquivos_mapeamento {
public:
	void arq_intersec(int, int, int);

private:
	ofstream outfile;

};



#endif
