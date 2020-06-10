/**
 * Author:	Kiran Bohyer & Yuxuan Luo
 * Date:	April 16th
 * 
 * Parse, calculate and generate truth table
 */
const LEFT_ASSOCIATIVE = 1;
const RIGHT_ASSOCIATIVE = 2;

var expression = "", rpn = "";

function calc(){
	//clean previous output
	document.getElementById("Result").innerText = "";

	var vars = [];

	expression = document.getElementById("expr").value;
	console.log(expression);		//for debug

	for(var i = 0; i < expression.length; i++){
		console.log(expression[i]);
	}

	recordVar(expression, vars);
	rpn = RPN(expression);
	
	for(var i = 0; i < rpn.length; i++){
		console.log(rpn[i]);
	}
	console.log(rpn[0]);
	console.log(vars[0].name);

	printHeader(vars);
	drawTruthTable(vars, 0, rpn);
}

function find(vars, key){
	for(var i = 0; i < vars.length; i++){
		if(vars[i].name == key){
			return vars[i].value.bool;
		}
	}
}
function isletter(input) {
	return ((input >= 'a' && input <= 'z') || (input >= 'A' &&input <= 'Z'));
}
function isOp(input) {
	return (input == '&' || input == '|' || input == '!');
}
function precedence(input) {
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
function association(input) {
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

//record variables in the input
function recordVar(input, vars) {
	var found = false;
	var steps = vars.length;

	for (var i = 0; i < input.length; i++) {
		if (isletter(input[i])) {
			for(var j = 0; j < steps; j++){
				found = false;
				if (vars[j].name == input[i]) {
					found = true;
				}
			}
			if(found == false){
				vars.push({name : input[i], value : {bool : true}});
			}
		}
	}
}

function printHeader(vars) {
	for (var i = 0; i < vars.length; i ++) {
		document.getElementById("Result").innerHTML += (vars[i].name + "\t");
	}
	document.getElementById("Result").innerHTML += "Result" + "</br>";
}

function printMap(vars, rpn) {
	for (var i = 0; i < vars.length; i++) {
		document.getElementById("Result").innerHTML += ((vars[i].value.bool) ? "T" : "F") + "   ";
	}
	document.getElementById("Result").innerHTML += evalution(rpn, vars) + '<br>';
}

function drawTruthTable(vars, index, rpn) {
	//two temporary indexes
	var tempIndex1 = index, tempIndex2 = index;

	if (vars.length > 0) {
		//end case
		if (index == (vars.length - 1)) {
			printMap(vars,rpn);
			vars[index].value.bool = !vars[index].value.bool;
			printMap(vars,rpn);
			vars[index].value.bool = !vars[index].value.bool;
		}
		else {
			drawTruthTable(vars, ++index, rpn);
			vars[tempIndex1].value.bool = !vars[tempIndex1].value.bool;
			drawTruthTable(vars, ++tempIndex1, rpn);
			vars[tempIndex2].value.bool = !vars[tempIndex2].value.bool;
		}
	}
}

function RPN(expr) {
	var stack = [], output = "", stringlength = expr.length;

	for (var i = 0; i < stringlength; i++) {
		if (expr[i] == ' ') {
			i++;
		}
		// Add variables to the output queue
		else if (isletter(expr[i])) {
			output += (expr[i]);
		}
		// push '(' and '!' to the stack. 
		else if (expr[i] == '(') {
			stack.push(expr[i]);
		}
		else if (expr[i] == '!') {
			output += (expr[i + 1]);
			output += '!';
		}
		//operators
		else if (isOp(expr[i])) {
			if (output.length == 0 && stack.length == 0 && (expr[i]) != '!') {
				console.log("You can't type left associative operators first.");
				break;
			}
			else if (i != stringlength && expr[i] == expr[i + 1]) {
				i++;
				while (stack.length != 0 && isOp(stack[stack.length - 1])) {
					if ((association(expr[i]) == LEFT_ASSOCIATIVE && precedence(expr[i]) < precedence(stack[stack.length - 1])) || 
						association(expr[i]) == RIGHT_ASSOCIATIVE && precedence(expr[i]) <= precedence(stack[stack.length - 1])) {
						output += (stack[stack.length - 1]);
						stack.pop();
						continue;
					}
					break;
				}
				stack.push(expr[i]);
			}
			else {
				console.log("please input \'&&\' or \'||\' rather than \'&\' or \'|\' \n");
			}
		}

		// If the scanned character is an ‘)’, pop and to output string from the stack 
		// until an ‘(’ is encountered. 
		else if (expr[i] == ')') {
			while (stack.length != 0 && stack[stack.length - 1] != '(') {
				output += (stack[stack.length - 1]);
				stack.pop();
			}
			stack.pop();
		}
	}

	//Pop all the remaining elements from the stack 
	while (stack.length != 0) {
		output += (stack[stack.length - 1]);
		stack.pop();
	}

	return output;
}
function evalution(rpn, vars) {
	var stack = [], stringlength = rpn.length;

	for (var i = 0; i < stringlength; i++) {
		if (isletter(rpn[i])) {
			stack.push(find(vars, rpn[i]));
		}
		else if (rpn[i] == '!') {
			var temp = stack[stack.length - 1];
			stack.pop();
			stack.push(!temp);
		}
		else {
			var temp1 = stack[stack.length - 1]; stack.pop();
			var temp2 = stack[stack.length - 1]; stack.pop();

			switch (rpn[i]) {
			case '&':
				stack.push(temp1 && temp2);
				break;
			case '|':
				stack.push(temp1 || temp2);
				break;
			default:
				break;
			}
		}
	}

	return stack[stack.length - 1];
}