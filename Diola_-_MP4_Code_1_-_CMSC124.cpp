/*


Grammars:

Infix

<expr> ::= <term> {(+|-)<term>}
<term> ::= <factor>{(*|/|^)<factor>}
<factor> ::= <number> | (<expr>) | <first_ident> | <letter>
<number> ::= 0|1|2|3|4|5|6|7|8|9| <number>
<first_ident> ::= <underscore><ident> | <alpha><ident>
<ident> ::= <first_ident> | <digit><ident>


Prefix Grammar:

draft:
<expr> ::= <op><expr><term> | <term>
<term> ::= <const> | <ident>

draft:
<expr> ::= <op><operands> | <term>
<operands> ::= <expr><term>

draft:
<expr> ::= (+|-|*|/|^)<operands> | <term>
<operands> ::= <expr><term>
<term> ::= <digit> | <letter>

<expr> ::= (+|-|*|/|^)<expr><expr> | <term>
<term> ::= <digit> | <letter>

Postfix Grammar:

<expr> ::= <term><expr><op> | <term>
<term> ::= <const> | <ident>



*/

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;


enum ExpressionType{


	INFIX,
	PREFIX,
	POSTFIX,
	ERROR_EXPR

};

/*
( :0
) :1
+ :2
- :3
* :4
/ :5
^ :6
a-zA-Z :7
_ :8
0-9 :9
err :10
end :11
*/

enum TokenType{


	L_PAR,
	R_PAR,

	ADD_OP,
	SUB_OP,
	MULT_OP,
	DIV_OP,
	EXP_OP,

	LETTER,
	UNDERSCORE,
	DIGIT,

	
	ERROR_TOKEN,
	END

};

class ExpressionParser{

	protected:

	string input;
	char current_char;
	TokenType token_type;
	int iter;
	bool valid;

	int next_char(void){
		int input_length = input.length();
		if(iter>=input_length){
			current_char = '\0';
			return -1;
		}
		
		iter++;
		current_char = input[iter];

		return 0;
	}

	// virtual TokenType peek(void) = 0;

	void skip_whitespace() {
		while (isspace(current_char)) 
			next_char();
	}

	virtual int lex() = 0;

	virtual int starting_expression() = 0; //This is equivalent to the starting symbol in the formal definition of CFG

	public:

	ExpressionParser(string init_input){
		input = init_input;
		current_char = 0;
		iter = -1;
		valid = true;
	}

	virtual int parse(void){
		// cout << "string received:  " << input << endl;
		lex();
		starting_expression();
		
		if(token_type != END){ //This means that parsing is not finished
			valid = false;
		}
		// cout << "current char at parse: " << current_char << endl; 
		// cout << "current token type at parse: " << token_type << endl; 
		if(valid && token_type != ERROR_TOKEN){
			// cout << "VALID" << endl;
			return 0;
		} else {
			// cout << "INVALID" << endl;
			return 1;
		}
		return -1;
	}


};

class InfixExpressionParser : public ExpressionParser{

	private:

