#include "unfixed_tables.h"
namespace unfixed_tables
{
	/* функции для работы с таблицей констант */

	//вычисление хеш-значения
	int Consts::Hash(int c)
	{
		return c % 20;
	}

	//нахождение константы с 
	bool Consts::FindConst(string c, int &i, int &j) 
	{
		i = Hash(int(c[0]));
		for (int k = 0; k < const_[i].size(); k++)
			if (const_[i][k] == c) 
			{
				j = k;
				return true;
			}
		j = -1;
		return false;
	}

	//добавление константы
	bool Consts::AddConst(string c) 
	{
		int i, j;
		if (FindConst(c, i, j)) return false;
		const_[Hash(int(c[0]))].push_back(c);
		return true;
	}

	//получение константы на позиции i, j
	string Consts::GetConst(int i, int j) 
	{
		return const_[i][j];
	}
	
	//получение всей таблицы в целом
	void Consts::GetElements(array<vector<string>, 20> &c) 
	{
		for (int i = 0; i < 20; i++) 
		{
			c[i].clear();
			for (auto j : const_[i])
				c[i].push_back(j);
		}
	}
	void Consts::PrintElements(ofstream &fo)
	{
		for (int i = 0; i < 20; i++)
		{
			fo << "\n[" << i << "] "; 
			for (int j = 0; j < const_[i].size(); j++)
				fo << const_[i][j] << " ";
		}
	}
	/* функции для работы с таблицей идентификаторов */

	//получение хеш-значения
	int Identificators::Hash(int id) 
	{
		return id % 20;
	}

	//нахождение идентификатора id
	bool Identificators::FindParameter(string id, int &i, int &j)
	{
		i = Hash(int (id[0]));
		for (int k = 0; k < identificator[i].size(); k++)
			if (identificator[i][k].name == id) {
				j = k;
				return true;
			}
		j = -1;
		return false;
	}

	//добавление параметра id
	bool Identificators::AddParameter(string id) 
	{
		int i, j;
		if (FindParameter(id, i, j)) return false;
		identificator[Hash(int(id[0]))].push_back({ id, false, false});
		return true;
	}

	void Identificators::ChangeType(string id, bool type)
	{
		int i, j;
		if (FindParameter(id, i, j))
			identificator[i][j].type = type;
	}

	//изменение значение параметра id
	void Identificators::ChangeInit(string id, bool init) 
	{
		int i, j;
		if (FindParameter(id, i, j)) 
			identificator[i][j].init = init;
	}

	//получение переменной из позиции i, j
	Identificator Identificators::GetParameter(int i, int j) 
	{
		return identificator[i][j];
	}

	//получение всей таблицы в целом
	void Identificators::GetElements(array<vector <Identificator>, 20> &id)
	{
		for (int i = 0; i < 20; i++) 
		{
			id[i].clear();
			for (auto j : identificator[i])
				id[i].push_back(j);
		}
	}

	void Identificators::PrintElements(ofstream &fo)
	{
		for (int i = 0; i < 20; i++)
		{
			fo << "\n["<< i << "] ";
			for (int j = 0; j < identificator[i].size(); j++)
				fo << "{"<< identificator[i][j].name << ", " << identificator[i][j].type <<", "<< identificator[i][j].init << "} ";
		}
	}
}
