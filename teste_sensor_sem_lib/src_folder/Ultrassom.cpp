/*
 * Ultrassom.cpp
 *
 *  Created on: 5 de set de 2017
 *      Author: pcsek
 */

#include "Ultrassom.h"
#include <fstream>
#include <sstream>

Ultrassom::Ultrassom(string endereco)
:endereco(endereco)
{

}

double Ultrassom::le(){
	ifstream is(endereco, ifstream::in);
 double leitura = 0;
 is >> leitura;
 is.close();
	return leitura;
}
