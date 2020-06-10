#pragma once

/**
	 receive input safely and return false if user want to quit
 */
bool askInfo(std::string &input) {
	//executes loop if the input fails (e.g., no characters were read)
	while (std::cout << "Please input the expression, enter \"quit\" to quit:\n \u2192"
		&& !(std::cin >> input)) {
		std::cin.clear();													//clear bad input flag
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //discard input
		std::cout << "Invalid input. Please re-enter.\n";
	}

	if (input == "quit" || input == "Quit") {
		return false;
	}
	else {
		return true;
	}
}


/**
	return true if the input is a letter
 */
bool isletter(char input) {
	return ((input >= 'a' && input <= 'z') || (input >= 'A' &&input <= 'Z'));
}

bool isOp(char input) {
	return input == '&' || input == '|' || input == '!';
}

unsigned precedence(char input) {
	switch (input)
	{
	case '&':
	case '|':
		return 2;
	case '!':
		return 3;
	default:
		return 1;
	}
}

unsigned association(char input) {
	switch (input)
	{
	case '&':
	case '|':
		return LEFT_ASSOCIATIVE;
	case '!':
		return RIGHT_ASSOCIATIVE;
	default:
		return 0;
	}
}

/**
	record every variables in the expression
 */
void recordVar(std::unordered_map<char, bool> &vars, std::string input) {
	for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
		if (isletter(*it)) {
			if (vars.find((*it)) == vars.end()) {
				vars.insert(std::pair<char, bool>((*it), true));
			}
		}
	}
}

void printHeader(std::unordered_map<char, bool> vars) {
	for (std::unordered_map<char, bool>::iterator temp = vars.begin(); temp != vars.end(); ++temp) {
		std::cout << "\033[4m" << (*temp).first << "\t";
	}
	std::cout << "Result\n" << "\033[0m" << std::endl;
}

/**
	print elements in map
 */
void printMap(std::unordered_map<char, bool> vars) {
	for (std::unordered_map<char, bool>::iterator temp = vars.begin(); temp != vars.end(); ++temp) {
		std::cout << (((*temp).second) ? ("T") : ("F")) << "\t";
	}
	std::cout << evalution(rpn, vars) << std::endl;
}

/**
	draw the truth table based on variables
 */
void drawTruthTable(std::unordered_map<char, bool> &vars, std::unordered_map<char, bool>::iterator it) {
	//two temporary variables
	std::unordered_map<char, bool>::iterator temp = it;
	std::unordered_map<char, bool>::iterator temp2 = it;

	if (vars.size() > 0) {
		//end case
		if (std::next(it) == vars.end()) {
			printMap(vars);
			(*it).second = !(*it).second;
			printMap(vars);
			(*it).second = !(*it).second;
		}
		else {
			drawTruthTable(vars, ++it);
			(*temp).second = !(*temp).second;
			drawTruthTable(vars, ++temp);
			(*temp2).second = !(*temp2).second;
		}
	}
}

/**
	Return the reverse Polish notaion result of expression
 */
std::string RPN(std::string expr) {
	std::stack<char> stk;
	std::string output;

	for (std::string::iterator it = expr.begin(); it != expr.end(); ++it) {
		if ((*it) == ' ') {
			it++;
		}
		// Add variables to the output queue
		else if (isletter(*it)) {
			output += (*it);
		}
		// push '(' and '!' to the stack. 
		else if ((*it) == '(') {
			stk.push((*it));
		}
		else if ((*it) == '!') {
			output += (*++it);
			output += ('!');
		}
		//operators
		else if (isOp((*it))) {
			if (output.empty() && stk.empty() && (*it) != '!') {
				std::cerr << "You can't type left associative operators first.";
				break;
			}
			else if (it != expr.end() && (*it) == (*++it)) {
				while (!stk.empty() && isOp(stk.top())) {
					if ((association(*it) == LEFT_ASSOCIATIVE && precedence((*it)) < precedence(stk.top())) || \
						association((*it)) == RIGHT_ASSOCIATIVE && precedence((*it)) <= precedence(stk.top())) {
						output += (stk.top());
						stk.pop();
						continue;
					}
					break;
				}
				stk.push((*it));
			}
			else {
				std::cerr << "please input \'&&\' or \'||\' rather than \'&\' or \'|\'" << std::endl;
			}
		}

		// If the scanned character is an ��)��, pop and to output string from the stack 
		// until an ��(�� is encountered. 
		else if ((*it) == ')') {
			while (!stk.empty() && stk.top() != '(') {
				output += (stk.top());
				stk.pop();
			}
			stk.pop();
		}
	}

	//Pop all the remaining elements from the stack 
	while (!stk.empty()) {
		output += (stk.top());
		stk.pop();
	}

	return output;
}

bool evalution(std::string rpn, std::unordered_map<char, bool> vars) {
	std::stack<bool> stk;

	for (std::string::iterator it = rpn.begin(); it != rpn.end(); ++it) {
		if (isletter(*it)) {
			stk.push((vars.find(*it)->second));
		}
		else if (*it == '!') {
			bool temp = stk.top(); stk.pop();
			stk.push(!temp);
		}
		else {
			bool temp1 = stk.top(); stk.pop();
			bool temp2 = stk.top(); stk.pop();

			switch (*it) {
			case '&':
				stk.push(temp1 && temp2);
				break;
			case '|':
				stk.push(temp1 || temp2);
				break;
			default:
				break;
			}
		}
	}

	return stk.top();
}