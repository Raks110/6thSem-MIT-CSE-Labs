%option noyywrap
%{
	#include<string.h>

	int words = 0;
	int spaces = 0;
	int characters = 0;
	int lines = 0;

%}

%%
[a-zA-Z] {characters++;}
[ ] {spaces++;}
[a-zA-Z]+ {words++;}
\n {lines++;}
%%

int main()
{
	yylex();
	printf("Number of Words : %d\n",words);
	printf("Number of Spaces : %d\n",spaces);
	printf("Number of Chars : %d\n",characters);
	printf("Number of Lines : %d\n",lines);
}
