#include "file_helper.h"

/*
	
	E -> TE_dash
	E_dash -> +TE_dash | ε
	T -> FT_dash
	T_dash -> *FT_dash | ε
	F -> (E) | i

*/

void E();

void F(){

	char c = read_next();
	if(c == '('){

		E();

		if(c == ')'){
			return;
		}
	}
	else if(c == 'i'){
		return;
	}
}

void T_dash(){

	char c = read_next();	
}