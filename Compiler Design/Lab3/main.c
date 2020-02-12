#include "lexical_helper.h"
#include "symbol_table.h"

void removePre(){
	FILE *fa, *fb;
	int ca, cb;

	fa = fopen("./samples/sample_test1.txt", "r");
	fb = fopen("./samples/sample_test2.txt", "w");

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


void removeComments() {
	FILE *fa,*fb;
	int ca,cb;
	fa = fopen("./samples/sample2.txt","r");
	fb = fopen("./samples/sample_test1.txt","w");
	ca = getc(fa);
	while (ca != EOF) {
		if (ca == '/') {
			cb = getc(fa);
			if (cb == '/') {
				while (ca != '\n')
					ca = getc(fa);
			}
			else if (cb == '*') {
				do {
					while(ca != '*')
						ca = getc(fa);
					ca = getc(fa);
				} while(ca != '/');
				ca = getc(fa);
				// if (ca == '\n')
				//     ca = getc(fa);	
			}
			else {
				putc(ca,fb);
				putc(cb,fb);
			}
		}
		else
			putc(ca,fb);
			ca = getc(fa);
	}
	fclose(fa);
	fclose(fb);
}

int main(){

	removeComments();
	removePre();

	FILE *fp = fopen("./samples/sample_test2.txt","r");
	int l=0,r=0;

	get_token_stream(fp);

    /*NODE* table = NULL;
    char args[][10] = {"Hello", "Bellow"};

	ENTRY entry = create_entry("abs",2,"int", args, 1);
	table = insert_into_list(table, entry);

    ENTRY entry2 = create_entry("backpack",2,"float", args, 1);
	table = insert_into_list(table, entry2);
	print_list(table);*/
}