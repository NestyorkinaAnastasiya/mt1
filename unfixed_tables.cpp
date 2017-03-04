#include "unfixed_tables.h"
namespace unfixed_tables{
	/* ������� ��� ������ � �������� �������� */

	//���������� ���-��������
	int Consts::Hash(int c){
		return c % 20;
	}

	//���������� ��������� � 
	bool Consts::FindConst(string c, int &i, int &j) {
		i = Hash(int(c[0]));
		for (int k = 0; k < const_[i].size(); k++)
			if (const_[i][k] == c) {
				j = k;
				return true;
			}
		j = -1;
		return false;
	}

	//���������� ���������
	bool Consts::AddConst(string c) {
		int i, j;
		if (FindConst(c, i, j)) return false;
		const_[Hash(int(c[0]))].push_back(c);
		return true;
	}

	//��������� ��������� �� ������� i, j
	string Consts::GetConst(int i, int j) {
		return const_[i][j];
	}
	
	//��������� ���� ������� � �����
	void Consts::GetElements(array<vector<string>, 20> &c) {
		for (int i = 0; i < 20; i++) {
			c[i].clear();
			for (auto j : const_[i])
				c[i].push_back(j);
		}
	}

	/* ������� ��� ������ � �������� ��������������� */

	//��������� ���-��������
	int Identificators::Hash(int id) {
		return id % 20;
	}

	//���������� �������������� id
	bool Identificators::FindParameter(string id, int &i, int &j){
		i = Hash(int (id[0]));
		for (int k = 0; k < identificator[i].size(); k++)
			if (identificator[i][k].name == id) {
				j = k;
				return true;
			}
		j = -1;
		return false;
	}

	//���������� ��������� id
	bool Identificators::AddParameter(string id) {
		int i, j;
		if (FindParameter(id, i, j)) return false;
		identificator[Hash(int(id[0]))].push_back({ id, -1, -1});
		return true;
	}

	//��������� �������� ��������� id
	void Identificators::ChangeParameter(string id, int str , int stolb) {
		int i, j;
		if (FindParameter(id, i, j)) {
			identificator[i][j].i = str;
			identificator[i][j].j = stolb;
		}
	}

	//��������� ���������� �� ������� i, j
	Identificator Identificators::GetParameter(int i, int j) {
		return identificator[i][j];
	}

	//��������� ���� ������� � �����
	void Identificators::GetElements(array<vector <Identificator>, 20> &id){
		for (int i = 0; i < 20; i++) {
			id[i].clear();
			for (auto j : identificator[i])
				id[i].push_back(j);
		}
	}
}