%option noyywrap
%{
	int pc=0, sc=0;
%}

%%
printf {fprintf(yyout, "write"); pc++;}
scanf {fprintf(yyout, "read"); sc++;}
. {ECHO;}
%%

int main(){
	yyin = fopen("a.c", "r");
	yyout = fopen("b.c", "r");
	yylex();
}