#ifndef ARQUIVOS_MAPEAMENTO_H_
#define ARQUIVOS_MAPEAMENTO_H_
#include <iostream>
#include <fstream>
#include "Const.h"
using namespace std;

class Arquivos_mapeamento {
public:
	void arquivo_map(direcao_checkpoint, list<no_intersec> );

private:
	ofstream outfile;
};



#endif
