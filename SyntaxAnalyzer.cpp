#include "SyntaxAnalyzer.h"

namespace s_analyzer
{

	int LL_1::Search(string str)
	{
		for (int i = 0; i < terms.size(); i++)
			if (str == terms[i]) return i;

		return -1;
	}

	//Получение значения приоритета (расставлены в порядке убывания
	//1 - самый высокий, 8 - самый низкий
	int SyntaxAnalyzer::GetPriority(string operation)
	{
		if (operation == "!") return 1;
		if (operation == "*" || operation == "/") return 2;
		if (operation == "+" || operation == "-") return 3;
		if (operation == "<" || operation == ">" || operation == "<=" || operation == ">=")return 4;
		if (operation == "==" || operation == "!=") return 5;
		if (operation == "&&") return 6;
		if (operation == "||") return 7;
		if (operation == "=" || operation == "+=" || operation == "-=" || operation == "*=" || operation == "/=")return 8;
		return 0;
	}

	SyntaxAnalyzer::SyntaxAnalyzer()
	{
		ifstream ft("LL1_term.txt"), fj("LL1_jump.txt"), fa("LL1_accept.txt"),
			fst("LL1_stack.txt"), fr("LL1_return.txt"), ferr("LL1_error.txt");
		//заполнение таблицы разбора
		for (int i = 0; i < 78; i++)
		{
			LL_1 l;
			string s, s1 = "";
			
			getline(ft, s);
			for (auto i : s)
				if (i != ' ')
					s1 += i;
				else 
				{
					l.terms.push_back(s1);
					s1 = "";
				}
			l.terms.push_back(s1);
			fj >> l.jump;
			fa >> l.accept;
			fst >> l.stack;
			fr >> l.ret;
			ferr >> l.error;

			table.push_back(l);
		}
		ft.close(); fj.close(); fa.close(); fst.close(); fr.close(); ferr.close();
		numberOfToken = 0;
		LA.Analize();
	}

	string SyntaxAnalyzer::GetValue(Token tk)
	{
		switch(tk.table)
		{
		case 1: return LA.keys.GetElement(tk.place.first);
		break;
		case 2: return LA.separators.GetElement(tk.place.first);
		break;
		case 3: return LA.operations.GetElement(tk.place.first);
		break;
		case 4:	return "const";
		break;
		case 5:	return "id";
		break;
		default: break;
		}
	}

	void SyntaxAnalyzer::MakePolsk(stack <Token> &st_p, vector <Token> &res, Token tk, bool &error)
	{
		int t_n = tk.table;
		string str_tok = GetValue(tk);
		Token tmp_tk;
		int cur_prior, top_prior;
		//если токен - терминал
		if (t_n == 4 || t_n == 5) res.push_back(tk); 
		//если токен - знак операции
		if (t_n == 3)
		{
			//право-ассоциированный оператор
			if (str_tok == "=" || str_tok == "+=" || str_tok == "-=" || str_tok == "*=" || str_tok == "/=")
			{	//если стек не пуст и на его вершине оператор
				if (!st_p.empty() && st_p.top().table == 3)
				{
					//получаем приоритеты операторов
					cur_prior = GetPriority(str_tok);
					top_prior = GetPriority(GetValue(st_p.top()));

					//пока стек не пуст и на его вершине оператор и приоритет оператора на вершине ниже
					while (!st_p.empty() && st_p.top().table == 3 && cur_prior < top_prior)
					{
						//выталкиваем из стека и пишем в результат
						tmp_tk = st_p.top();
						res.push_back(tmp_tk);
						st_p.pop();
						//если на вершине оператор,то получаем его приоритет
						if (!st_p.empty() && st_p.top().table == 3) top_prior = GetPriority(GetValue(st_p.top()));
					}

				}
			}
			else //лево-ассоциированный оператор
			{
				//если стек не пуст и на его на вершине оператор
				if (!st_p.empty() && st_p.top().table == 3)
				{
					//получаем приоритеты операторов
					cur_prior = GetPriority(str_tok);
					top_prior = GetPriority(GetValue(st_p.top()));
					//пока стек не пуст и на его вершине оператор и приоритет оператора на вершине ниже 
					//или равен текущему
					while (!st_p.empty() && st_p.top().table == 3 && cur_prior >= top_prior)
					{
						//выталкиваем из стека и пишем в результат
						tmp_tk = st_p.top();
						res.push_back(tmp_tk);
						st_p.pop();
						//если на вершине оператор,то получаем его приоритет
						if (!st_p.empty() && st_p.top().table == 3) top_prior = GetPriority(GetValue(st_p.top()));
					}
				}
			}
			//кладём оператор в стек
			st_p.push(tk);
		}
	}

