#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void strupr(char* str, char* res) {

	strcpy(res, str);

	for(int i = 0; str[i] != '\0'; i++) {
		res[i] = toupper(res[i]);
	}
}

int check_keyword(char* str, char** KEYS, size_t KEY_LEN, int line_num) {

	int col = -1;

	for(int i = 0; i < KEY_LEN; i++) {

		char *pos = strstr(str, KEYS[i]);

		if(pos != NULL) {

			char* res = (char*) malloc(sizeof(char) * strlen(KEYS[i]));
			strupr(KEYS[i], res);

			printf("%s", res);

			col = pos - str + 4;

			printf("\tFound at line %d \tFound at col %d\n", line_num, col);

		}

	}


	return col;
}
#include <stdio.h>
#include<string.h>

int main(){

	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("file_read.txt", "r");
	if (fp == NULL)
	    exit(0);

	int flag = 0;

	while ((read = getline(&line, &len, fp)) != -1) {

		if(!strcmp("<TEXT>\r\n", line))
	    	flag = 1;
	    else if(!strcmp("</TEXT>\r\n", line))
	    	flag = 0;
	    else if(flag==1)
	    	printf("%s", line);
	}

    fclose(fp);
    if (line)
        free(line);
    exit(1);
}





int main() {

	char** KEYS = (char**) malloc(5 * sizeof(char*));

	for(int i = 0; i < 5; i++) {
		KEYS[i] = (char*) malloc(sizeof(char) * 10);
	} 

	strcpy(KEYS[0], "int");
	strcpy(KEYS[1], "main");
	strcpy(KEYS[2], "for");
	strcpy(KEYS[3], "return");
	strcpy(KEYS[4], "if");

	FILE* fp = fopen("./q3.c", "r+");

	char* line = (char*) malloc(sizeof(char) * 300);
	size_t len = 0;

	int read = 0;

	int line_num = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		line_num++;

		int col = check_keyword(line, KEYS, 5, line_num);

	}

	return 0;

}