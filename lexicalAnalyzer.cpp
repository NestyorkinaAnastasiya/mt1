#include "lexicalAnalyzer.h"
namespace l_analyzer{
	LexicalAnalyzer::LexicalAnalyzer()
	{
		fopen_s(&prog, "Program.txt", "r");
		eof = false;
		error = 0;
		line = 1;
	}
	void LexicalAnalyzer::AnalyzeSymbol()
	{
		symbol = fgetc(prog);
		if (symbol == EOF) eof = true;
		//константа
		else if ('0' <= symbol && symbol <= '9')
			condition = 1;
		//идентификатор
		else if ('a' <= symbol && symbol <= 'z' || 'A' <= symbol && symbol <= 'Z' || symbol == '_')
			condition = 2;
		//комментарий
		else if (symbol == '/')
			condition = 3;
		//знаки операций
		else if (symbol == '*'|| symbol == '+' || symbol == '-' || symbol == '=' || symbol == '!' || symbol == '>' || symbol == '<')
			condition = 4;
		//разделитель
		else if (symbol == ' ' || symbol == '(' || symbol == ')' || symbol == '}' || symbol == '{' || symbol == '\t' || symbol == '\n' || symbol == ';' || symbol == ',')
			condition = 5;
		//недопустимый символ
		else condition = 0;
	}

	void LexicalAnalyzer::Comment()
	{	//предыдущий символ
		char prev_symbol;
		//флаг, сигнализирующий о конце комментария
		bool endComment = false;

		AnalyzeSymbol();
		
		if (symbol == '/')
		{
			while (!eof && symbol != '\n')
			{ 
				prev_symbol = symbol;
				AnalyzeSymbol();
			}
			fseek(prog, ftell(prog) - 1, SEEK_SET(0));
		}
		else if (symbol == '*')
		{	//пока не закончен комментарий и не конец файла
			while (!endComment && !eof)
			{
				prev_symbol = symbol;
				AnalyzeSymbol();

				//если комментарий закончен
				if (prev_symbol == '*' && symbol == '/')
					endComment = true;
				//если комментарий не закрыт, то ошибка "незакрытый комментарий"
				if (eof && !endComment) error = 2;
			}
		}
		else 
		{	//иначе переходим в состояние операции
			symbol = '/';
			condition = 4;
			fseek(prog, ftell(prog)-1, SEEK_SET(0));
			Operation();
		}
	}

	void LexicalAnalyzer::Operation()
	{
		int position = ftell(prog);
		str = symbol;
		char prev_symbol = symbol;
		int prev_condition = condition;

		AnalyzeSymbol();
		if (!eof)
		{
			str += symbol;
			int i = operations.FindElement(str);
			if (i != -1)
			{
				//добавление токена (3, i, -1) 
				//второго измерения и типа нет в таблице
				Token op{ 3, { i, -1 }, false, line };
				tokens.push_back(op);
			}
			else
			{
				//восстановление позиции чтения в файле:
				fseek(prog, position, SEEK_SET(0));
				str = symbol = prev_symbol;
				condition = prev_condition;
				i = operations.FindElement(str);
				Token op{ 3, { i, -1 }, false, line};
				tokens.push_back(op);
			}
		}
	}

