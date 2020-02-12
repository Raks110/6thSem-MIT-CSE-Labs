#include <stdio.h>
#include <stdlib.h>
#include "file_help.h"

int main(int argc, char** argv){

	FILE *fa, *fb;
	int ca, cb;

	fa = fopen("q1_input.c", "r");
	fb = fopen("q2_output.c", "w");

	if(check_error(fa) || check_error(fb))
		exit(0);

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

	return 0;
}