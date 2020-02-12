#include<stdio.h>
#include<stdlib.h>

FILE *fp;

char read_next(){

	return fgetc(fp);
}

void revert(){

	fseek(fp, -1, SEEK_CUR);
}

FILE* open_file(char filename[]){

	return fopen(filename, "r");
}

int is_first_char(FILE *fp){

	return ftell(fp) == 1;
}

int current_position(FILE *fp){

	return ftell(fp);
}

void red(){
  printf("\033[1;31m");
}

void green(){
  printf("\033[0;32m");
}

void reset(){
  printf("\033[0m");
}

void print_error(char error[], char hint[]){
	
	red();
	printf("%s",error);
	reset();
	green();
	printf("Hint: ");
	reset();
	printf("%s", hint);
}