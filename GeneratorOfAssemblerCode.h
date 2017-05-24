#pragma once
#include "SyntaxAnalyzer.h"
using namespace s_analyzer;

namespace generator_ac
{	
	class Generator
	{	// Файл с результирующим кодом
		ofstream file;
		// Блок синтаксического анализатора
		SyntaxAnalyzer SA;
		// Число выполненных функций сравнения
		int countCO = 0;
		// Печать заголовка
		void PrintHead();
		// Печать переменных
		void PrintVarDeclMode();
		// Печать начала программы
		void PrintStart();
		// Печать конца программы
		void PrintEnd();
		// Строковое описание ассемблеровской функции присваивания
		string Mov(string receiver, string source);
		// Строковое описание ассемблеровской функции вычитания
		string Sub(string receiver, string source);
		// Строковое описание ассемблеровской функции сложения
		string Add(string receiver, string source);
		// Строковое описание ассемблеровской функции умножения
		string Mul(string source);
		// Строковое описание ассемблеровской функции деления
		string Div(string source);
		// Строковое описание ассемблеровской функции &&
		string And(string receiver, string source);
		// Строковое описание ассемблеровской функции ||
		string Or(string receiver, string source);

		// Управление операциями 
		void ManageOperation(Token source, Token receiver, string operation);
		// Функция, реализующая арифметические операции, операции присваивания, && и ||
		void Operation(Token source, Token receiver, int operation);
		// Функция, реализующая присваивание
		void Assignment(Token source, Token receiver);
		// Функция, реализующая операции сравнения
		void ComparisonOperation(Token op1, Token op2, int numberOfOperation);
		// Получение строкового значения токена
		string GetValue(Token tk);
		// Выбор и реализация операции
		string OperationSelection(string source, string receiver, int numberOfOperation);
		// Обработка одного выражения
		void PolskExpression(int num_polsk); 
	public:
		Generator();
		void Generate();
	};
}
