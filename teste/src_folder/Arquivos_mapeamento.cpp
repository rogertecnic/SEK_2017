#include "Arquivos_mapeamento.h"

void Arquivos_mapeamento::arquivo_map(status_checkpoint s_cp){
	outfile.open("mapeamento.txt", ios::out);

	if (outfile.is_open()) {
		outfile << "Intersecções" << endl;
		outfile << s_cp.checkpoint_vermelho << endl;
		outfile << s_cp.checkpoint_verde << endl;
		outfile << s_cp.checkpoint_azul << endl;

		outfile << "\nPosições bonecos" << endl;

	}

	outfile.close();
}