	int lex(){
		next_char();
		skip_whitespace();
		if(current_char == '+'){
			token_type = ADD_OP;
		} 
		else if(current_char == '-'){
			token_type = SUB_OP;
		}
		else if(current_char == '*'){
			token_type = MULT_OP;
		}
		else if(current_char == '/'){
			token_type = DIV_OP;
		}
		else if(current_char == '^'){
			token_type = EXP_OP;
		}
		else if(current_char == '('){
			token_type = L_PAR;
		}
		else if(current_char == ')'){
			token_type = R_PAR;
		}
		else if(current_char >= '0' && current_char <= '9'){
			token_type = DIGIT;
		}
		else if(current_char == '_'){
			token_type = UNDERSCORE;
		}
		else if((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')){
			token_type = LETTER;
		} 
		else if(current_char == '\0'){
			token_type = END;
		}
		else {
			token_type = ERROR_TOKEN;
		}


		return 0;
	}

	

	int starting_expression(){
		expr();
		return 0;
	}

	int expr(){
		term();
		while(token_type == ADD_OP || token_type == SUB_OP){
			lex();
			term();
		}
		return 0;
	}

	int term(){
		factor();
		while(token_type == MULT_OP || token_type == DIV_OP || token_type == EXP_OP){
			lex();
			factor();
		}
		return 0;
	}

	int factor(){
		if(token_type == L_PAR){
			lex();
			expr();
			if(token_type != R_PAR){
				valid = false;
			}
			lex();
		}
		else if(token_type == DIGIT){
			number();
		}
		else if(token_type == LETTER){
			lex();
		} else {
			valid = false;
		}
		return 0;
	}
	int number(){
		lex();
		while(token_type == DIGIT){
			lex();
		}
		return 0;
	}


	public:

	InfixExpressionParser(string expression) : ExpressionParser(expression){

		// cout << "Expression Received: " << expression << endl;
	}


};


class PrefixExpressionParser : public ExpressionParser{

	private:

	int lex(){
		next_char();
		skip_whitespace();
		if(current_char == '+'){
			token_type = ADD_OP;
		} 
		else if(current_char == '-'){
			token_type = SUB_OP;
		}
		else if(current_char == '*'){
			token_type = MULT_OP;
		}
		else if(current_char == '/'){
			token_type = DIV_OP;
		}
		else if(current_char == '^'){
			token_type = EXP_OP;
		}
		else if(current_char >= '0' && current_char <= '9'){
			token_type = DIGIT;
		}
		else if((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')){
			token_type = LETTER;
		} 
		else if(current_char == '\0'){
			token_type = END;
		}
		else {
			token_type = ERROR_TOKEN;
		}


		return 0;
	}

	

	int starting_expression(){
		expr();
		return 0;
	}

	int expr(){
		if(token_type == ADD_OP || token_type == SUB_OP || token_type == MULT_OP || token_type == DIV_OP || token_type == EXP_OP){
			lex();
			expr();
			expr();
		}
		else{
			term();
		}
		return 0;
	}


	int term(){
		if(token_type == DIGIT || token_type == LETTER){
			lex();
		}
		return 0;
	}
	

	


	public:

	PrefixExpressionParser(string expression) : ExpressionParser(expression){

		// cout << "Expression Received: " << expression << endl;
	}


};


//I had to use a stack for postfix parsing since I need to product the parse tree from bottom up, it is kept to a minumum to maintain similarity with prefix and infix parsing.
class PostfixExpressionParser : public ExpressionParser{

	private:

	int lex(){
		next_char();
		skip_whitespace();
		if(current_char == '+'){
			token_type = ADD_OP;
		} 
		else if(current_char == '-'){
			token_type = SUB_OP;
		}
		else if(current_char == '*'){
			token_type = MULT_OP;
		}
		else if(current_char == '/'){
			token_type = DIV_OP;
		}
		else if(current_char == '^'){
			token_type = EXP_OP;
		}
		else if(current_char >= '0' && current_char <= '9'){
			token_type = DIGIT;
		}
		else if((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z')){
			token_type = LETTER;
		} 
		else if(current_char == '\0'){
			token_type = END;
		}
		else {
			token_type = ERROR_TOKEN;
		}


		return 0;
	}

	

	int starting_expression(){
		
		expr();
	
		return 0;
	}

	int expr(){

		stack<char> token_stack;
		
		while(token_type != END && token_type != ERROR_TOKEN){
			if(token_type == DIGIT || token_type == LETTER){
				token_stack.push(current_char);
				lex();
			} else if(token_type == ADD_OP || token_type == SUB_OP || token_type == MULT_OP || token_type == DIV_OP || token_type == EXP_OP){
				if(token_stack.size()<2){
					valid = false;
					return 1;
				}
				token_stack.pop();
				lex();
			} else{
				break;
			}
		}
		if(token_stack.size() != 1){
			valid = false;
		}
		// term();
		

		return 0;
	}


	// int term(){
	// 	if(token_type == DIGIT || token_type == LETTER){
	// 		lex();
	// 	}
	// 	return 0;
	// }

	// int op(){
	// 	if(token_type == ADD_OP || token_type == SUB_OP || token_type == MULT_OP || token_type == DIV_OP || token_type == EXP_OP){
	// 		lex();
	// 	}
	// 	return 0;
	// }
	

	


	public:

	PostfixExpressionParser(string expression) : ExpressionParser(expression){

		// cout << "Expression Received: " << expression << endl;
	}


};

class Expression{


