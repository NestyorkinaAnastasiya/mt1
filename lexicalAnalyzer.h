#include "fixed_tables.h" 
#include "unfixed_tables.h" 
#include <iostream>
#include <sstream>
using namespace fixed_tables;
using namespace unfixed_tables;
namespace l_analyzer
{
	struct Token
	{
		// 1 - таблица ключевых слов
		// 2 - таблица разделителей
		// 3 - таблица знаков операции
		// 4 - таблица постоянных значений
		// 5 - таблица идентификаторов
		int table;
		pair <int, int> place;
		bool type;
		int line;
		bool Equal(Token tk){
			if (table == tk.table && type == tk.type && place == tk.place)
				return true;
			else return false;
		};
	};


	class LexicalAnalyzer  
	{
		//файл, содержащий текст программы 
		FILE *prog;
		//текущий символ файла
		char symbol;
		string str;
		//флаг конца файла
		bool eof;
		//текущая линия обработки
		int line;
		//тип ошибки
		// 1 - недопустимый символ
		// 2 - незакрытый комментарий
		// 3 - неверная последовательность символов
		int error;
		//состояние
		// 1 - константа
		// 2 - идентификатор
		// 3 - комментарий
		// 4 - знак операций
		// 5 - разделитель
		// 0 - недопустимый символ
		int condition;
		
		//анализ 1 символа (определение состояния)
		void AnalyzeSymbol();
		void AnalyzeError(ofstream &fo);
		//обработка комментария
		void Comment();
		//обработка операции
		void Operation();
		//обработка постоянного значения
		void Constanta();
		//обработка идентификатора
		void Identificator();
		//обработка разделителя
		void Separator();
	public:
		KeyWords keys;
		Separators separators;
		SignOp operations;
		Consts consts;
		Identificators identificators;
		//вектор токенов
		vector <Token> tokens;
		LexicalAnalyzer();
		void Analize();				
	};
}
