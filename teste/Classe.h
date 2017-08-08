/*
 * Classe.h
 *
 *  Created on: 26 de jul de 2017
 *      Author: rogerio
 */

#ifndef CLASSE_H_
#define CLASSE_H_
#include <iostream>
#include <thread>

class Classe {
public:
	Classe(int);
	void func();
	std::thread t;
	int num;
};

#endif /* CLASSE_H_ */