	void SyntaxAnalyzer::EndPolsk(stack <Token> &st_p, vector <Token> &res, bool &error)
	{
		Token tmp_tk;

		while (!st_p.empty() && !error)
		{
			tmp_tk = st_p.top();
			st_p.pop();
			// Если токен не принадлежит таблице знаков операций - ошибка
			if (tmp_tk.table != 3) { error = true; errText = "Not operation"; }
			else res.push_back(tmp_tk);
		}
		// Если ошибка, очищаем стек и результат
		if (error)
		{
			while (!st_p.empty())
				st_p.pop();
			res.clear();
		} 
		// Иначе если сформирован результат
		else if (res.size())
			{
				//добавляем текущую польскую запись в вектор польских записей 
				RPnotations.push_back(res);
				res.clear();
			}
	}

	void SyntaxAnalyzer::ToTheEndOfSentence()
	{
		Token cur_token;
		string str_tok;
		bool flag = true;
		while (flag)
		{
			if (LA.tokens.size() - numberOfToken - 1 > 0)
			{
				numberOfToken++;
				cur_token = LA.tokens[numberOfToken];
				flag = true;
			}
			else { flag = false; theEnd = true; }

			if (flag)
			{
				str_tok = GetValue(cur_token);
				if (str_tok == ";") flag = false;
			}
		}

		while (!stackPositions.empty()) stackPositions.pop();
		if (!correctStates.empty()) stackPositions = correctStates;
	}

	void SyntaxAnalyzer::WorkWithTable(string strToken)
	{
		// Принят токен или нет
		bool ok = false;
		//пока токен не принят и не возникла ошибка
		while (!ok && !error)
		{
			//если не нашли токен в списке "терминалов"
 			while (!error && table[curPosition].Search(strToken) == -1)
			{
				//если нет альтернативы выбора
				if (table[curPosition].error)
				{
					error = true;
					errText = "symbol <<" + strToken + ">> is not unexpected";
				}
				//если она есть
				else curPosition++;
			}
			if (!error)
			{
				ok = table[curPosition].accept;
				//если нетерминал
				if (!ok)
				{	//кладём в стек,если нужно
					if (table[curPosition].stack) stackPositions.push(curPosition);
					//возвращаемся,если нужно
					if (table[curPosition].ret)
					{
						curPosition = stackPositions.top();
						stackPositions.pop();
						curPosition++;
					}
					//иначе идём дальше по джампам
					else curPosition = table[curPosition].jump;
				}
				else
				{
					prevPosition = curPosition;
					//возврат обратно и переход на следующую строку
					if (table[curPosition].ret)
					{
						if (!stackPositions.empty())
						{
							curPosition = stackPositions.top();
							stackPositions.pop();
							curPosition++;
						}
					}
					else //когда принимаем,но возвращаться не надо
					{
						if (table[curPosition].jump != -1) curPosition = table[curPosition].jump;
					}
				}
			}
		}
	}
	
