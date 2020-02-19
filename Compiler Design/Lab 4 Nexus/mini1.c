#include"file_helper.h"

/*
	
	S -> a | > | (T)
	T -> ST_dash
	T_dash -> ,ST_dash | ε 

*/

FILE *fp;

//keeps track of number of brackets
int brackets = 0;

//function signature
void S();

void T_dash(){

	char c = read_next();
	if(c == ','){

		//according to grammar, if ',' is encountered then ST' follow
		S();
		T_dash();
		return;
	}
	else if(c == ')'){
		//revert so that function S can handle ')' by itself.
		revert();
		return;
	}
	else{
		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "After an 'a' or '<' you can only enter ',' or ')'.\n");
		print_error(error, hint);
		return;
	}
}

void T(){

	//T is only meant to call ST' as T -> ST'
	S();
	T_dash();
	return;
}

void S(){

	char c = read_next();
	if(c == 'a'){
		
		if(is_first_char(fp)){

			//If the input begins with a, then only a$ is acceptable.
			char temp = read_next();
			if(temp == '$'){
				
				printf("S: Successfully Read.\n");
				return;
			}
			else{

				char error[100], hint[100];
				sprintf(error, "Error reading character %c at position %d.\n\n", temp, current_position(fp));
				sprintf(hint, "Only '$' can be entered after 'a' if string begins with 'a'.\n");
				print_error(error, hint);
				return;
			}
		}
		else
			return;	
	}
	else if(c == '>'){

		if(is_first_char(fp)){

			char temp = read_next();
			if(temp == '$'){

				printf("S: Successfully Read.\n");
				return;
			}
			else{

				char error[100], hint[100];
				sprintf(error, "Error reading character %c at position %d.\n\n", temp, current_position(fp));
				sprintf(hint, "Only '$' can be entered after '>' if string begins with '>'.\n");
				print_error(error, hint);
				return;
			}
		}
		else
			return;
	}
	else if(c == '('){

		brackets++;

		T();
		char c = read_next();
		if(c == ')'){

			brackets--;
			if(brackets == 0){
				char temp = read_next();
				if(temp == '$'){

					printf("S: Successfully Read.\n");
					return;
				}
				else{


					char error[100], hint[100];
					sprintf(error, "Error reading character %c at position %d.\n\n", temp, current_position(fp));
					sprintf(hint, "Only '$' can be entered after ')' if all '('s and ')'s are balanced.\n");
					print_error(error, hint);
					return;
				}
			}
			else
				return;
		}
	}
	else{
		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "You can start with 'a', '<' or '(' only after a '(' or start of string.\n");
		print_error(error, hint);
		return;
	}
}

int main(){

	fp = open_file("./samples/mini1.txt");
	S();
}