#pragma once
#include <vector>
#include <array>
#include <string>
using namespace std;

namespace fixed_tables {

	class KeyWords {
		array <string, 5> words;
	public:
		KeyWords();
		int FindElement(string el);
		string GetElement(int number);
		~KeyWords();
	};

	class Separators {
		array<char, 8> separator;
	public:
		Separators();
		int FindElement(char symbol);
		char GetElement(int number);
		~Separators();
	};

	class SignOp {
		array <string, 20> signs;
	public:
		SignOp();
		int FindElement(string el);
		string GetElement(int number);
		~SignOp();
	};
}