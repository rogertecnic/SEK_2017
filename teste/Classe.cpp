/*
 * Classe.cpp
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */

#include "Classe.h"

Classe::Classe(int num)
:t(std::thread(&Classe::func, this)), num(num)
{
}


void Classe::func(){
	std::cout << "thread "<<num<<" rodando..."<<std::endl;
}