	private:

		string expression;
		ExpressionType type;

		bool is_valid_infix() const{
			InfixExpressionParser parser(expression);
			if(parser.parse() == 0){
				return true;
			} else{
				return false;
			}
		}

		bool is_valid_prefix() const{
			PrefixExpressionParser parser(expression);
			if(parser.parse() == 0){
				return true;
			} else{
				return false;
			}
		}

		bool is_valid_postfix() const{
			PostfixExpressionParser parser(expression);
			if(parser.parse() == 0){
				return true;
			} else{
				return false;
			}
		}

		void evaluate_type(void){

			if(this->is_valid_infix()){
				type = INFIX;
			} 
			else if(this->is_valid_prefix()){
				type = PREFIX;
			} 
			else if(this->is_valid_postfix()){
				type = POSTFIX;
			}
			else {
				type = ERROR_EXPR;
			}
			
		}

		int get_priority(char c){
			if (c == '-' || c == '+')
				return 1;
			else if (c == '*' || c == '/')
				return 2;
			else if (c == '^')
				return 3;
			return 0;
		}

		int get_least_priority(string expression){
			int prio = 100;
			int par_encountered = 0;
			for(int i=0; i<expression.length(); i++){
				if(is_operator(expression[i])){
					if(get_priority(expression[i]) < prio && par_encountered <2){
						prio = get_priority(expression[i]);
					}
				}
				if(expression[i] == '('){
					par_encountered++;
				} else if (expression[i] == ')'){
					par_encountered--;
				}
			}
			return prio;
		}

		bool is_operator(char c){
			if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^'){
				return true;
			}
			return false;
		}

		bool is_operand(char c){
			if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')){
				return true;
			}
			return false;
		}

	public:


		Expression(string input){
			expression = input;
			this->evaluate_type();
		}

		string infix_to_prefix(){

			if(type != INFIX){
				return "error";
			}

			string prefix = "";
			string infix = expression;
			stack<char> op_stack;

			reverse(infix.begin(), infix.end());

			for(int i=0; i<infix.length(); i++){
				if(is_operand(infix[i])){
					prefix += infix[i];
				}
				else if(is_operator(infix[i]) && op_stack.empty()){
					op_stack.push(infix[i]);
				}
				else if(is_operator(infix[i])){
					if(get_priority(infix[i]) >= get_priority(op_stack.top())){
						op_stack.push(infix[i]);
					} 
					else if (get_priority(infix[i]) < get_priority(op_stack.top())){
						while(!op_stack.empty() && get_priority(infix[i]) < get_priority(op_stack.top())){
							prefix += op_stack.top();
							op_stack.pop();
						}
						op_stack.push(infix[i]);
					}
				}
				else if(infix[i] == ')'){
					op_stack.push(infix[i]);
				}
				else if(infix[i] == '('){
					while(!op_stack.empty() && op_stack.top() != ')'){
						prefix += op_stack.top();
						op_stack.pop();
					}
					if(!op_stack.empty()){
						op_stack.pop();
					}
					
				}
			}
			while(!op_stack.empty()){
				prefix += op_stack.top();
				op_stack.pop();
			}

			reverse(prefix.begin(), prefix.end());

			return prefix;
		}

