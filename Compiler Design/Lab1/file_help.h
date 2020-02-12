#include <string.h>
#include <ctype.h>

char types[10][30] = {"INT", "FILE", "CHAR", "FOR", "WHILE", "IF", "ELSE", "MAIN", "CONTINUE", "RETURN"};

int check_error(FILE* file){

	if(file == NULL){

		printf("%s\n", "E104: Unexpected Error while opening file.");
		return 1;
	}

	return 0;
}

int is_keyword(FILE *fp, char a, int col, int row, FILE *output){

	char temp[30] = "";
	char add[2] = {toupper(a), '\0'};

	if(a != ' ' || a != '\t' || a != '\n'){
		
		strcat(temp, add);

		while(a != ' ' || a != '\t' || a != '\n'){

			a = getc(fp);
			add[0] = toupper(a);

			printf("%c\n", a);

			strcat(temp, add);
		}
	}
	else{
		return -1;
	}

	for(int i=0;i<10;i++){
		if(!strcmp(temp, types[i])){

			fputs(temp, output);
			
			putc(' ', output);
			
			char str[10];
			sprintf(str, "%d", row);

			fputs(str, output);

			putc(':', output);

			char str1[10];
			sprintf(str, "%d", col);

			fputs(str1, output);
		}
	}

	return strlen(temp);
}