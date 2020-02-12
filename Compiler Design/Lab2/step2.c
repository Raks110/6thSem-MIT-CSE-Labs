void print_token(TOKEN a){
char a1[100];
switch(a.type){
case 0:
strcpy(a1,"KEYWORD");
break;
case 1:
strcpy(a1,"IDENTIFIER");
break;
case 2:
strcpy(a1,"RELOP");
break;
case 3:
strcpy(a1,"AOP");
break;
case 4:
strcpy(a1,"LOP");
break;
case 5:
strcpy(a1,"SPECIAL CHAR");
break;
case 6:
strcpy(a1,"NUMBER");
break;
case 7:
strcpy(a1,"LITERAL");
break;
}
printf("<%s,%d,%d,%s>\n",a.lexeme,a.column,a.row,a1);
}
TOKEN get_next_token(){
FILE *fa=fopen("step2.c","r");
char ca=getc(fa);
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
