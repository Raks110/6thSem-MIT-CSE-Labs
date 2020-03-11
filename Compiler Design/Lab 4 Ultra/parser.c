#include "lexical_helper.h"
#include "symbol_table.h"

FILE *fp;
NODE *table = NULL;

int lines = 0, columns = 1;
int row = -1, col = -1;

int revert_token = 0;
TOKEN token_past;

TOKEN get_next(){
	TOKEN t;
	if(!revert_token){
		t = get_stream(&row, &col, &lines, &columns, fp, &table);
		token_past = t;
	}
	else{
		t = token_past;
		revert_token = 0;
	}
	return t;
}

void declerations();
void statement_list();
void expn();
void decision_stat();
void looping_stat();
void il_prime();
void simple_expn();
void eprime();
void seprime();
void term();
void tprime();
void dprime();

void program(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, "main") == 0){

		t = get_next();
		if(strcmp(t.lexeme, "(") == 0){

			t = get_next();
			if(strcmp(t.lexeme, ")") == 0){

				t = get_next();
				if(strcmp(t.lexeme, "{") == 0){

					declerations();
					statement_list();

					t = get_next();
					if(strcmp(t.lexeme, "}") == 0){

						//Success
						return;
					}
					else{
						printf("Error. Expected '}' at %d:%d.\n", t.row, t.column);
						exit(1);
					}
				}
				else{

					printf("Error. Expected '{' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{

				printf("Error. Expected ')' at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{

			printf("Error. Expected '(' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else{
		printf("Error. Expected 'main' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void data_type(){


	TOKEN t = get_next();

	if(strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char") == 0){
		//Success
		return;
	}
	else{

		printf("Error. Expected some data type (int | char) at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void relop(){
	TOKEN t = get_next();
	if(t.type == 2){
		//Success
		return;
	}
	else{
		//Error

		printf("Error. Expected a relational operator at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void addop(){

	TOKEN t = get_next();
	if((strcmp(t.lexeme, "+") == 0) || (strcmp(t.lexeme, "-") == 0)){
		//Success
		return;
	}
	else{
		//Error
		printf("Error. Expected an addition operator (+ | -) at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void mulop(){
	TOKEN t = get_next();
	if((strcmp(t.lexeme, "*") == 0) || (strcmp(t.lexeme, "/") == 0) || (strcmp(t.lexeme, "%") == 0)){
		//Success
		return;
	}
	else{
		//Error
		printf("Error. Expected a multiplication operator (* | / | %) at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void factor(){
	TOKEN t = get_next();
	if(t.type == 1 || t.type == 6){
		//Success
		return;
	}
	else{
		//Error
		printf("Error. Expected an identifier or a number at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void identifier_list(){

		TOKEN t = get_next();

		if(t.type == 1){

			//read identifier
			il_prime();
		}
		else{

			printf("Error. Expected identifier at %d:%d.\n", t.row, t.column);
			exit(1);
		}
}

void declerations(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char") == 0){

		revert_token = 1;

		data_type();
		identifier_list();

		t = get_next();

		if(strcmp(t.lexeme, ";") == 0){

			declerations();
			return;
		}
		else{
			//Error
			printf("Error. Expected ';' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else if(t.type == 1 || strcmp(t.lexeme, "if") == 0 || strcmp(t.lexeme, "while") == 0 || strcmp(t.lexeme, "for") == 0 || strcmp(t.lexeme, "}") == 0){

		revert_token = 1;
		return;
	}
	else{
		//Error
		printf("Error. Expected data type, some control statement or '}' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void il_prime(){

		TOKEN t = get_next();

		if(strcmp(t.lexeme, ";") == 0){
			revert_token = 1;
			return;
		}
		else if(strcmp(t.lexeme, ",") == 0){

			identifier_list();
			return;
		}
		else if(strcmp(t.lexeme, "[") == 0){
			t = get_stream(&row, &col, &lines, &columns, fp, &table);
			if(t.type == 6){
				t = get_stream(&row, &col, &lines, &columns, fp, &table);
				if(strcmp(t.lexeme, "]") == 0){
					t = get_stream(&row, &col, &lines, &columns, fp, &table);
					if(strcmp(t.lexeme, ",")){
						identifier_list();
					}
					else{
						//Error
						printf("Error. Expected ',' at %d:%d.\n", t.row, t.column);
						exit(1);
					}
				}
				else{
					//Error
					printf("Error. Expected ']' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{
				//Error
				printf("Error. Expected a number at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{
			//Error
			printf("Error. Expected '[', ',' or ';' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
}

void assign_stat(){

	TOKEN t = get_next();
	if(t.type == 1){

		TOKEN t = get_next();
		if(strcmp(t.lexeme, "=") == 0){
			expn();
			return;
		}
		else{
			//Error
			printf("Error. Expected '=' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else{
		printf("Error. Expected an identifier at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void statement(){

	TOKEN t = get_next();

	if(t.type == 1){
		revert_token = 1;
		assign_stat();

		t = get_next();
		if(strcmp(t.lexeme, ";") == 0){
			return;
		}
	}
	else if(strcmp(t.lexeme, "if") == 0){
		revert_token = 1;
		decision_stat();
	}
	else if((strcmp(t.lexeme, "for") == 0) || (strcmp(t.lexeme, "while") == 0)){
		revert_token = 1;
		looping_stat();
	}
	else{

		printf("Error. Expected an identifier or some control statement at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void statement_list(){
	TOKEN t = get_next();
	if(strcmp(t.lexeme, "}") == 0){
		revert_token = 1;
		return;
	}
	else{
		revert_token = 1;
		statement();
		statement_list();
	}
}

void expn(){
	simple_expn();
	eprime();
	return;
}

void eprime(){
	TOKEN t = get_next();
	if((strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0)){
		revert_token = 1;
		return;
	}
	else{
		revert_token = 1;
		relop();
		simple_expn();
		return;
	}
}

void simple_expn(){
	term();
	seprime();
}

void seprime(){
	TOKEN t = get_next();
	if((strcmp(t.lexeme, "=") == 0) || (strcmp(t.lexeme, "==") == 0) || (strcmp(t.lexeme, "!") == 0) || (strcmp(t.lexeme, ">") == 0) || (strcmp(t.lexeme, "<") == 0) || (strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0)){
		revert_token = 1;
		return;
	}
	else{
		revert_token = 1;
		addop();
		term();
		seprime();
		return;
	}
}

void term(){

	factor();
	tprime();
}

void tprime(){

	TOKEN t = get_next();
	if((strcmp(t.lexeme, "=") == 0) || (strcmp(t.lexeme, "==") == 0) || (strcmp(t.lexeme, "!") == 0) || (strcmp(t.lexeme, ">") == 0) || (strcmp(t.lexeme, "<") == 0) || (strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0) || (strcmp(t.lexeme, "+") == 0) || (strcmp(t.lexeme, "-") == 0)){
		revert_token = 1;
		return;
	}
	else{
		revert_token = 1;
		mulop();
		factor();
		tprime();
	}
}

void decision_stat(){
	TOKEN t = get_next();
	if((strcmp(t.lexeme, "if") == 0)){

		t = get_next();
		if((strcmp(t.lexeme, "(") == 0)){

			expn();
			t = get_next();
			if((strcmp(t.lexeme, ")") == 0)){

				t = get_next();
				if((strcmp(t.lexeme, "{") == 0)){
					statement_list();

					t = get_next();
					if((strcmp(t.lexeme, "}") == 0)){
						dprime();
						return;
					}
					else{
						//Error
						printf("Error. Expected '}' at %d:%d.\n", t.row, t.column);
						exit(1);
					}
				}
				else{
					//Error
					printf("Error. Expected '{' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{
				//Error
				printf("Error. Expected ')' at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{
			//Error
			printf("Error. Expected '(' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else{
		//Error
		printf("Error. Expected 'if' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void dprime(){

	TOKEN t = get_next();
	if(t.type == 1 || (strcmp(t.lexeme, "if") == 0) || (strcmp(t.lexeme, "while") == 0) || (strcmp(t.lexeme, "for") == 0) || (strcmp(t.lexeme, "}") == 0)){
		revert_token = 1;
		return;
	}
	else{

		if(strcmp(t.lexeme, "else") == 0){

			t = get_next();
			if(strcmp(t.lexeme, "{") == 0){

				statement_list();
				t = get_next();

				if(strcmp(t.lexeme, "}") == 0){
					//Success
					return;
				}
				else{
					//Error
					printf("Error. Expected '}' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{
				//Error
				printf("Error. Expected '{' at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{
			//Error
			printf("Error. Expected 'else' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
}

void looping_stat(){

	TOKEN t = get_next();
	if(strcmp(t.lexeme, "while") == 0){

		t = get_next();
		if((strcmp(t.lexeme, "(") == 0)){

			expn();
			t = get_next();
			if((strcmp(t.lexeme, ")") == 0)){

				t = get_next();
				if((strcmp(t.lexeme, "{") == 0)){
					statement_list();

					t = get_next();
					if((strcmp(t.lexeme, "}") == 0)){
						return;
					}
					else{
						//Error
						printf("Error. Expected '}' at %d:%d.\n", t.row, t.column);
						exit(1);
					}
				}
				else{
					//Error
					printf("Error. Expected '{' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{
					//Error
					printf("Error. Expected ')' at %d:%d.\n", t.row, t.column);
					exit(1);
			}
		}
		else{

			printf("Error. Expected '(' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else if(strcmp(t.lexeme, "for") == 0){

		t = get_next();
		if(strcmp(t.lexeme, "(") == 0){

			assign_stat();
			t = get_next();

			if(strcmp(t.lexeme, ";") == 0){

				expn();
				t = get_next();

				if(strcmp(t.lexeme, ";") == 0){

					assign_stat();
					t = get_next();

					if(strcmp(t.lexeme, ")") == 0){

						t = get_next();
						if(strcmp(t.lexeme, "{") == 0){

							statement_list();
							t = get_next();

							if(strcmp(t.lexeme, "}") == 0){

								//Success
								return;
							}
							else{
								//Error
								printf("Error. Expected '}' at %d:%d.\n", t.row, t.column);
								exit(1);
							}
						}
						else{
							//Error
							printf("Error. Expected '{' at %d:%d.\n", t.row, t.column);
							exit(1);
						}
					}
					else{
						//Error
						printf("Error. Expected ')' at %d:%d.\n", t.row, t.column);
						exit(1);
					}
				}
				else{
					//Error
					printf("Error. Expected ';' at %d:%d.\n", t.row, t.column);
					exit(1);
				}
			}
			else{
				//Error
				printf("Error. Expected ';' at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{
			//Error
			printf("Error. Expected '(' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
}

int main(){

	removeComments();
	removePre();

	fp = fopen("./samples/sample_test2.txt","r");

	/*char c = fgetc(fp);

  while(c != EOF){

  	revert(fp);

		TOKEN t = get_stream(&row, &col, &lines, &columns, fp, &table);
		print_token_to_screen(t);

		c = getc(stdin);

    c = fgetc(fp);
	}*/

	program();
	char c = fgetc(fp);
	if(c != '$'){
		printf("Expected EOF ($) after '}' of Main function.\n");
		exit(1);
	}
	else{
		printf("Successfully parsed the C++ program.\n");
	}
}
