/*
 * Ultrassom.h
 *
 *  Created on: 5 de set de 2017
 *      Author: pcsek
 */


#ifndef ULTRASSOM_H_
#define ULTRASSOM_H_


#include <iostream>
#include <string>

using namespace std;


class Ultrassom {
public:
	Ultrassom(string endereco);
	double le();

private:
	string endereco;
};

#endif /* ULTRASSOM_H_ */
