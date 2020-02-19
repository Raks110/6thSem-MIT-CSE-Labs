#include "lexical_helper.h"
#include "symbol_table.h"

FILE *fp;
NODE *table = NULL;

int lines = 0, columns = 1;
int row = -1, col = -1;

void declerations();
void statement_list();

void program(){

	TOKEN t = get_stream(&row, &col, &lines, &columns, fp, &table);

	if(strcmp(t.lexeme, "main") == 0){

		t = get_stream(&row, &col, &lines, &columns, fp, &table);
		if(strcmp(t.lexeme, "(") == 0){

			t = get_stream(&row, &col, &lines, &columns, fp, &table);
			if(strcmp(t.lexeme, ")") == 0){

				t = get_stream(&row, &col, &lines, &columns, fp, &table);
				if(strcmp(t.lexeme, "{") == 0){

					declerations();
					statement_list();

					t = get_stream(&row, &col, &lines, &columns, fp, &table);
					if(strcmp(t.lexeme, "}") == 0){

						//Success
						return;
					}
					else{
						printf("Error\n");
						return;
					}
				}
				else{

					printf("Error\n");
					return;
				}
			}
			else{

				printf("Error\n");
				return;
			}		
		}
		else{

			printf("Error\n");
			return;
		}
	}
	else{
		printf("Error\n");
		return;
	}
}

void data_type(){

	TOKEN t = get_stream(&row, &col, &lines, &columns, fp, &table);

	if(strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char")){
		//Success
	}
	else{

		printf("Error\n");
		return;
	}
}

int main(){

	removeComments();
	removePre();

	fp = fopen("./samples/sample_test2.txt","r");
	
	char c = fgetc(fp);

    while(c != EOF){

    	revert(fp);
	
		TOKEN t = get_stream(&row, &col, &lines, &columns, fp, &table);
		print_token_to_screen(t);

		c = getc(stdin); 

    	c = fgetc(fp);
	}
}