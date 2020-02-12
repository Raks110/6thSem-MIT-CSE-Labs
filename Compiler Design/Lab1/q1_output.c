#include <stdio.h>
#include <stdlib.h>
#include "file_help.h"

int main(int argc, char** argv){

FILE *fa, *fb;
int ca, cb;

fa = fopen("q1_input.c", "r");
fb = fopen("q1_output.c", "w");

if(check_error(fa) || check_error(fb))
exit(0);

ca = getc(fa);

while(ca != EOF){

if(ca == ' ' || ca == '\t'){

cb = getc(fa);

if(ca == ' ' && cb != ' ' && cb != '\t')
putc(ca, fb);

else{

while(cb == ' ' || cb == '\t')
cb = getc(fa);
}

putc(cb, fb);
}
else
putc(ca, fb);

ca = getc(fa);
}

fclose(fa);
fclose(fb);

return 0;
}