#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

enum _tokenType{
	word, operator, decimal_integer, octal_integer, hexadecimal_integer, float_integer 
};

//list of all operators required
enum _operatorType{
	left_parenthesis, right_parenthesis, left_bracket, right_bracket, structure_member, structure_pointer, comma, negate, ones_complement, shift_right, shift_left, bitwise_XOR, bitwise_OR, increment, decrement, addition, division, logical_OR, logical_AND, conditional_true, conditional_false, equality, inequality, less_than_test, greater_than_test, less_than_or_equal_test, greater_than_or_equal_test, assignment, plus_equals, minus_equals, times_equals, divide_equals, mod_equals, shift_right_equals, shift_left_equals, bitwise_AND_equals, bitwise_XOR_equals, bitwise_OR_equals, AND_address_operator, minus_subtract_operator, multiply_dereference_operator
};
typedef enum _tokenType tokenType;
typedef enum _operatorType operatorType;

//token struct for easier access to variables with pointers
struct _token{
	tokenType tokenType;
	operatorType operType;
	int beginIdx;
	int endIdx;
};
typedef struct _token token;

//Determines the number of characters for a float token
void determineFloat(token* token, char* input){
	int hitPeriod = 0;
	char* character = input + (*token).beginIdx;
	while(isdigit(*character) || *character == '.'){
		if(hitPeriod && *character == '.'){
			break;
		}else if(*character == '.'){
			++hitPeriod;
		}
		++(*token).endIdx;
		++character;
	}
	char next = *(character + 1);
	if((*character == 'E' || *character == 'e') && (next == '+' || next == '-' || isdigit(next))){
		++(*token).endIdx;
		++character;

		if(*character == '+' || *character == '-'){
			++(*token).endIdx;
			++character;
		}

		while(isdigit(*character)){
			++(*token).endIdx;
			++character;
		}
	}
}

//Finds out how many characters a hexadecimal token has
void determineHexadecimalInt(token* token, char* input){
	char* character = input + (*token).beginIdx + 2;
	(*token).endIdx += 2;
}

//Finds out how many characters a decimal token is
void determineDecimalToken(token* token, char* input){
	char* character = input + (*token).beginIdx;
	while(isdigit(*character)){
		++(*token).endIdx;
		++character;
	}
	if(*character == '.' && isdigit(*(character + 1))){
		(*token).endIdx = token->beginIdx;
		(*token).tokenType = float_integer;
		determineFloat(token, input);
	}
}

//Finds out how many characters an octal token is
void determineOctalToken(token* token, char* input){
	char* character = input + (*token).beginIdx;
	while(isdigit(*character)){
		++(*token).endIdx;
		++character;
	}
	if(*character == '.'){
		(*token).endIdx = (*token).beginIdx;
		(*token).tokenType = float_integer;
		determineFloat(token, input);
	}
}

//Finding size and type of operator
void determineOperator(token* token, char* input){
	char* character = input + (*token).beginIdx;
	char next = input[(*token).beginIdx + 1];
	char second = input[(*token).beginIdx + 2];
	++(*token).endIdx;
	if(*character == '>'){
		if(next == '>'){
			++(*token).endIdx;
			if(second == '='){
				(*token).operType = shift_right_equals;
				++(*token).endIdx;
			}else{
				(*token).operType = shift_right;
			}
		}else if (next == '='){
			(*token).operType = greater_than_or_equal_test;
			++(*token).endIdx;
		}else{
			(*token).operType = greater_than_test;
		}
	}else if(*character == '<'){
		if(next == '<'){
			++(*token).endIdx;
			if(second == '='){
				(*token).operType = shift_left_equals;
				++(*token).endIdx;
			}else{
				(*token).operType = shift_left;
			}
		}else if(next == '='){
			++(*token).endIdx;
			(*token).operType = less_than_or_equal_test;
		}else{
			(*token).operType = less_than_test;
		}
	}else if(*character == '+'){
		if (next == '+'){
			(*token).operType = increment;
			++(*token).endIdx;
		}else if(next == '='){
			(*token).operType = plus_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = addition;
		}
	}else if(*character == '-'){
		if(next == '-'){
			(*token).operType = decrement;
			++(*token).endIdx;	
		}else if(next == '>'){
			(*token).operType = structure_pointer;
			++(*token).endIdx;
		}else if(next == '='){
			(*token).operType = minus_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = minus_subtract_operator;
		}
	}else if(*character == '!'){
		if(next == '='){
			(*token).operType = inequality;
			++(*token).endIdx;
		}else{
			(*token).operType = negate;
		}
	}else if(*character == '^'){
		if(next == '='){
			(*token).operType = bitwise_XOR_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = bitwise_XOR;
		}
	}else if(*character == '|'){
		if(next == '|'){
			(*token).operType = logical_OR;
			++(*token).endIdx;
		}else if(next == '='){
			(*token).operType = bitwise_OR_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = bitwise_OR;
		}
	}else if(*character == '/'){
		if(next == '='){
			(*token).operType = divide_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = division;
		}
	}else if(*character == '&'){
		if(next == '&'){
			(*token).operType = logical_AND;
			++(*token).endIdx;
		}else if(next == '='){
			(*token).operType = bitwise_AND_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = AND_address_operator;
		}
	}else if(*character == '='){
		if(next == '='){
			(*token).operType = equality;
			++(*token).endIdx;
		}else{
			(*token).operType = assignment;
		}
	}else if(*character == '*'){
		if(next == '='){
			(*token).operType = times_equals;
			++(*token).endIdx;
		}else{
			(*token).operType = multiply_dereference_operator;
		}
	}else if(*character == '%'){
		if(next == '='){
			(*token).operType = mod_equals;
			++(*token).endIdx;
		}
	}else if(*character == '.'){
		(*token).operType = structure_member;
	}else if(*character == ','){
		(*token).operType = comma;
	}else if(*character == '('){
		(*token).operType = left_parenthesis;
	}else if (*character == ')'){
		(*token).operType = right_parenthesis;
	}else if (*character == '['){
		(*token).operType = left_bracket;
	}else if (*character == ']'){
		(*token).operType = right_bracket;
	}else if(*character == '~'){
		(*token).operType = ones_complement;
	}else if(*character == '?'){
		(*token).operType = conditional_true;
	}else if(*character == ':'){
		(*token).operType = conditional_false;
	}
}

