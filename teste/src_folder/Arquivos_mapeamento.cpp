#include "Arquivos_mapeamento.h"

void Arquivos_mapeamento::arquivo_map(status_checkpoint s_cp, list<no_intersec> vet){
	list<no_intersec>::iterator it1 = vet.begin();

	outfile.open("mapeamento.txt", ios::out);

	if (outfile.is_open()) {
		outfile << "Intersecções" << endl;
		outfile << s_cp.checkpoint_vermelho << endl;
		outfile << s_cp.checkpoint_verde << endl;
		outfile << s_cp.checkpoint_azul << endl;

		outfile << "\nPosições bonecos" << endl;

		while(){

			for(unsigned i = 0; i < (*it1).posicao_pre_d.size(); i++)
				outfile << (*it1).posicao_pre_d[i];

			for(unsigned i = 0; i < (*it1).posicao_pos_d.size(); i++)
				outfile << (*it1).posicao_pre_d[i];

			for(unsigned i = 0; i < (*it1).posicao_pre_e.size(); i++)
				outfile << (*it1).posicao_pre_d[i];

			for(unsigned i = 0; i < (*it1).posicao_pos_e.size(); i++)
				outfile << (*it1).posicao_pre_d[i];

			it1++;
		}



	}

	outfile.close();
}