		string infix_to_postfix(){

			if(type != INFIX){
				return "error";
			}

			string postfix = "";
			string infix = expression;
			stack<char> op_stack;

			for(int i=0; i<infix.length(); i++){
				if(is_operand(infix[i])){
					postfix += infix[i];
				}
				else if(is_operator(infix[i]) && op_stack.empty()){
					op_stack.push(infix[i]);
				}
				else if(is_operator(infix[i])){
					if(get_priority(infix[i]) > get_priority(op_stack.top())){
						op_stack.push(infix[i]);
					} 
					else if (get_priority(infix[i]) <= get_priority(op_stack.top())){
						while(!op_stack.empty() && get_priority(infix[i]) <= get_priority(op_stack.top())){
							postfix += op_stack.top();
							op_stack.pop();
						}
						op_stack.push(infix[i]);
					}
				}
				else if(infix[i] == '('){
					op_stack.push(infix[i]);
				}
				else if(infix[i] == ')'){
					while(!op_stack.empty() && op_stack.top() != '('){
						postfix += op_stack.top();
						op_stack.pop();
					}
					if(!op_stack.empty()){
						op_stack.pop();
					}
				}
			}
			while(!op_stack.empty()){
				postfix += op_stack.top();
				op_stack.pop();
			}
			

			return postfix;
		}

		string prefix_to_infix(){


			if(type != PREFIX){
				return "error";
			}

			string infix = "";
			string prefix = expression;
			stack<string> op_stack;

			reverse(prefix.begin(), prefix.end());

			for(int i=0; i<prefix.length(); i++){
				
				if(is_operand(prefix[i])){
					string operand = "";
					operand.push_back(prefix[i]);
					op_stack.push(operand);
				}
				else if(is_operator(prefix[i])){
					if(op_stack.size() < 2){
						return "error";
					}
					infix += "(";
					string operand_one = op_stack.top();
					op_stack.pop();
					string operand_two = op_stack.top();
					op_stack.pop();
					int op_one_prio, op_two_prio, current_op_prio = get_priority(prefix[i]);
					if(operand_one.length() == 1){
						op_one_prio = 0;
					} else {
						// op_one_prio = get_priority(operand_one.at(operand_one.length() - 3));
						op_one_prio = get_least_priority(operand_one);
					}
					if(operand_two.length() == 1){
						op_two_prio = 0;
					} else {
						// op_two_prio = get_priority(operand_two.at(operand_two.length() - 3));
						op_two_prio = get_least_priority(operand_two);
					}
					if(op_one_prio >= current_op_prio){
						operand_one.erase(0,1);
						operand_one.pop_back();
					}
					if(op_two_prio >= current_op_prio){
						operand_two.erase(0,1);
						operand_two.pop_back();
					}
					infix += operand_one + prefix[i] + operand_two;
					/*
					if(op_one_prio == op_two_prio){
						if(op_one_prio >= current_op_prio){
							operand_one.erase(0,1);
							operand_one.pop_back();
							operand_two.erase(0,1);
							operand_two.pop_back();
							infix += operand_one + prefix[i] + operand_two;
						} else {
							infix += operand_one + prefix[i] + operand_two;
						}
					} else {
						if(op_one_prio >= current_op_prio){
							operand_one.erase(0,1);
							operand_one.pop_back();
						}
						if(op_two_prio >= current_op_prio){
							operand_two.erase(0,1);
							operand_two.pop_back();
						}
						infix += operand_one + prefix[i] + operand_two;
					}
					*/
					infix += ")";
					/*
					infix += "(";
					infix += op_stack.top();
					op_stack.pop();
					infix += prefix[i];
					infix += op_stack.top();
					op_stack.pop();
					infix += ")";
					*/
					op_stack.push(infix);
					infix.clear();
					
				}
			}
			infix = op_stack.top();
			infix.erase(0,1);
			infix.pop_back();
			return infix;

			
		}

