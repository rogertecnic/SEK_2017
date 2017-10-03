#ifndef ARQUIVOS_MAPEAMENTO_H_
#define ARQUIVOS_MAPEAMENTO_H_
#include <iostream>
#include <fstream>
#include "Mapeamento.h" // TODO inclusao circular entre Mapeamento.h e Arquivos_mapeamento.h

using namespace std;

class Arquivos_mapeamento {
public:
	void arquivo_map(status_checkpoint s_cp, list<no_intersec> vet);

private:
	ofstream outfile;
};



#endif