	void SyntaxAnalyzer::Analyze()
	{
		// true - есть токены для обработки
		bool tokensExist = true;		
		// Текущий токен
		Token curToken;
		// Текущий токен
		Token prevToken;
		// Текстовый эквивалент текущего токена
		string strToken;		
		// Можно ли закончить п.з.
		bool finP = true;
		// Можно ли начать п.з.
		bool begP = false; 
		bool begP_ = false;
		// Результат п.з.
		vector <Token> polsk; 
		// Список с описываемыми переменными
		vector <Token> describedID; 
		// Стек для п.з.
		stack <Token> stPolsk; 
		// Идёт описание
		bool description = false; 
		// Идентификатор для инициализации
		Identificator idForInit; 
		// Идёт инициализация
		bool init = false; 
		// Сообщение с ошибкой
		err errToPush; 

		if (LA.tokens.size())
			prevToken = curToken = LA.tokens[0];
		else { curPosition = 77; theEnd = true; }
		while (!theEnd)
		{
			error = false;
			errText = "";
			strToken = GetValue(curToken);
			//Работа с таблицей разбора
			WorkWithTable(strToken);
			
														/*проверка на описание*/
			// Начало описания (11 позиция - int)
			if (!error && prevPosition == 11) description = true;
			 // Кладём токен в список описанных идентификаторов (12 позиция - id)
			if (!error && description && (prevPosition == 12 || prevPosition == 22)) describedID.push_back(curToken);

			 // Если ошибка,всё аннулируется
			if (error && description)
			{
				description = false;
				Identificator local;

				// Нужно,если уже стоит метка,что инициализированы переменные
				for (int i = 0; i < describedID.size(); i++)
				{
					local = LA.identificators.GetParameter(describedID[i].place.first, describedID[i].place.second);
					// Убрать метку об объявлении типа переменной
					LA.identificators.ChangeType(local.name, false);
					// "Удаление" ссылки на таблицу констант
					LA.identificators.ChangeInit(local.name, false);
				}
				if (describedID.size()) describedID.clear();
			}

			// Предложение с описанием заканчивается(24 позиция - ;)
			if (!error && description && prevPosition == 24)
			{
				description = false;
				Identificator local;

				// Меняем атрибут описания у всех идентификаторов,что были описаны
				for (int i = 0; i < describedID.size(); i++)
				{
					local = LA.identificators.GetParameter(describedID[i].place.first, describedID[i].place.second);
					// Метка "тип определён"
					LA.identificators.ChangeType(local.name, true);
					// "Определение" инициализации
					LA.identificators.ChangeInit(local.name, local.init);
				}
			}

														/*проверка на инициализацию*/

			
			// Операция присваивания -> необходимо проверить, определён ли тип переменной
			if (!error && ((prevPosition > 34) && (prevPosition < 40) || prevPosition == 18))
			{
				for (auto &i: describedID)
					if (i.Equal(prevToken))
					{
						init = true;
						break;
					}
				idForInit = LA.identificators.GetParameter(prevToken.place.first, prevToken.place.second);
				// Тип не определён -> ошибка
				if (!init)
				{
					error = true;
					errText = "variable " + idForInit.name + " is not described";
				}
				// Для операций +=, -=, *=, /= нужна инициализация
				else if ((prevPosition > 34) && (prevPosition < 39) && !idForInit.init)
				{
					if (!idForInit.init)
					{
						error = true;
						errText = "variable " + idForInit.name + " is not initialized";
						init = false;
					}
				}
			}

			//инициализация заканчивается успешно
			if (!error && (prevPosition == 28 || prevPosition == 21 || prevPosition == 24) && init)
			{
				// Устанавливаем для идентификатора,что он инициализирован
				LA.identificators.ChangeInit(idForInit.name, true);
				init = false;
			}
													/*проверка переменных в операции*/
			//надо посмотреть,что там творится с переменной
			if (!error && prevPosition == 44)
			{
				Identificator a = LA.identificators.GetParameter(curToken.place.first, curToken.place.second);
				if (!a.init || !a.type) error = true;
				if (error)
				{
					if (!a.type) errText = "variable " + a.name + " is not described";
					else errText = "variable " + a.name + " is not initialized";
				}
			}

											/*запоминаем последнее корректное состояние*/
			if ((prevPosition == 5 || prevPosition == 21 || prevPosition == 24 || prevPosition == 28 ) && !error)
				if (!stackPositions.empty())
				{
					while (correctStates.size())
						correctStates.pop();
					correctStates = stackPositions;
					correctPosition = curPosition;
				}
				else correctPosition = curPosition;

														/*постфиксная запись*/
			//проверка,можно ли начинать составление п.з.
			if (!begP)
				if(!error && (prevPosition == 12 || prevPosition == 22 || prevPosition == 25))
				begP = true;

			if (!begP_)
				if (begP && !error && (prevPosition == 18 || (prevPosition > 34 && prevPosition < 40)))
				{
					begP_ = true;
					MakePolsk(stPolsk, polsk, prevToken, error);
				}

			if (begP && begP_ && prevPosition != 21 && prevPosition != 24 && prevPosition != 49)
				MakePolsk(stPolsk, polsk, curToken, error);

			//проверка,можно ли заканчивать составление п.з.
			if ((prevPosition == 21 || prevPosition == 24 || prevPosition == 28) && begP || error && begP)
			{
				EndPolsk(stPolsk, polsk, error); 
				begP = false;
				begP_ = false;
			}

												/*обработка ошибки и переход в корректное состояние*/
			if (error)
			{
				errToPush.Set(curToken, errText);
				//запоминаем строку,в которой ошибка и суть ошибки
				errors.push_back(errToPush);
				//прогоняем до конца предложения с переходом в корректное состояние
				ToTheEndOfSentence();
				//возвращаемся в последнюю корректную позицию
				curPosition = correctPosition;
			}
			

			//получаем следующий токен,если это возможно				
			if (LA.tokens.size() - numberOfToken - 1 > 0) 
			{
				prevToken = curToken;
				curToken = LA.tokens[numberOfToken+1];
				numberOfToken++;
			}
			else //токены закончились
				theEnd = true;
		}
		Output();
	}
	void SyntaxAnalyzer::PrintToken(ofstream &file, Token tok)
	{
		string name = "";

		switch (tok.table)
		{
		case 1:
			name = LA.keys.GetElement(tok.place.first);
			break;
		case 2:
			name = LA.separators.GetElement(tok.place.first);
			break;
		case 3:
			name = LA.operations.GetElement(tok.place.first);
			break;
		case 4:
			name = LA.consts.GetConst(tok.place.first, tok.place.second);
			break;
		case 5:
			name = LA.identificators.GetParameter(tok.place.first, tok.place.second).name;
			break;
		default:
			break;
		}

		file << name << ' ';
	}

	void SyntaxAnalyzer::Output()
	{
		ofstream fo("resultSA.txt");

		for (int i = 0; i <  RPnotations.size(); i++)
		{
			for (int j = 0; j <  RPnotations[i].size(); j++)
				PrintToken(fo, RPnotations[i][j]);
			fo << endl;
		}
		fo << endl << "SA errors:\n";
		for (int i = 0; i < errors.size(); i++)
			fo << "line " << errors[i].line << " " << errors[i].text << endl;
	}
}