		string prefix_to_postfix(){

			if(type != PREFIX){
				return "error";
			}

			string postfix = "";
			string prefix = expression;
			stack<string> op_stack;

			reverse(prefix.begin(), prefix.end());

			for(int i=0; i<prefix.length(); i++){
				if(is_operand(prefix[i])){
					string operand = "";
					operand.push_back(prefix[i]);
					op_stack.push(operand);
				} else if(is_operator(prefix[i])){
					if(op_stack.size() < 2){
						return "error";
					}
					
					postfix += op_stack.top();
					op_stack.pop();
					postfix += op_stack.top();
					op_stack.pop();
					postfix += prefix[i];
					op_stack.push(postfix);
					
					postfix.clear();
					
				}
			}
			
			postfix = op_stack.top();
		

			return postfix;
		}

		string postfix_to_infix(){

			if(type != POSTFIX){
				return "error";
			}

			string postfix = expression;
			string infix = "";
			stack<string> op_stack;

			

			for(int i=0; i<postfix.length(); i++){
				
				if(is_operand(postfix[i])){
					string operand = "";
					operand.push_back(postfix[i]);
					op_stack.push(operand);
				}
				else if(is_operator(postfix[i])){
					if(op_stack.size() < 2){
						return "error";
					}
					infix += "(";
					string operand_one = op_stack.top();
					op_stack.pop();
					string operand_two = op_stack.top();
					op_stack.pop();
					int op_one_prio, op_two_prio, current_op_prio = get_priority(postfix[i]);
					if(operand_one.length() == 1){
						op_one_prio = 0;
					} else {
						// op_one_prio = get_priority(operand_one.at(operand_one.length() - 3));
						op_one_prio = get_least_priority(operand_one);
					}
					if(operand_two.length() == 1){
						op_two_prio = 0;
					} else {
						// op_two_prio = get_priority(operand_two.at(operand_two.length() - 3));
						op_two_prio = get_least_priority(operand_two);
					}
					if(op_one_prio >= current_op_prio){
						operand_one.erase(0,1);
						operand_one.pop_back();
					}
					if(op_two_prio >= current_op_prio){
						operand_two.erase(0,1);
						operand_two.pop_back();
					}
					infix += operand_two + postfix[i] + operand_one;
					/*
					if(op_one_prio == op_two_prio){
						if(op_one_prio >= current_op_prio){
							operand_one.erase(0,1);
							operand_one.pop_back();
							operand_two.erase(0,1);
							operand_two.pop_back();
							infix += operand_one + prefix[i] + operand_two;
						} else {
							infix += operand_one + prefix[i] + operand_two;
						}
					} else {
						if(op_one_prio >= current_op_prio){
							operand_one.erase(0,1);
							operand_one.pop_back();
						}
						if(op_two_prio >= current_op_prio){
							operand_two.erase(0,1);
							operand_two.pop_back();
						}
						infix += operand_one + prefix[i] + operand_two;
					}
					*/
					infix += ")";
					/*
					infix += "(";
					infix += op_stack.top();
					op_stack.pop();
					infix += prefix[i];
					infix += op_stack.top();
					op_stack.pop();
					infix += ")";
					*/
					op_stack.push(infix);
					infix.clear();
					
				}
			}
			infix = op_stack.top();
			infix.erase(0,1);
			infix.pop_back();
			return infix;

		}

		string postfix_to_prefix(){

			if(type != POSTFIX){
				return "error";
			}

			string postfix = expression;
			string prefix = "";
			stack<string> op_stack;

			// reverse(postfix.begin(), postfix.end());

			for(int i=0; i<postfix.length(); i++){
				if(is_operand(postfix[i])){
					string operand = "";
					operand.push_back(postfix[i]);
					op_stack.push(operand);
				} else if(is_operator(postfix[i])){
					if(op_stack.size() < 2){
						return "error";
					}
					prefix += postfix[i];
					string operand_one = op_stack.top();
					op_stack.pop();
					string operand_two = op_stack.top();
					op_stack.pop();
					

					prefix += operand_two + operand_one;
					op_stack.push(prefix);
					
					prefix.clear();
					
				}
			}

			prefix = op_stack.top();

			return prefix;
		}
		
