/**
 *Logic Calculator
 *
 *Author:	Kiran Bohyer & Yuxuan Luo
 *Date:		April 2nd
 */

#include"header.h"
#include"functions.h"

int main() {
	std::string input;
	std::unordered_map<char, bool> vars;
	char buff;

	while (true) {
		#ifdef _WIN64
		system("CLS");
		#elif __linux__
		system("clear");
		#endif
		if (askInfo(input)) {
			recordVar(vars, input);
			rpn = RPN(input);

			std::unordered_map<char, bool>::iterator it = vars.begin();
			printHeader(vars);
			drawTruthTable(vars, it);

			vars.clear();

			
			#ifdef _WIN64
			system("PAUSE");
			#elif __linux__
			std::cout << "Press [ENTER] to continue..." << std::endl;
			system("read _");
			#endif
		}
		else {
			return 0;
		}
	}
}
