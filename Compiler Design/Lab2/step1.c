
typedef struct token{

	char lexeme[100];
	unsigned int column;
	unsigned int row;
	unsigned int type;
}TOKEN;


int check_error(FILE* file){

	if(file == NULL){

		printf("%s\n", "E104: Unexpected Error while opening file.");
		return 1;
	}

	return 0;
}

void clear_predirs(){

	FILE *fa, *fb;
	int ca, cb;

	fa = fopen("q1.c", "r");
	fb = fopen("step1.c", "w");

	if(check_error(fa) || check_error(fb))
		return;

	ca = getc(fa);
	int flag = 0;

	while(ca != EOF){

		if(ca == '#' && !flag){

			cb = getc(fa);

			while(cb != '\n'){
				cb = getc(fa);
			}

			ca = getc(fa);
			continue;
		}
		else if(ca != '/'){

			flag = 1;
		}

		putc(ca, fb);
		ca = getc(fa);
	}

	fclose(fa);
	fclose(fb);
}

void clear_spaces(){

	FILE *fa, *fb;
	int ca, cb;

	fa = fopen("step1.c", "r");
	fb = fopen("step2.c", "w");

	if(check_error(fa) || check_error(fb))
		return;

	ca = getc(fa);

	while(ca != EOF){

		if(ca == ' ' || ca == '\t'){

			cb = getc(fa);

			if(ca == ' ' && cb != ' ' && cb != '\t')
				putc(' ', fb);

			else{
				
				while(cb == ' ' || cb == '\t')
					cb = getc(fa);
			}
			
			putc(cb, fb);
		}
		else
			putc(ca, fb);

		ca = getc(fa);
	}

	fclose(fa);
	fclose(fb);
}

void print_token(TOKEN a){

	char a1[100];
	switch(a.type){

		case 0:
			strcpy(a1, "KEYWORD");
			break;
		case 1:
			strcpy(a1, "IDENTIFIER");
			break;
		case 2:
			strcpy(a1, "RELOP");
			break;
		case 3:
			strcpy(a1, "AOP");
			break;
		case 4:
			strcpy(a1, "LOP");
			break;
		case 5:
			strcpy(a1, "SPECIAL CHAR");
			break;
		case 6:
			strcpy(a1, "NUMBER");
			break;
		case 7:
			strcpy(a1, "LITERAL");
			break;
	}

	printf("<%s, %d, %d, %s>\n", a.lexeme, a.column, a.row, a1);
}

TOKEN get_next_token(){

			printf("Entering state");
	FILE *fa = fopen("step2.c", "r");
	char ca = getc(fa);
	char buf[100];
	int number = 0;
	int i=0;
	int state = 0;
	int cols = 0, rows = 0;
	
	while(ca!=EOF){

		switch(state){

			TOKEN temp;

			case 0:
				if(isalpha(ca)){

					state = 1;
					buf[i] = ca;
					i++;
				}
				else if(isdigit(ca)){

					state = 2;
					buf[i] = ca;
					i++;
				}
				else if(ca == '.' || ca == ';' || ca == ',' || ca == '(' || ca == ')' || ca == '{' || ca == '}'){

					state = 3;
					buf[i] = ca;
					buf[i+1] = '\0';
				}
				else if(ca == '+' || ca == '/' || ca == '*' || ca == '%' || ca == '<' || ca == '>' || ca == '='){

					state = 4;
					buf[i] = ca;
					i++;
				}
				else if(ca == '!' || ca == '&' || ca == '|'){

					state = 8;
					buf[i] = ca;
					i++;
				}
				else if(ca == '"'){

					state = 9;
				}
				break;

			case 1:
				if(isalpha(ca)){

					state = 1;
					buf[i] = ca;
					i++;
				}
				else{

					fa--;
					state = 5;
					buf[i] = '\0';
				}
				state = 1;
				break;

			case 2:
				if(isdigit(ca)){

					state = 1;
					buf[i] = ca;
					i++;
				}
				else{

					fa--;
					state = 6;
					buf[i] = '\0';
				}
				state = 2;
				break;

			case 3:
				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 5;
				
				print_token(temp);
				i = 0;
				state = 0;
				break;

			case 4:
				if(isalpha(ca) || isdigit(ca)){

					fa--;
					state = 7;
					buf[i] = '\0';
				}
				else{
					buf[i] = ca;
					buf[i+1] = '\0';
					state = 7;
				}
				break;

			case 5:

				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 0;
				
				if(strcmp(buf, "if") || strcmp(buf, "else") || strcmp(buf, "for") || strcmp(buf, "while") || strcmp(buf, "do") || strcmp(buf, "main") || strcmp(buf, "int") || strcmp(buf, "char") || strcmp(buf, "switch") || strcmp(buf, "case")){

					temp.type = 0;
					print_token(temp);
				}
				else{

					temp.type = 1;
					print_token(temp);	
				}
				state = 0;
				i = 0;
				break;

			case 6:
				
				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 6;

				print_token(temp);
				state = 0;
				i = 0;
				break;

			case 7:
				
				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 2;

				if(buf[0] == '+' || buf[0] == '/' || buf[0] == '*' || buf[0] == '%')
					temp.type++;
				else if(buf[0] == '=' && buf[0] != '=')
					temp.type++;
				print_token(temp);
				state = 0;
				i = 0;
				break;

			case 8:
				if(ca != '!'){
					ca = getc(fa);
					buf[i] = ca;
					i++;
				}
				buf[i] = '\0';
				
				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 4;

				print_token(temp);

				i = 0;
				state = 0;
				break;

			case 9:
				if(ca != '"'){
					buf[i] = ca;
					i++;
					state = 9;
				}
				else{
					buf[i] = '\0';
					state = 10;
				}
				break;

			case 10:
				
				strcpy(temp.lexeme, buf);
				temp.column = cols;
				temp.row = rows;
				temp.type = 7;

				print_token(temp);
				i = 0;
				state = 0;
				break;
		}

		if(state != 3 && state != 6 && state != 5 && state != 7 && state != 8 && state != 10){
			ca = getc(fa);
			if(ca == '\n'){
				cols++;
				rows = 0;
				ca = getc(fa);
			}
			else
				rows++;
		}
	}
}

int main(){

	clear_predirs();
	clear_spaces();

	get_next_token();

	return 0;
}