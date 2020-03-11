#include "lexical_helper.h"
#include "symbol_table.h"

//Function declarations
void declerations();
void statement_list();
void expn();
void decision_stat();
void looping_stat();
void il_prime();
void il_prime_prime();
void simple_expn();
void eprime();
void seprime();
void term();
void tprime();
void dprime();

//Global variables
FILE *fp;
NODE *table = NULL;

int lines = 0, columns = 1;
int row = -1, col = -1;

int revert_token = 0;
TOKEN token_past;

//Function to handle reverting and normal TOKEN stream reading
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

//Implementation of RD Parser
void program(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, "main") == 0){

		//main accepted
		t = get_next();
		if(strcmp(t.lexeme, "(") == 0){

			//'(' accepted
			t = get_next();
			if(strcmp(t.lexeme, ")") == 0){

				//')' accepted
				t = get_next();
				if(strcmp(t.lexeme, "{") == 0){

					//'{' accepted
					declerations();
					statement_list();

					t = get_next();
					if(strcmp(t.lexeme, "}") == 0){

						//Program should ideally end here
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

	if(strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char") == 0 || strcmp(t.lexeme, "float") == 0){
		//Accepted some data type
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
		//Type 2 corresponds to RELOP
		return;
	}
	else{

		printf("Error. Expected a relational operator at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void addop(){

	TOKEN t = get_next();

	if((strcmp(t.lexeme, "+") == 0) || (strcmp(t.lexeme, "-") == 0)){
		//Accepted + or -
		return;
	}
	else{

		printf("Error. Expected an addition operator (+ | -) at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void mulop(){

	TOKEN t = get_next();

	if((strcmp(t.lexeme, "*") == 0) || (strcmp(t.lexeme, "/") == 0) || (strcmp(t.lexeme, "%") == 0)){
		//Accepted *, /, or %
		return;
	}
	else{

		printf("Error. Expected a multiplication operator (* | / | %) at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void factor(){

	TOKEN t = get_next();

	if(t.type == 1 || t.type == 6){
		//Type 1 cprresponds with identifiers and Type 6 corresponds to Numbers
		//Accept either Type 1 or Type 6 tokens
		return;
	}
	else{

		printf("Error. Expected an identifier or a number at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void identifier_list(){

		TOKEN t = get_next();

		if(t.type == 1){

			//Accepted an identifier (Type 1 token)
			il_prime();
		}
		else{

			printf("Error. Expected identifier at %d:%d.\n", t.row, t.column);
			exit(1);
		}
}

void declerations(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char") == 0 || strcmp(t.lexeme, "float")){

		//Accepted a data type. Now we need to let the function data_type handle this.
		//Hence set revert_token flag and allow data_type function to re-read the previous token
		revert_token = 1;

		data_type();
		identifier_list();

		t = get_next();

		if(strcmp(t.lexeme, ";") == 0){

			//Accept a ';'
			declerations();
			return;
		}
		else{

			printf("Error. Expected ';' at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else if(t.type == 1 || strcmp(t.lexeme, "if") == 0 || strcmp(t.lexeme, "while") == 0 || strcmp(t.lexeme, "for") == 0 || strcmp(t.lexeme, "}") == 0){

		//Accept any element in the follow set of declerations.
		//We need to set revert_token flag as we just read a token that's in the follow of declerations.
		//We want some other production to handle this token
		revert_token = 1;
		return;
	}
	else{

		printf("Error. Expected data type, some control statement or '}' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void il_prime(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, ";") == 0){

		//Accepted ';' which is in follow set of il_prime
		//Revert and allow some other production to handle this
		revert_token = 1;
		return;
	}
	else if(strcmp(t.lexeme, ",") == 0){

		//Accepted ','
		identifier_list();
		return;
	}
	else if(strcmp(t.lexeme, "[") == 0){

		//Accepted '['
		t = get_next();

		if(t.type == 6){

			//Accepted some number [Token of type 6]
			t = get_next();

			if(strcmp(t.lexeme, "]") == 0){
					
				//Accepted ']'
				il_prime_prime();
			}
			else{

				printf("Error. Expected ']' at %d:%d.\n", t.row, t.column);
				exit(1);
			}
		}
		else{

			printf("Error. Expected a number at %d:%d.\n", t.row, t.column);
			exit(1);
		}
	}
	else{

		printf("Error. Expected '[', ',' or ';' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void il_prime_prime(){

	TOKEN t = get_next();

	if(strcmp(t.lexeme, ",")){
						
		//Accepted ','
		identifier_list();
		return;
	}
	else if(strcmp(t.lexeme, ";") == 0){

		//Accepted ';' which is in follow set of il_prime
		//Revert and allow some other production to handle this
		revert_token = 1;
		return;
	}
	else{

		printf("Error. Expected ',' or ';' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void assign_stat(){

	TOKEN t = get_next();
	if(t.type == 1){

		//Accepted an identifier (TOKEN of type 1)
		TOKEN t = get_next();
		if(strcmp(t.lexeme, "=") == 0){

			//Accepted '='
			expn();
			return;
		}
		else{

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

		//Accepted an identifier (TOKEN of type 1)
		//Originally, assign_stat should handle this identifier. Thus set revert_token and then call assign_stat
		revert_token = 1;
		assign_stat();

		t = get_next();
		if(strcmp(t.lexeme, ";") == 0){

			//Accepted ';'
			return;
		}
	}
	else if(strcmp(t.lexeme, "if") == 0){
		
		//Accepter 'if'
		//Originally, decision_stat should handle this identifier. Thus set revert_token and then call decision_stat
		revert_token = 1;
		decision_stat();
	}
	else if((strcmp(t.lexeme, "for") == 0) || (strcmp(t.lexeme, "while") == 0)){
		
		//Accepter 'for' or 'while'
		//Originally, looping_stat should handle this identifier. Thus set revert_token and then call looping_stat
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

		//Accepted '}'
		//'}' is in follow set of statement_list, thus allow the next function in pipeline to handle this token
		//by setting the revert_token flag
		revert_token = 1;
		return;
	}
	else if(t.type == 1 || strcmp(t.lexeme, "if") == 0 || strcmp(t.lexeme, "while") == 0 || strcmp(t.lexeme, "for") == 0){
		
		//Accepter an Identifier, 'if', 'while' or 'for'
		//Originally statement should handle this, thus set revert_flag and call statement
		revert_token = 1;
		statement();
		statement_list();
	}
	else{

		printf("Error. Expected a '}', identifier or some control statement at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void expn(){

	TOKEN t = get_next();

	if(t.type == 1 || t.type == 6){

		//Accepter a number or an identifier.
		//Let simple_expn handle this token. Set revert_token flag and call simple_expn.
		revert_token = 1;
		simple_expn();
		eprime();
		return;
	}
	else{

		printf("Error. Expected an identifier or a number at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void eprime(){

	TOKEN t = get_next();
	
	if((strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0)){
		revert_token = 1;
		return;
	}
	else if(t.type == 2){
		revert_token = 1;
		relop();
		simple_expn();
		return;
	}
	else{

		printf("Error. Expected a ';', ')' or some relational operator at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void simple_expn(){

	TOKEN t = get_next();

	if(t.type == 1 || t.type == 6){

		revert_token = 1;
		term();
		seprime();
	}
	else{

		printf("Error. Expected an identifier or a number at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void seprime(){

	TOKEN t = get_next();

	if(t.type == 2 || (strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0)){
		revert_token = 1;
		return;
	}
	else if((strcmp(t.lexeme, "+") == 0) || (strcmp(t.lexeme, "-") == 0)){
		revert_token = 1;
		addop();
		term();
		seprime();
		return;
	}
	else{

		printf("Error. Expected a relational operator, '+', '-', ';' or ')' at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void term(){

	TOKEN t = get_next();

	if(t.type == 1 || t.type == 6){

		revert_token = 1;

		factor();
		tprime();
	}
	else{

		printf("Error. Expected an identifier or a number at %d:%d.\n", t.row, t.column);
		exit(1);
	}
}

void tprime(){

	TOKEN t = get_next();

	if(t.type == 2 || (strcmp(t.lexeme, ";") == 0) || (strcmp(t.lexeme, ")") == 0) || (strcmp(t.lexeme, "+") == 0) || (strcmp(t.lexeme, "-") == 0)){
		
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
