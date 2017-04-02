#include "lexicalAnalyzer.h"
namespace l_analyzer{
	LexicalAnalyzer::LexicalAnalyzer()
	{
		fopen_s(&prog, "Program.txt", "r");
		eof = false;
		error = 0;
		line = 0;
		while (!eof && separators.FindElement(symbol) != -1)
			AnalyzeSymbol();
	}
	void LexicalAnalyzer::AnalyzeSymbol()
	{
		symbol = fgetc(prog);
		if (symbol == EOF) eof = true;
		//константа
		if ('0' <= symbol && symbol <= '9')
			condition = 1;
		//идентификатор
		else if ('a' <= symbol && symbol <= 'z' || 'A' <= symbol && symbol <= 'Z' || symbol == '_')
			condition = 2;
		//комментарий
		else if (symbol == '/')
			condition = 3;
		//знаки операций
		else if (symbol == '*', symbol == '+' || symbol == '-' || symbol == '=' || symbol == '!' || symbol == '>' || symbol == '<')
			condition = 4;
		//разделитель
		else if (separators.FindElement(symbol) != -1)
		{
			if (symbol == '\n') line++;
			condition = 5;
		}
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
				AnalyzeSymbol();
		}
		else if (symbol == '*')
		{	//пока не закончен комментарий и не конец файла
			while (!endComment && !eof)
			{
				prev_symbol = symbol;
				AnalyzeSymbol();

				//если комментарий закончен
				if (prev_symbol == '*' && condition == 3)
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
				Token op{ 3, { i, -1 }, -1};
				tokens.push_back(op);
			}
			else
			{
				str[2] = '\0';
				//восстановление позиции чтения в файле:
				fseek(prog, position, SEEK_SET(0));
				symbol = prev_symbol;
				condition = prev_condition;
				if (symbol != '!')
				{
					i = operations.FindElement(str);
					Token op{ 3, { i, -1 }, -1 };
					tokens.push_back(op);
				}
				//ошибка "неправильная последовательность символов"
				//нет отдельной операции '!'
				else error = 3;
			}
		}
	}

	void LexicalAnalyzer::Constanta()
	{
		str = symbol;
		int position = ftell(prog);
		char prev_symbol = symbol;
		int prev_condition = condition;

		AnalyzeSymbol();
		if (condition != 1) error = true;
		//пока символ не конец файла, не произошла ошибка и
		//не перешли в состояние разделителя
		while (!eof && !error && condition != 5)
		{	//если не цифра - ошибка
			if (condition != 1) error = 3;
			else
			{	
				prev_symbol = symbol;
				prev_condition = condition;
				str += symbol;
				position++;
				AnalyzeSymbol();
			}
		}
		//если не было ошибок формируем токен
		if (!error)
		{
			consts.AddConst(str);
			int i, j;
			consts.FindConst(str, i, j);
			Token c{ 4, { i, j }, -1 };
			tokens.push_back(c);
			//переход к последнему символу константы
			fseek(prog, position, SEEK_SET(0));
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
				Token key{ 1, { k, -1 }, -1 };
				tokens.push_back(key);
			}
			else //обработка идентификатора
			{
				identificators.AddParameter(str);
				int i, j;
				identificators.FindParameter(str, i, j);
				Token ident{ 5, { i, j }, -1 };
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
		str = symbol;
		int position = ftell(prog);
		char prev_symbol = symbol;
		int prev_condition = condition;
		if (symbol == ' ')
		{
			AnalyzeSymbol();
			//обработка множества пробеллов(их удаления)
			while (!eof && symbol == ' ')
			{
				position++;
				prev_symbol = symbol;
				prev_condition = condition;
				AnalyzeSymbol();
			}
			//переход к предыдущему символу
			fseek(prog, position, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
			Token sep{ 2, { separators.FindElement(' '), -1 }, -1 };
			tokens.push_back(sep);
		}
		else if (symbol == '\n')
		{
			AnalyzeSymbol();
			//обработка множества пустых строк(их удаления)
			while (!eof && symbol == '\n')
			{
				position++;
				prev_symbol = symbol;
				prev_condition = condition;
				AnalyzeSymbol();
			}
			//переход к предыдущему символу
			fseek(prog, position, SEEK_SET(0));
			symbol = prev_symbol;
			condition = prev_condition;
			Token sep{ 2, { separators.FindElement('\n'), -1 }, -1 };
			tokens.push_back(sep);
		}
		else
		{
			Token sep{ 2, { separators.FindElement(symbol), -1 }, -1 };
			tokens.push_back(sep);
		}
	}

	void LexicalAnalyzer::AnalyzeError(ofstream &fo)
	{
		switch (error)
		{
		case 1: fo << "\nLA:: ERROR(line "<< line <<"):: Invalid character << " << symbol << " >>.";
			break;
		case 2: fo << "\nLA:: ERROR:: Comment not closed.";
			break;
		case 3: fo << "\nLA:: ERROR(line "<< line <<"):: Incorrect sequence of characters << "<< str + symbol<< " >>.";
		}
	}
	void LexicalAnalyzer::Analize()
	{
		ofstream t("tokens.txt");
		do 
		{
			AnalyzeSymbol();
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