	void LexicalAnalyzer::Constanta()
	{
		str = symbol;
		char prev_symbol = symbol;
		int prev_condition = condition;

		AnalyzeSymbol();
		//пока символ не конец файла, не произошла ошибка и
		//не перешли в другое состояние
		while (!eof && !error && condition == 1)
		{	
				prev_symbol = symbol;
				prev_condition = condition;
				str += symbol;
				AnalyzeSymbol();
		}

		if (condition == 2) error = 3;
		//если не было ошибок формируем токен
		if (!error)
		{
			consts.AddConst(str);
			int i, j;
			consts.FindConst(str, i, j);
			Token c{ 4, { i, j }, false, line};
			tokens.push_back(c);
			//переход к последнему символу константы
			fseek(prog, ftell(prog)-1, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
		}
	}

	void LexicalAnalyzer::Identificator()
	{
		str = symbol;
		int position = ftell(prog);
		char prev_symbol = symbol;
		int prev_condition = condition;

		AnalyzeSymbol();
		//пока символ не конец файла, не произошла ошибка и
		//присутствует состояние буквы, цифры и нижнего подчёркивания
		while (!eof && !error && (condition == 1 || condition == 2))
		{	
			prev_symbol = symbol;
			prev_condition = condition;
			str += symbol;
			position++;
			AnalyzeSymbol();
		}
		//если не было ошибок формируем токен
		if (!error)
		{
			int k = keys.FindElement(str);
			//обработка ключевого слова
			if (k != -1)
			{
				Token key{ 1, { k, -1 }, false, line };
				tokens.push_back(key);
			}
			else //обработка идентификатора
			{
				identificators.AddParameter(str);
				int i, j;
				identificators.FindParameter(str, i, j);
				Token ident{ 5, { i, j }, false, line };
				tokens.push_back(ident);
			}

			//переход к предыдущему символу
			fseek(prog, position, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
		}

	}

	void LexicalAnalyzer::Separator()
	{
		char prev_symbol = symbol;
		int prev_condition = condition;
		if (symbol == ' ')
		{
			AnalyzeSymbol();
			//обработка множества пробеллов(их удаления)
			while (!eof && symbol == ' ')
			{
				prev_symbol = symbol;
				prev_condition = condition;
				AnalyzeSymbol();
			}
			//переход к предыдущему символу
			fseek(prog, ftell(prog)-1, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
		}
		else if (symbol == '\n')
		{
			line++;
			AnalyzeSymbol();
			//обработка множества пустых строк(их удаления)
			while (!eof && symbol == '\n')
			{
				line++;
				prev_symbol = symbol;
				prev_condition = condition;
				AnalyzeSymbol();
			}
			//переход к предыдущему символу
			fseek(prog, ftell(prog)-1, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
		}
		else if (symbol == '\t')
		{
			line++;
			AnalyzeSymbol();
			//обработка множества табуляций
			while (!eof && symbol == '\t')
			{
				line++;
				prev_symbol = symbol;
				prev_condition = condition;
				AnalyzeSymbol();
			}
			//переход к предыдущему символу
			fseek(prog, ftell(prog) - 1, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
		}
		else
		{
			str = "";
			str += symbol;
			Token sep{ 2, { separators.FindElement(str), -1 }, false, line};
			tokens.push_back(sep);
		}
	}

	void LexicalAnalyzer::AnalyzeError(ofstream &fo)
	{
		switch (error)
		{
		case 1: fo << "\nLA:: ERROR(line " << line << "):: Invalid character << " << symbol << " >>.";
			break;
		case 2: fo << "\nLA:: ERROR(line " << line << "):: Comment not closed.";
			break;
		case 3: fo << "\nLA:: ERROR(line " << line << "):: Incorrect sequence of characters << " << str + symbol << " >>.";
		}
	}
	void LexicalAnalyzer::Analize()
	{
		ofstream t("tokens.txt");
		do 
		{
			AnalyzeSymbol();
			if (!eof)
			switch (condition)
			{	//состояние константа
			case 1: Constanta();
				break;
				//состояние идентификатор
			case 2: Identificator();
				break;
				//состояние комментарий
			case 3: Comment();
				break;
				//состояние операция
			case 4: Operation();
				break;
				//состояние разделитель
			case 5: Separator();
				break;
				//ошибка недопустимый символ
			case 0: error = 1;
			}
		} while (!eof && !error);
		identificators.PrintElements(t);
		consts.PrintElements(t);
		for (auto i:tokens)
			t << "\n(" << i.table << ", (" << i.place.first <<", "<< i.place.second << "), " << i.type<< ")";
		if (error) AnalyzeError(t);
	}
}
