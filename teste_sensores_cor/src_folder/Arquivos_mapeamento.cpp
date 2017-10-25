#include "Arquivos_mapeamento.h"

void Arquivos_mapeamento::arquivo_map(){
	int it1 = 0;
	int count = 1;

	outfile.open("mapeamento.txt", ios::out);

	if (outfile.is_open()) {
		outfile << "Intersecções" << endl;
		outfile << "direcao do vermelho: "<< cp.checkpoint_vermelho << endl;
		outfile << "direcao do verde: " << cp.checkpoint_verde << endl;
		outfile << "direcao do azul: " << cp.checkpoint_azul << endl;

		outfile << "\nPosições bonecos" << endl;


		while(it1 < no.size()){
			outfile << "Intersecção: " << count << endl;

			outfile << "Posição pre d:";
			for(unsigned i = 0; i < no.at(it1).posicao_pre_d.size(); i++)
				outfile << no.at(it1).posicao_pre_d[i] << " ";

			outfile << endl;

			outfile << "Posição pos d:";
			for(unsigned i = 0; i < no.at(it1).posicao_pos_d.size(); i++)
				outfile << no.at(it1).posicao_pos_d[i] << " ";

			outfile << endl;

			outfile << "Posição pre e:";
			for(unsigned i = 0; i < no.at(it1).posicao_pre_e.size(); i++)
				outfile << no.at(it1).posicao_pre_e[i] << " ";

			outfile << endl;

			outfile << "Posição pos e:";
			for(unsigned i = 0; i < no.at(it1).posicao_pos_e.size(); i++)
				outfile << no.at(it1).posicao_pos_e[i] << " ";

			outfile << "\n\n";

			it1++;
			count++;

		}


		outfile << "\n\nFim do arquivo!";



	}

	outfile.close();
}
