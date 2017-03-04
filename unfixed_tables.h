#pragma once
#include "fixed_tables.h"

namespace unfixed_tables {
	
		class Consts {
			array<vector <string>, 20> const_;
			int Hash(int c);
		public:
			bool AddConst(string c);
			string GetConst(int i, int j);
			bool FindConst(string c, int &i, int &j);
			void GetElements(array<vector<string>,20> &c);
		};

		//��������� ��������������� (����������) �������� � ����
		//������������ ���������� � ������� � �������� � ������ 
		//����������� ��������
		struct Identificator {
			string name;
			int i;
			int j;
		};

		class Identificators {
			array<vector <Identificator>, 20> identificator;
			int Hash(int id);
		public:
			bool AddParameter(string id);
			void ChangeParameter(string id, int i, int j);
			bool FindParameter(string id, int &i, int &j);
			Identificator GetParameter(int i, int j);
			void GetElements(array<vector <Identificator>,20> &id);
		};
}