		int get_equivalents(){
			
			cout << "\nGetting Equivalent Form of Expression Input" << endl;
			cout << "-------------------------------------------" << endl;

			cout << "Expression:\t" << expression << endl;
			cout << "Type: ";
			if(type == INFIX){
				cout << "\t\tINFIX" << endl;
				cout << "Prefix:\t\t" << infix_to_prefix() << endl;
				cout << "Postfix:\t" << infix_to_postfix() << endl;
			} else if(type == PREFIX){
				cout << "\t\tPREFIX" << endl;
				cout << "Infix:\t\t" << prefix_to_infix() << endl;
				cout << "Postfix:\t" << prefix_to_postfix() << endl;
			} else if(type == POSTFIX){
				cout << "\t\tPOSTFIX" << endl;
				cout << "Infix:\t\t" << postfix_to_infix() << endl;
				cout << "Prefix:\t\t" << postfix_to_prefix() << endl;
			} else {
				cout << endl;
			}

			return 0;
		}


		

};

class ExpressionsTester{


	private:

	vector<string> infix_expressions;
	vector<string> prefix_expressions;
	vector<string> postfix_expressions;


	public:

	ExpressionsTester(){

		infix_expressions.push_back("A+B*C+D");
		infix_expressions.push_back("(A+B)*(C+D)");
		infix_expressions.push_back("A*B+C*D");
		infix_expressions.push_back("A+B+C+D");
		infix_expressions.push_back("(A+B)*C");
		infix_expressions.push_back("A*B+C/D");
		infix_expressions.push_back("A*(B+C)/D");
		infix_expressions.push_back("a*(b+c/d)");
		infix_expressions.push_back("a*(b+c)");
		infix_expressions.push_back("a/b+c/d");
		infix_expressions.push_back("((a+b)*c)-d");
		infix_expressions.push_back("(a+(((b*c)-((d/(e^f))*g))*h))");

		prefix_expressions.push_back("++A*BCD");
		prefix_expressions.push_back("*+AB+CD");
		prefix_expressions.push_back("+*AB*CD");
		prefix_expressions.push_back("+++ABCD");
		prefix_expressions.push_back("*+ABC");
		prefix_expressions.push_back("+*AB/CD");
		prefix_expressions.push_back("/*A+BCD");
		prefix_expressions.push_back("*a+b/cd");
		prefix_expressions.push_back("*a+bc");
		prefix_expressions.push_back("+/ab/cd");
		prefix_expressions.push_back("-*+abcd");
		prefix_expressions.push_back("+a*-*bc*/d^efgh");

		postfix_expressions.push_back("ABC*+D+");
		postfix_expressions.push_back("AB+CD+*");
		postfix_expressions.push_back("AB*CD*+");
		postfix_expressions.push_back("AB+C+D+");
		postfix_expressions.push_back("AB+C*");
		postfix_expressions.push_back("AB*CD/+");
		postfix_expressions.push_back("ABC+*D/");
		postfix_expressions.push_back("abcd/+*");
		postfix_expressions.push_back("abc+*");
		postfix_expressions.push_back("ab/cd/+");
		postfix_expressions.push_back("ab+c*d-");
		postfix_expressions.push_back("abc*def^/g*-h*+");

	}