//Finds out how many characters a word token is 
void determineWord(token* token, char* input){
	char* character = input + (*token).beginIdx;
	while(isalnum(*character)){
		++(*token).endIdx;
		++character;
	}	
}

//Determine the token type and sets it
void determineInitialCharacter(token* token, char* input) {
	char character = input[(*token).beginIdx];
	char next = input[(*token).beginIdx + 1];
	if(isalpha(character)){
		(*token).tokenType = word;
		return;
	}else if(isdigit(character) && character != '0'){
		(*token).tokenType = decimal_integer;
		return;
	}else if(character == '0' && next != 'X' && next != 'x'){
		(*token).tokenType = octal_integer;
		return;
	}else if(character == '0' && (next == 'X' || next == 'x')){
		(*token).tokenType = hexadecimal_integer;
		return;
	}else{
		(*token).tokenType = operator;
		return;
	}
}

//Takes the given operator type name and returns it in string form
char* getOperatorString(token* token){
	switch((*token).operType){
		case left_parenthesis:
			return "left parenthesis";	
			break;
		case right_parenthesis:
			return "right parenthesis";
			break;
		case left_bracket:
			return "left bracket";
			break;
		case right_bracket:
			return "right bracket";
			break;
		case structure_member:
			return "structure member";
			break;
		case structure_pointer:
			return "structure pointer";
			break;
		case comma:
			return "comma";
			break;
		case negate:
			return "negate";
			break;
		case ones_complement:
			return "1s complement";
			break;
		case shift_right:
			return "shift right";
			break;
		case shift_left:
			return "shift left";
			break;
		case bitwise_XOR:
			return "bitwise XOR";
			break;
		case bitwise_OR:
			return "bitwise OR";
			break;
		case increment:
			return "increment";
			break;
		case decrement:
			return "decrement";
			break;
		case addition:
			return "addition";
			break;
		case division:
			return "division";
			break;
		case logical_OR:
			return "logical OR";
			break;
		case logical_AND:
			return "logical AND";
			break;
		case conditional_true:
			return "conditional true";
			break;
		case conditional_false:
			return "conditional false";
			break;
		case equality:
			return "equality";
			break;
		case inequality:
			return "inequality";
			break;
		case less_than_test:
			return "less than test";
			break;
		case greater_than_test:
			return "greater than test";
			break;
		case less_than_or_equal_test:
			return "less than or equal test";
			break;
		case greater_than_or_equal_test:
			return "greater than or equal test";
			break;
		case assignment:
			return "assignment";
			break;
		case plus_equals:
			return "plus equals";
			break;
		case minus_equals:
			return "minus equals";
			break;
		case times_equals:
			return "times equals";
			break;
		case divide_equals:
			return "divide equals";
			break;
		case mod_equals:
			return "mod equals";
			break;
		case shift_right_equals:
			return "shift right equals";
			break;
		case shift_left_equals:
			return "shift left equals";
			break;
		case bitwise_AND_equals:
			return "bitwise AND equals";
			break;
		case bitwise_XOR_equals:
			return "bitwise XOR equals";
			break;
		case bitwise_OR_equals:
			return "bitwise OR equals";
			break;
		case AND_address_operator:
			return "AND/address operator";
			break;
		case minus_subtract_operator:
			return "minus/subtract operator";
			break;
		case multiply_dereference_operator:
			return "multiply/dereference operator";
			break;
		default:
			return "";
	}
}

//Takes the given token type name and returns it in string form
char* getTypeString(token* token){
	switch((*token).tokenType){
		case word:
			return "word";
			break;
		case decimal_integer:
			return "decimal integer";
			break;
		case octal_integer:
			return "octal integer";
			break;
		case hexadecimal_integer:
			return "hexadecimal integer";
			break;
		case float_integer:
			return "floating point";
			break;
		case operator:
			return getOperatorString(token);
			break;
		default: "";
	}
}

//Main function to print tokens
int main(int argc, char *argv[]){
	int i;
	for(i = 1; i < argc; ++i){
		char *input = argv[i];
		int j = 0;
		token* currentToken = (token*)malloc(sizeof(token));
		while(input[j] != '\0'){
			if(isspace(input[j])){
				++j;
			}
			(*currentToken).beginIdx = j;
			(*currentToken).endIdx = j;
			determineInitialCharacter(currentToken, input);
			switch((*currentToken).tokenType){
				case word:
					determineWord(currentToken, input);
					break;
				case operator:
					determineOperator(currentToken, input);
					break;
				case decimal_integer:
					determineDecimalToken(currentToken, input);
					break;
				case octal_integer:
					determineDecimalToken(currentToken, input);
					break;
				case hexadecimal_integer:
					determineHexadecimalInt(currentToken, input);
					break;
			}
			int tokenLength = (*currentToken).endIdx - (*currentToken).beginIdx;
			char* str = (char*)malloc(sizeof(char)*tokenLength + 1);
			memcpy(str, &input[(*currentToken).beginIdx], tokenLength);
			char* type_str = getTypeString(currentToken);
			printf("%s: \"%s\"\n", type_str, str);
			j = (*currentToken).endIdx;
		}
	}
}
