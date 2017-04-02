#pragma once
#include "fixed_tables.h"
#include <algorithm>

namespace unfixed_tables 
{
	
	class Consts 
	{
		array<vector <string>, 20> const_;
		int Hash(int c);
	public:
		bool AddConst(string c);
		string GetConst(int i, int j);
		bool FindConst(string c, int &i, int &j);
		void GetElements(array<vector<string>,20> &c);
		void PrintElements(ofstream &fo);
	};

	//Структура индентификатора (переменной) включает в себя
	//наименование переменной и позицию её значения в списке 
	//константных значений
	struct Identificator 
	{
		string name;
		bool type;
		int i;
		int j;
	};

	class Identificators 
	{
		array<vector <Identificator>, 20> identificator;
		int Hash(int id);
	public:
		bool AddParameter(string id);
		void ChangeType(string id, bool type);
		void ChangePosition(string id, int i, int j);
		bool FindParameter(string id, int &i, int &j);
		Identificator GetParameter(int i, int j);
		void GetElements(array<vector <Identificator>,20> &id);
		void PrintElements(ofstream &fo);
	};
}
