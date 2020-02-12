#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "symbol_table.h"

struct token{

    char lexeme[100];
    unsigned int column;
    unsigned int row;
    unsigned int type;
};

typedef struct token TOKEN;
int scope = 0;

void set_token(char *l, unsigned int cols, unsigned int row, unsigned int type, TOKEN *temp){

    strcpy(temp->lexeme, l);
    temp->column = cols;
    temp->row = row;
    temp->type = type;
}

void print_token(TOKEN a){

    char a1[100];
    switch(a.type){

        case 0:
            strcpy(a1, "KEYWORD");
            break;
        case 1:
            strcpy(a1, "IDENTIFIER");
            break;
        case 2:
            strcpy(a1, "RELOP");
            break;
        case 3:
            strcpy(a1, "AOP");
            break;
        case 4:
            strcpy(a1, "LOP");
            break;
        case 5:
            strcpy(a1, "SPECIAL CHAR");
            break;
        case 6:
            strcpy(a1, "NUMBER");
            break;
        case 7:
            strcpy(a1, "LITERAL");
            break;
    }

    printf("<%s, %d, %d, %s>\n", a.lexeme, a.row, a.column, a1);
}

void revert(FILE *fp){

    if(fseek(fp, -1, SEEK_CUR)){

        printf("E101: Error encountered. Invalidate output.");
    }
}

void is_arithmetic(FILE *fp){

    revert(fp);
    char c = fgetc(fp);

    if(c == '+' || c == '-' || c == '*' || c == '/'){
        printf("<%c>", c);
    }
}

void ignore_fillers(FILE *fp){

    revert(fp);
    char c = fgetc(fp);

    if(c == '-'){

        c = fgetc(fp);
        if(c == '-'){

            char temp = fgetc(fp);
            while(temp != '\n'){
                temp = fgetc(fp);
            }
        }
        else{
            revert(fp);
            is_arithmetic(fp);
        }
    }
}

void check_special(FILE *fp){

    revert(fp);

    char c = fgetc(fp);
    if(c == '.' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ':'){

        if(c == '{'){
            scope++;
        }
        else if(c == '}'){
            scope--;
        }

        char buf[2];
        buf[0] = c; buf[1] = '\0';

        printf("<%c>", c);
    }
}

void check_literal(FILE *fp){

    revert(fp);

    char c = fgetc(fp);
    if(c == '"'){

        char buf[100];
        int i = 0;

        char temp = fgetc(fp);
        while(temp!='"'){

            buf[i++] = temp;
            temp = fgetc(fp);
        }

        buf[i] = '\0';
        printf("<%s>",buf);
    }
}

void check_number(FILE *fp){

    revert(fp);

    char c = fgetc(fp);
    char buf[100];
    int i = 0;

    while(isdigit(c)){

        buf[i++] = c;
        c = fgetc(fp);
    }

    buf[i] = '\0';
    printf("<%s>", buf);
}

int is_keyword(char *buf){

    char keys[4][100];

    strcpy(keys[0],"String");
    strcpy(keys[1],"concat");
    strcpy(keys[2],"Int");
    strcpy(keys[3],"Class");
    int length = 4;

    //check if buf is a key

    for(int i=0;i<length;i++){

        if(!strcmp(buf, keys[i])){

            return 1;
        }
    }

    return 0;
}

ENTRY check_identifier(FILE *fp, char buf[]){

    ENTRY entry;
    char args[100][10];

    int num = 0;

    int pos = ftell(fp);

    FILE *fcopy = fopen("sample.txt", "r");
    fseek(fcopy, pos, SEEK_CUR);

    char c = fgetc(fcopy);
    if(c == ' '){
        c = fgetc(fcopy); 
    }

    char arg[100];
    int i=0;
    char type[100];

    if(c == '('){

        char temp;
        temp = fgetc(fcopy);

        while(isalpha(temp) || isdigit(temp) || temp == ' ' || temp == ':'){

            if(temp == ':'){
                
                arg[i++] = '\0';
                strcpy(args[num], arg);
                i = 0;
                num++;
            }
            else{

                arg[i++] = temp;
            }
            temp = fgetc(fcopy);
            
            if(temp == ')'){

                if(i != 0){
                    arg[i++] = '\0';
                    strcpy(args[num], arg);
                    num++;
                    i=0;
                }
            }
        }
        if(temp == ':'){

            temp = fgetc(fp);

            while(isalpha(temp)){
                arg[i++] = temp;
                temp = fgetc(fp);
            }

            arg[i] = '\0';
            strcpy(type, arg);
        }
    }

    entry = create_entry(buf, num, type, args, scope >= 1);
    return entry;
}

NODE* check_word(FILE *fp, NODE *table){

    revert(fp);
    char c = fgetc(fp);
    char buf[100];
    int i=0;

    while(isalpha(c) || c == '_'){

        buf[i++] = c;
        c = fgetc(fp);
    }

    buf[i] = '\0';

    if(is_keyword(buf)){
        printf("<%s>",buf);
    }
    else{

        if(c == ':' || c == '(' || c == '{'){
            ENTRY entry = check_identifier(fp, buf);
            table = insert_into_list(table, entry);

            printf("<ID, %d>", entry.id);
        }
        else{

            printf("<ID, %s>", buf);
        }
    }

    return table;
}

void check_equal(FILE *fp){
    revert(fp);

    char c = fgetc(fp);
    if(c == '<'){

        c = fgetc(fp);
        if(c == '-'){
            printf("<<->");
        }
    }
}

void get_token_stream(FILE *fp){

    char c = fgetc(fp);
    NODE *table = NULL;

    while(c != EOF){

        if(c == '\n'){
            printf("\n");
            c = fgetc(fp);
            continue;
        }
        if(c == ' '){
            c = fgetc(fp);
            continue;
        }
        if(isalpha(c) || c == '_'){
            table = check_word(fp, table);
            c = fgetc(fp);
            continue;
        }
        if(isdigit(c)){
            check_number(fp);
            c = fgetc(fp);
            continue;
        }
        if(c == '<'){

            check_equal(fp);
            c = fgetc(fp);
            continue;
        }
        if(c == '"'){
            check_literal(fp);
            c = fgetc(fp);
            continue;
        }
        if(c == '-'){
            ignore_fillers(fp);
            c = fgetc(fp);
            continue;
        }
        if(c == '+' || c == '*' || c == '/' || c == '-'){
            is_arithmetic(fp);
            c = fgetc(fp);
            continue;
        }
        else{
            check_special(fp);
            c = fgetc(fp);
            continue;
        }
    }

    printf("\nSymbol Table:\n\n");
    print_list(table);
}

int main(){

    FILE *fp = fopen("sample.txt","r");
    get_token_stream(fp);
}