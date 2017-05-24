#include "GeneratorOfAssemblerCode.h"
namespace generator_ac
{
	void Generator::PrintHead()
	{
		file << ".386" << endl;
		file << ".MODEL FLAT, STDCALL"<< endl;
		file << "EXTERN ExitProcess@4: PROC" << endl;
	}

	void Generator::PrintVarDeclMode()
	{
		file << ".DATA" << endl;
		//печатаем объявлённые переменные
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < SA.LA.identificators.identificator[i].size(); j++)
				file << SA.LA.identificators.identificator[i][j].name + " DD ?" << endl;
	}

	void Generator::PrintStart()
	{
		file << ".CODE" << endl;
		file << "START PROC" << endl;
	}

	void Generator::PrintEnd()
	{
		file << "PUSH 0" << endl;
		file << "CALL ExitProcess@4" << endl;
		file << "START ENDP" << endl;
		file << "END START" << endl;
	}

	string Generator::Mov(string receiver, string source)
	{
		return "MOV " + receiver + ", " + source;
	}

	string Generator::Sub(string receiver, string source)
	{
		return "SUB " + receiver + ", " + source;
	}

	string Generator::Add(string receiver, string source)
	{
		return "ADD " + receiver + ", " + source;
	}

	string Generator::Mul(string source)
	{
		return "MUL " + source;
	}

	string Generator::Div(string source)
	{
		return "DIV " + source;
	}

	string Generator::And(string receiver, string source)
	{
		return "AND " + receiver + ", " + source;
	}

	string Generator::Or(string receiver, string source)
	{
		return "OR " + receiver + ", " + source;
	}

	// Управление операциями 
	void Generator::ManageOperation(Token source, Token receiver, string operation)
	{
		// Временная переменная 
		Token r = { 0,{ -1,-1 }, 0, 0 };
		int t_ns = source.table;
		int t_nd = receiver.table;

		if (operation == "+") Operation(source, receiver, 1);
		else if (operation == "-") Operation(source, receiver, 2);
		else if (operation == "*") Operation(source, receiver, 3);
		else if (operation == "/") Operation(source, receiver, 4);
		else if (operation == "=") Assignment(source, receiver);
		else if (operation == "+=" || operation == "-=" || operation == "*=" || operation == "/=")
		{
			if (operation == "+=") Operation(source, receiver, 1);
			else if (operation == "-=") Operation(source, receiver, 2);
			else if (operation == "*=") Operation(source, receiver, 3);
			else if (operation == "/=") Operation(source, receiver, 4);
			Assignment(r, receiver);
		}
		else if (operation == "!=")	ComparisonOperation(source, receiver, 1);
		else if (operation == "==")	ComparisonOperation(source, receiver, 2);
		else if (operation == ">")	ComparisonOperation(source, receiver, 3);
		else if (operation == "<")	ComparisonOperation(source, receiver, 4);
		else if (operation == ">=")	ComparisonOperation(source, receiver, 5);
		else if (operation == "<=")	ComparisonOperation(source, receiver, 6);
		else if (operation == "&&")	Operation(source, receiver, 5);
		else if (operation == "||")	Operation(source, receiver, 6);
	}
	// Функция, реализующая арифметические операции и операции присваивания
	void Generator::Operation(Token source, Token receiver, int operation)
	{
		int t_ns = source.table;
		int t_nd = receiver.table;

		// 2 const || receiver - const, source - id || 2 id || receiver - id, source - const
		if ((t_ns == 4 || t_ns == 5) && (t_nd == 4 || t_nd == 5)) 
		{
			file << Mov("EAX", GetValue(receiver)) << endl;
			file << OperationSelection(GetValue(source), "EAX", operation) << endl;
			file << "PUSH EAX" << endl;
		}
		// receiver - const, source - r
		if (t_ns == 0 && t_nd == 4) 
		{
			file << "POP EBX" << endl;
			file << Mov("EAX", GetValue(receiver)) << endl;
			file << OperationSelection("EBX", "EAX", operation) << endl;
			file << "PUSH EAX" << endl;
		}
		// source - const || id, receiver - r
		if ((t_ns == 4 || t_ns == 5 ) && t_nd == 0)
		{
			file << "POP EAX" << endl;
			file << OperationSelection(GetValue(source), "EAX", operation) << endl;
			file << "PUSH EAX" << endl;
		}
		// receiver - id, source - r
		if (t_ns == 0 && t_nd == 5) 
		{
			file << "POP EBX" << endl;
			file << Mov("EAX", GetValue(receiver)) << endl;
			file << OperationSelection("EBX", "EAX", operation) << endl;
			file << "PUSH EAX" << endl;
		}
		// 2 r
		if (t_ns == 0 && t_nd == 0) 
		{
			file << "POP EBX" << endl;
			file << "POP EAX" << endl;
			file << OperationSelection("EBX", "EAX", operation) << endl;
			file << "PUSH EAX" << endl;
		}
	}
	// Функция, реализующая присваивание
	void Generator::Assignment(Token source, Token receiver)
	{
		int t_ns = source.table;
		int t_nr = receiver.table;
		string result;
		// source - r
		if (t_ns == 0) 
		{
			file << "POP EAX" << endl;
			file << Mov(GetValue(receiver), "EAX") << endl;
		}
		// source - id || source - const
		if (t_ns == 4 || t_ns == 5)
		{
			result = Mov("EAX", GetValue(source));
			file << result << endl;
			result = Mov(GetValue(receiver), "EAX");
			file << result << endl;
		}
	}
	// Функция, реализующая операции сравнения
	void Generator::ComparisonOperation(Token op1, Token op2, int numberOfOperation)
	{
		int t_n1 = op1.table;
		int t_n2 = op2.table;
		stringstream ss;
		countCO++;
		ss << countCO;
		string nEq = ss.str();
		// Константы или идентификаторы
		if ((t_n1 == 4 || t_n1 == 5) && (t_n2 == 4 || t_n2 == 5)) 
			file << "CMP " + GetValue(op1) + "," + GetValue(op2) << endl;
		// r и (константы или идентификаторы)
		if (t_n1 == 0 && (t_n2 == 4 || t_n2 == 5)) 
		{
			file << "POP EAX" << endl;
			file << "CMP EAX," + GetValue(op2) << endl;
		}
		if (t_n2 == 0 && (t_n1 == 4 || t_n1 == 5)) 
		{
			file << "POP EAX" << endl;
			file << "CMP EAX," + GetValue(op1) << endl;
		}
		// 2 r
		if (t_n1 == 0 && t_n2 == 0) 
		{
			file << "POP EAX" << endl;
			file << "POP EBX" << endl;
			file << "CMP EAX, EBX" << endl;
		}
		switch (numberOfOperation)
		{
		case 1: 
		{
			// != верно->переходим по метке
			file << "JNE EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности !=
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по !=,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;			
		} break;
		case 2:
		{
			// == верно->переходим по метке
			file << "JE EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности ==
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по ==,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;
		} break;
		case 3:
		{
			// > верно->переходим по метке
			file << "JA EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности >
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по >,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;
		} break;
		case 4:
		{
			// < верно->переходим по метке
			file << "JB EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности <
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по <,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;
		} break;
		case 5:
		{
			// >= верно->переходим по метке
			file << "JAE EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности >=
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по >=,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;
		} break;
		case 6:
		{
			// <= верно->переходим по метке
			file << "JBE EQ" + nEq + "1" << endl;
			// Иначе кладём 0 и перепрыгиваем метку по верности <=
			file << "PUSH 0" << endl;
			file << "JMP EQ" + nEq + "2" << endl;
			// Метка по <=,  PUSH 1
			file << "EQ" + nEq + "1:" << endl;
			file << "PUSH 1" << endl;
			// Метка для перепрыгивания
			file << "EQ" + nEq + "2:" << endl;
		} break;
		}
	}

	string Generator::GetValue(Token tk)
	{
		string name = "";

		switch (tk.table)
		{
		case 1:
			name = SA.LA.keys.GetElement(tk.place.first);
			break;
		case 2:
			name = SA.LA.separators.GetElement(tk.place.first);
			break;
		case 3:
			name = SA.LA.operations.GetElement(tk.place.first);
			break;
		case 4:
			name = SA.LA.consts.GetConst(tk.place.first, tk.place.second);
		break;
		case 5:
			name = SA.LA.identificators.GetParameter(tk.place.first, tk.place.second).name;
		break;
		default:
			break;
		}

		return name;
	}

	string Generator::OperationSelection(string source, string receiver, int numberOfOperation)
	{
		switch (numberOfOperation)
		{
		case 1: return Add(receiver, source);
			break;
		case 2: return Sub(receiver, source);
			break;
		case 3: return Mul(source);
			break;
		case 4: return Div(source);
			break;
		case 5: return And(receiver, source);
			break;
		case 6: return Or(receiver, source);
			break;
		}
	}
	
	void Generator::PolskExpression(int num_polsk)
	{
		// Источник, приёмник
		Token source, receiver;
		// Временная переменная 
		Token r; 
		// Стек операндов
		vector <Token> SO; 
		// Строка для операции
		string str; 
		// Текущая опз
		vector <Token> curPolsk; 
		r = { 0, {-1,-1}, 0, 0 };

		for (int m = 0; m < SA.RPnotations[num_polsk].size(); m++)
			curPolsk.push_back(SA.RPnotations[num_polsk][m]);

		for (int k = 0; k < curPolsk.size(); k++)
		{
			//если операнд
			if (curPolsk[k].table == 4 || curPolsk[k].table == 5)
				SO.push_back(curPolsk[k]);
			else
			{
				str = GetValue(curPolsk[k]);
				// Достаём операнды
				source = SO[SO.size() - 1];
				receiver = SO[SO.size() - 2];
				SO.pop_back(); SO.pop_back();
				// Операция
				ManageOperation(source, receiver, str);
				SO.push_back(r);
			}
		}
		SO.clear();
	}

	Generator::Generator()
	{
		SA.Analyze();
		file.open("code.txt");
	}

	void Generator::Generate()
	{
		PrintHead();
		PrintVarDeclMode();
		PrintStart();

		for (int i = 0; i < SA.RPnotations.size(); i++)
			PolskExpression(i);

		PrintEnd();
		file.close();
	}
}
