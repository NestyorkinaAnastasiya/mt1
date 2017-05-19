#pragma once
#include <vector>
#include <array>
#include <string>
#include <fstream>
using namespace std;

namespace fixed_tables 
{

	class KeyWords 
	{
		array <string, 3> words;
	public:
		KeyWords();
		int FindElement(string el);
		string GetElement(int number);
		~KeyWords();
	};

	class Separators 
	{
		array<string, 9> separator;
	public:
		Separators();
		int FindElement(string symbol);
		string GetElement(int number);
		~Separators();
	};

	class SignOp 
	{
		array <string, 18> signs;
	public:
		SignOp();
		int FindElement(string el);
		string GetElement(int number);
		~SignOp();
	};
}
