#include<stdio.h>
#include<ctype.h>

/*

	E -> numT
	T -> *numT | ε

*/

FILE *fp;
int error = 0;
int success = 0;

char read_next(){

	return fgetc(fp);
}

void revert(){

	fseek(fp, -1, SEEK_CUR);
}

void T(){

	char c = read_next(fp);
	
	if(c == '$' && !error){
		printf("Successfully Read.\n");
		success = 1;
	}

	if(c == '*'){

		char c = read_next(fp);
		if(isdigit(c)){

			while(isdigit(c)){
				c = read_next(fp);
			}
			revert();
			T();
		}
		else if(!success){
			printf("Unexpected Input. I am a dumb mini-parser!\n");
			error = 1;
		}		
	}
	else if(!success){
		printf("Unexpected Input. I am a dumb mini-parser!\n");
		error = 1;
	}
}

void E(){

	char c = read_next(fp);
	
	if(c == '$' && !error){
		printf("Successfully Read.\n");
		success = 1;
	}

	if(isdigit(c)){

		while(isdigit(c)){
			c = read_next(fp);
		}
	}
	revert();
	T();
}

int main(){

	fp = fopen("./samples/mini.txt", "r");
	E();
}