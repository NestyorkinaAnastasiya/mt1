#include "unfixed_tables.h"
#include <iostream>
using namespace fixed_tables;
using namespace unfixed_tables;

void main() {
	KeyWords kw;
	Separators s;
	SignOp so;
	Consts c;
	Identificators id;
	int k;
	string str;

	//тесты для постоянных таблиц
	cout << "[1]:" << endl;
	k = kw.FindElement("void");
	cout << "k = " << k << endl;
	k = kw.FindElement("double");
	cout << "k = " << k << endl;
	str = kw.GetElement(2);
	cout << "key_word[2] = " << str << endl << endl;

	cout << "[2]:" << endl;
	k = s.FindElement('\t');
	cout << "k = " << k << endl;
	k = s.FindElement('f');
	cout << "k = " << k << endl;
	str = s.GetElement(2);
	cout << "separaror[2] = " << str << endl << endl;

	cout << "[3]:" << endl;
	k = so.FindElement("==");
	cout << "k = " << k << endl;
	k = so.FindElement("===");
	cout << "k = " << k << endl;
	str = so.GetElement(2);
	cout << "sign_op[2] = " << str << endl << endl;
	
	//тесты для переменных таблиц
	cout << "[4]:" << endl;
	array<vector<string>,20> strs;
	c.AddConst("20");
	c.GetElements(strs);
	cout << "consts: " << endl ;
	for (int i = 0 ; i < 20; i++){
			cout << i << " : ";
			for (auto j : strs[i])
				cout << j << ' ';
			cout << endl;
	}

	int i, j;
	if(c.FindConst("20", i, j))
		cout << "i = " << i << ", j = " << j <<endl;
	else cout << "element isn't searched" << endl;
	
	str = c.GetConst(i, j);
	cout << "const["<< i << ", " << j << "] = " << str << endl << endl;

	cout << "[5]:" << endl;
	array<vector<Identificator>,20> ids;
	id.AddParameter("a");
	id.GetElements(ids);
	cout << "identificators: " << endl;
	for (int i = 0; i < 20; i++) {
		cout << i << " : ";
		for (auto j : ids[i])
			cout << "{" << j.name << ", " << j.i << ", " << j.j << "}";
		cout << endl;
	}

	id.ChangeParameter("a", 10, 0);
	id.GetElements(ids);
	cout << "identificators: " << endl;
	for (int i = 0; i < 20; i++) {
		cout << i << " : ";
		for (auto j : ids[i])
			cout << "{" << j.name << ", " << j.i << ", " << j.j << "}";
		cout << endl;
	}

	if (id.FindParameter("a", i, j))
		cout << "i = " << i << ", j = " << j << endl;
	else cout << "element isn't searched" << endl;

	Identificator ident = id.GetParameter(i, j);
	cout << "identificator[" << i << ", " << j << "] = {" << ident.name 
		<< ", " << ident.i << ", " << ident.j <<"}" << endl;

	getchar();
}