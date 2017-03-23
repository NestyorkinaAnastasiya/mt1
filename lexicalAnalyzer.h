#include "fixed_tables.h" 
#include "unfixed_tables.h" 
using namespace fixed_tables;
using namespace unfixed_tables;
namespace l_analyzer{
	struct Token{
		int table;
		pair <int, int> place;
		int type;
	};

	class LexicalAnalyzer {
		//вектор токенов
		vector <Token> tokens;
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
		void AnalyzeError(FILE *fo);
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
		LexicalAnalyzer();
		void Analize();				
	};
}
