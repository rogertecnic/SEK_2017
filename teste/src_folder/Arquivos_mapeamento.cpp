#include "Arquivos_mapeamento.h"

void Arquivos_mapeamento::arquivo_map(direcao_checkpoint s_cp){
	//list<no_intersec>::iterator it1 = vet.begin();
	int count = 1;

	outfile.open("mapeamento.txt", ios::out);

	if (outfile.is_open()) {
		outfile << "Intersecções" << endl;
		outfile << s_cp.checkpoint_vermelho << endl;
		outfile << s_cp.checkpoint_verde << endl;
		outfile << s_cp.checkpoint_azul << endl;

		outfile << "\nPosições bonecos" << endl;
		/*

		while(it1 != vet.end()){
			outfile << "Intersecção: " << count << endl;

			outfile << "Posição pre d:";
			for(unsigned i = 0; i < (*it1).posicao_pre_d.size(); i++)
				outfile << (*it1).posicao_pre_d[i] << " ";

			outfile << endl;

			outfile << "Posição pos d:";
			for(unsigned i = 0; i < (*it1).posicao_pos_d.size(); i++)
				outfile << (*it1).posicao_pre_d[i] << " ";

			outfile << endl;

			outfile << "Posição pre e:";
			for(unsigned i = 0; i < (*it1).posicao_pre_e.size(); i++)
				outfile << (*it1).posicao_pre_d[i] << " ";

			outfile << endl;

			outfile << "Posição pos e:";
			for(unsigned i = 0; i < (*it1).posicao_pos_e.size(); i++)
				outfile << (*it1).posicao_pre_d[i] << " ";

			outfile << "\n\n";

			it1++;
			count++;

		}*/


		outfile << "\n\nFim do arquivo!";



	}

	outfile.close();
}