	int InfixExpressionTester(){
		
		cout << "Testing Infix Expression Parser" << endl;

		bool show_details = false;
		int expected, result;

		expected = 0;
		for(int i=0; i<infix_expressions.size(); i++){
			InfixExpressionParser infix_tester(infix_expressions.at(i));
			result = infix_tester.parse();
			if(show_details){
				cout << "\n--- POSITIVE TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << infix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}

				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}

		expected = 1;
		for(int i=0; i<prefix_expressions.size(); i++){

			InfixExpressionParser infix_tester(prefix_expressions.at(i));
			result = infix_tester.parse();
			if(show_details){
				cout << "\n--- FIRST NEGATIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << prefix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}
		
		expected = 1;
		for(int i=0; i<postfix_expressions.size(); i++){

			InfixExpressionParser infix_tester(postfix_expressions.at(i));
			result = infix_tester.parse();
			if(show_details){
				cout << "\n--- SECOND NEGATIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << postfix_expressions.at(i) << endl;
				cout << "Expected:\t" << "VALID" << endl;
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}


	return 0;
	}

	int PrefixExpressionTester(){
		
		cout << "Testing Prefix Expression Parser" << endl;

		bool show_details = false;
		int expected, result;

		expected = 1;
		for(int i=0; i<infix_expressions.size(); i++){
			PrefixExpressionParser prefix_tester(infix_expressions.at(i));
			result = prefix_tester.parse();
			if(show_details){
				cout << "\n--- FIRST NEGATIVE TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << infix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}

				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}

		expected = 0;
		for(int i=0; i<prefix_expressions.size(); i++){

			PrefixExpressionParser prefix_tester(prefix_expressions.at(i));
			result = prefix_tester.parse();
			if(show_details){
				cout << "\n--- POSTIIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << prefix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}
		
		expected = 1;
		for(int i=0; i<postfix_expressions.size(); i++){

			PrefixExpressionParser prefix_tester(postfix_expressions.at(i));
			result = prefix_tester.parse();
			if(show_details){
				cout << "\n--- SECOND NEGATIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << postfix_expressions.at(i) << endl;
				cout << "Expected:\t" << "VALID" << endl;
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}


	return 0;
	}

	int PostfixExpressionTester(){
		
		cout << "Testing Postfix Expression Parser" << endl;

		bool show_details = false;
		int expected, result;

		expected = 1;
		for(int i=0; i<infix_expressions.size(); i++){
			PostfixExpressionParser postfix_tester(infix_expressions.at(i));
			result = postfix_tester.parse();
			if(show_details){
				cout << "\n--- FIRST NEGATIVE TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << infix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}

				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}

		expected = 1;
		for(int i=0; i<prefix_expressions.size(); i++){

			PostfixExpressionParser postfix_tester(prefix_expressions.at(i));
			result = postfix_tester.parse();
			if(show_details){
				cout << "\n--- SECOND NEGATIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << prefix_expressions.at(i) << endl;
				cout << "Expected:\t" ;
				if(expected == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}
		
		expected = 0;
		for(int i=0; i<postfix_expressions.size(); i++){

			PostfixExpressionParser postfix_tester(postfix_expressions.at(i));
			result = postfix_tester.parse();
			if(show_details){
				cout << "\n--- POSITIVE TEST NO " << i << " ---" << endl;
				cout << "Expression:\t" << postfix_expressions.at(i) << endl;
				cout << "Expected:\t" << "VALID" << endl;
				cout << "Actual:\t";
				if(result == 0){
					cout << "VALID" << endl;
				} else{
					cout << "INVALID" << endl;
				}
			}
			cout << "Result:\t";

			if(expected == result){
				cout << "PASSED" << endl;
			} else{
				cout << "FAILED" << endl;
			}

		}


	return 0;
	}

	int infixToPrefixTester(){

		cout << "Testing Infix to Prefix Conversions" << endl;
		bool show_details = true;
		string result = "", expected = "";


		for(int i=0; i<infix_expressions.size(); i++){
			
			Expression test_expr(infix_expressions.at(i));
			result = test_expr.infix_to_prefix();
			expected = prefix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << infix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}

	int infixToPostfixTester(){

		cout << "Testing Infix to Postfix Conversions" << endl;
		bool show_details = true;
		string result = "", expected = "";


		for(int i=0; i<infix_expressions.size(); i++){
			
			Expression test_expr(infix_expressions.at(i));
			result = test_expr.infix_to_postfix();
			expected = postfix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << infix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}

	int prefixToInfixTester(){

		//IMPORTANT NOTE
		//Test no 10 and 11 are supposed to passed since they are the most simplified version of the given expressions

		cout << "Testing Prefix to Infix Conversions" << endl;
		cout << "\nNote: Test no 10 and 11 are supposed to passed since they are the most simplified version of the given expressions." << endl;
		bool show_details = false;
		string result = "", expected = "";


		for(int i=0; i<prefix_expressions.size(); i++){
			
			Expression test_expr(prefix_expressions.at(i));
			result = test_expr.prefix_to_infix();
			expected = infix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << prefix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}

	int prefixToPostfixTester(){


		cout << "Testing Prefix to Postfix Conversions" << endl;

		bool show_details = false;
		string result = "", expected = "";


		for(int i=0; i<prefix_expressions.size(); i++){
			
			Expression test_expr(prefix_expressions.at(i));
			result = test_expr.prefix_to_postfix();
			expected = postfix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << prefix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}

	int postfixToInfixTester(){

		//IMPORTANT NOTE
		//Test no 10 and 11 are supposed to passed since they are the most simplified version of the given expressions

		cout << "Testing Postfix to Infix Conversions" << endl;
		cout << "\nNote: Test no 10 and 11 are supposed to passed since they are the most simplified version of the given expressions." << endl;
		bool show_details = false;
		string result = "", expected = "";


		for(int i=0; i<postfix_expressions.size(); i++){
			
			Expression test_expr(postfix_expressions.at(i));
			result = test_expr.postfix_to_infix();
			expected = infix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << postfix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}
 
	int postfixToPrefixTester(){


		cout << "Testing Postfix to Prefix Conversions" << endl;

		bool show_details = false;
		string result = "", expected = "";


		for(int i=0; i<prefix_expressions.size(); i++){
			
			Expression test_expr(postfix_expressions.at(i));
			result = test_expr.postfix_to_prefix();
			expected = prefix_expressions.at(i);
			if(show_details){
				cout << "\n--- TEST NO " << i << "---" << endl;
				cout << "Expression:\t" << postfix_expressions.at(i) << endl;
				cout << "Expected:\t" << expected << endl;
				cout << "Actual:\t\t" << result << endl;
			}

			cout << "Result:\t";

			if(expected == result){
					cout << "PASSED" << endl;
			} else {
				cout << "FAILED" << endl;
			}

		}
		
		return 0;
	}

	int test_parser_and_converter(){

		InfixExpressionTester();
		PrefixExpressionTester();
		PostfixExpressionTester();

		infixToPrefixTester();
		infixToPostfixTester();
		prefixToInfixTester();
		prefixToPostfixTester();
		postfixToInfixTester();
		postfixToPrefixTester();

		return 0;
	}

	int get_equivalent_tester(){

		for(int i=0; i<4; i++){
			Expression test(infix_expressions.at(i));
			test.get_equivalents();
		}

		for(int i=4; i<8; i++){
			Expression test(prefix_expressions.at(i));
			test.get_equivalents();
		}

		for(int i=8; i<12; i++){
			Expression test(postfix_expressions.at(i));
			test.get_equivalents();
		}
	
		return 0;
	}

};


int main(void){

	ExpressionsTester tester;


	tester.get_equivalent_tester();



	// Uncomment next comment block for custom input expression
/*	

	string input;
	cout << "Input:";
	getline(cin, input);

	Expression test(input);
	test.get_equivalents();

*/
	

	return 0;


}


