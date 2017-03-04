#include  "fixed_tables.h"

namespace fixed_tables {
	
		KeyWords::KeyWords() {
			words[0] = "void";
			words[1] = "main";
			words[2] = "int";
			words[3] = "if";
			words[4] = "else";
		}

		int KeyWords::FindElement(string el) {
			for (int i = 0; i < words.size(); i++)
				if (words[i] == el) return i;
			return -1;
		}

		string KeyWords::GetElement(int number){
			return words[number];
		}

		KeyWords::~KeyWords() {}

		Separators::Separators() {
			separator[0] = ' ';
			separator[1] = '(';
			separator[2] = ')';
			separator[3] = '}';
			separator[4] = '{';
			separator[5] = '\t';
			separator[6] = '\n';
			separator[7] = ';';
		}
		int Separators::FindElement(char symbol) {
			for (int i = 0; i < separator.size(); i++)
				if (separator[i] == symbol) return i;
			return -1;
		}
		char Separators::GetElement(int number){
			return separator[number];
		}

		Separators::~Separators() {}
	
		SignOp::SignOp() {
			//���������� ��������
			signs[0] = "&&";
			signs[1] = "||";
			signs[2] = "!";

			//�������� ���������
			signs[3] = "==";
			signs[4] = "!=";
			signs[5] = ">";
			signs[6] = "<";
			signs[7] = ">=";
			signs[8] = "<=";
			
			//�������������� ��������
			signs[9] = "-";
			signs[10] = "+";
			signs[11] = "*";
			signs[12] = "/";

			//�������� ������������
			signs[13] = "=";
			signs[14] = "+=";
			signs[15] = "-=";
			signs[16] = "++";
			signs[17] = "--";
			signs[18] = "*=";
			signs[19] = "/=";
		}

		int SignOp::FindElement(string el){
			for (int i = 0; i < signs.size(); i++)
				if (signs[i] == el) return i;
			return -1;
		}

		SignOp::~SignOp() {}

		string SignOp::GetElement(int number){
			return signs[number];
		}
}