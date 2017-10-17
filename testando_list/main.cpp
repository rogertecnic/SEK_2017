#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

struct strutura{
	string s = "aff";
	int num = 19;
	bool A = true, B = true;
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
	cout << s.s << endl;
	lista.push_back(s);
	s.s= "mano do ceu";
	cout << s.s << endl;
	lista.push_back(s);
	cout << "lista inicializada:" << endl;
	iterator_list = lista.begin();
	//
	//	printf("%p", iterator_list); cout << "  value iterator" << endl;
	//	printf("%p", *iterator_list);cout << "  value iterator pointing" << endl;
	//	printf("%p", &iterator_list);cout << "  adress iterator" << endl;
	//	printf("%p", lista);cout << "  value lista" << endl;
	//	printf("%p", &lista);cout << "  adress lista" << endl;
	cout << (*iterator_list).s << endl;
	iterator_list ++;
	cout << (*iterator_list).s << endl;
	iterator_list ++;
	printf("%s", iterator_list->s);
	iterator_list ++;
	printf("%s", iterator_list->s);
	iterator_list ++;
	printf("%s", iterator_list->s);
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
