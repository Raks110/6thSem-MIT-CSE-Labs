#include"file_helper.h"
#include<ctype.h>

/*

	E -> TE_dash
	E_dash -> +TE_dash | ε
	T -> FT_dash
	T_dash -> *FT_dash | ε
	F -> (E) | id

*/

FILE *fp;

typedef struct stat{

	int success;
	int failure;
	int done;

}STATUS;

STATUS status;

void init_status(){

	status.success = 0;
	status.failure = 0;
}

void success(){

	status.success = 1;
}

void failure(){

	status.failure = 1;
}

void found_dollar(){
	status.done = 1;
}

void E();

void F(){

	char c = read_next();
	if(c == '('){

		E();

		c = read_next();
		if(c == ')'){

			success();
			return;
		}
		else{

			failure();
			char error[100], hint[100];
			sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
			sprintf(hint, "Ensure your paranthesis are balanced.\n");
			print_error(error, hint);
			return;
		}
	}
	else if(isalpha(c)){

		success();
		return;
	}
	else{

		failure();
		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "You need to use either ( or an alphabet at this point.\n");
		print_error(error, hint);
		return;
	}
}

void T_dash(){

	char c = read_next();
	if(c == '*'){
		
		F();
		T_dash();
	}
	else if(c == '+' || c == '$' || c == ')'){
		revert();
		return;
	}
	else{

		failure();
		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "You need to use either *, +, $ or ) at this point.\n");
		print_error(error, hint);
		return;
	}
}

void T(){

	F();
	T_dash();
}

void E_dash(){

	char c = read_next();
	if(c == '+'){
		
		T();
		E_dash();
	}
	else if(c == '$' || c == ')'){
		
		revert();
		return;
	}
	else{

		failure();
		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "You need to use either +, $ or ) at this point.\n");
		print_error(error, hint);
		return;
	}
}

void E(){

	T();
	E_dash();
}

int main(){

	fp = open_file("./samples/mini2.txt");
	E();

	revert();

	char c = read_next();
	c = read_next(); // as revert is called. So we go two steps forward.

	if(c != '$'){
		failure();

		char error[100], hint[100];
		sprintf(error, "Error reading character %c at position %d.\n\n", c, current_position(fp));
		sprintf(hint, "File did not end correctly. Ensure '$' is used at the right place.\n");
		print_error(error, hint);
	}

	if(!status.failure){

		printf("Successfully Parsed.\n");
	}
	else{

		printf("\nFile not parsed.\n");
	}
}