%option noyywrap
%{

	int c = 0;
	int v= 0;


%}

%%
[aeiouAEIOU] {v++;}
[^aeiouAEIOU] {c++;}
%%

int main()
{
	yylex();
	printf("Number of Vovels : %d\n",v);
	printf("Number of Consonants : %d\n",c);
	
}