#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

struct strutura{
	string s = "uma";
	int num = 19;
	bool A = false, B = true;
	vector<double> vetor1;
	vector<double> vetor2;
};

int main(){
	int u = 9;
	int *p = &u;
	// %p imprime em formato hexadecimal
	//	printf("%p\n",u); 		// value of u
	//	printf("%p\n",&u); 		// adress of u
	//	//printf("%p\n",*u); 	// erro
	//	printf("%p\n",p); 		// value of pointer to u "p"
	//	printf("%p\n",&p);		// adress of pointer to u "p"
	//	printf("%p\n",*p);		// value of u
	vector<double> v;
	v.push_back(56);
	list<strutura> lista;
	list<strutura>::iterator iterator_list = lista.begin();
	cout << "lista nao inicializada:" << endl;
	//	printf("%p", iterator_list); cout << "  value iterator" << endl;
	//	printf("%p", *iterator_list);cout << "  value iterator pointing" << endl;
	//	printf("%p", &iterator_list);cout << "  adress iterator" << endl;
	//	printf("%p", lista);cout << "  value lista" << endl;
	//	printf("%p", &lista);cout << "  adress lista" << endl;
	//
	strutura s;
	cout << "vector: " << v[0] << endl;
	cout << s.s << endl;
	lista.push_back(s);
	s.s= "duas";
	cout << s.s << endl;
	lista.push_back(s);
	s.s= "tres";
	cout << s.s << endl;
	lista.push_back(s);
	s.s= "quatro";
	cout << s.s << endl;
	lista.push_back(s);
	cout << "lista inicializada:" << endl;
	iterator_list = lista.begin();
	iterator_list --;
	auto it = iterator_list;
	iterator_list ++;
	//iterator_list ++;

	if(true) cout <<"ehh deu true, n era pra ter dado" << endl;
	if(iterator_list == lista.begin()) cout <<"iteradot no inicio" << endl;

	//	cout << (*iterator_list).s << endl;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;
	//	iterator_list ++;
	//	iterator_list --;
	//	auto it_no_anterior = iterator_list;
	//	iterator_list ++;
	//	cout << (*it_no_anterior).s << endl;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;
	//	iterator_list ++;
	//	cout << (*iterator_list).s << endl;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;
	//	iterator_list ++;
	//	cout << (*iterator_list).s << endl;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;
	//	iterator_list = lista.end();
	//	cout << (*iterator_list).s << endl;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;
	//	printf("ultima posicao: %d\n" , iterator_list);
	//	iterator_list ++;
	//	cout <<"distancia: "<< std::distance(lista.begin(), iterator_list) << endl;

	//	cout << (*iterator_list).s << endl;
	//	iterator_list ++;
	//	cout << (*iterator_list).s << endl;
	//	iterator_list ++;
	//	printf("%s", iterator_list->s);
	//	iterator_list ++;
	//	printf("%s", iterator_list->s);
	//	iterator_list ++;
	//	printf("%s", iterator_list->s);
	//	cout << (*iterator_list).s << endl;
	//	iterator_list --;
	//	cout << (*iterator_list).s << endl;
	//	iterator_list -- ;
	//	cout << (*iterator_list).s << endl;
	//	iterator_list --;
	//	cout << (*iterator_list).s << endl;
	//	iterator_list --;
	//	cout << (*iterator_list).s << endl;
	//	iterator_list --;
	//	cout << (*iterator_list).s << endl;
	return 0;
}


