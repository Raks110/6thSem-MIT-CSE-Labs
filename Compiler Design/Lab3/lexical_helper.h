//
// Created by hp on 1/29/2020.
//

#ifndef COMPILER_DESIGN_LEXICAL_HELPER_H
#define COMPILER_DESIGN_LEXICAL_HELPER_H

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "symbol_table.h"

int keywordDone;
char type[100];

struct token{

    char lexeme[100];
    unsigned int column;
    unsigned int row;
    unsigned int type;
};

typedef struct token TOKEN;

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

//Ignores comments and new lines

void ignore_fillers(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;
    char c = fgetc(fp);

    while(c=='\n'){

        c=fgetc(fp);
        if(c=='#'){

            while(c!='\n')
                c=fgetc(fp);

        }

        (*line)++;
        (*column)=1;
    }

    if(c=='/'){

        c=fgetc(fp);
        if(c=='/'){

            while(c!='\n')
                c=fgetc(fp);

            (*line)++;
        }
        else if(c=='*'){

            while(!(c=='*' && fgetc(fp)=='/')){

                if(c=='\n')
                    (*line)++;
                c=fgetc(fp);
            }
        }
    }

    c = fgetc(fp);

    while(c=='\n'){

        c=fgetc(fp);
        if(c=='#'){

            while(c!='\n')
                c=fgetc(fp);

        }

        (*line)++;
        (*column)=1;
    }
}

// Checks for any arithmetic operators

void check_arithmetic(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

    char c = fgetc(fp);
    TOKEN temp1;
    temp1.type = -1;

    if(c == '+' || c == '-'){

        char temp = fgetc(fp);
        char buf[3];

        if(temp == c || temp == '='){

            buf[0] = c; buf[1] = temp; buf[2] = '\0';
            (*column) += 2;
            set_token(buf, *column, *line, 3, &temp1);
        }
        else{

            buf[0] = c; buf[1] = '\0';
            (*column) += 1;
            set_token(buf, *column, *line, 3, &temp1);
            revert(fp);
        }
    }
    else if(c == '/' || c == '*' || c == '%'){

        char temp = fgetc(fp);
        char buf[3];

        if(temp == '='){

            buf[0] = c; buf[1] = temp; buf[2] = '\0';
            (*column) += 2;
            set_token(buf, *column, *line, 3, &temp1);
        }
        else{

            buf[0] = c; buf[1] = '\0';
            (*column) += 1;
            set_token(buf, *column, *line, 3, &temp1);
            revert(fp);
        }
    }

    if(temp1.type != -1)
        print_token(temp1);
}

void check_logical(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

    char c = fgetc(fp);
    TOKEN temp1;

    temp1.type = -1;

    if(c == '&' || c == '|'){

        char temp = fgetc(fp);
        char buf[3];

        if(temp == c){

            buf[0] = c; buf[1] = temp; buf[2] = '\0';
            (*column) += 2;
            set_token(buf, *column, *line, 4, &temp1);
        }
        else{

            buf[0] = c; buf[1] = '\0';\
			(*column) += 1;
            set_token(buf, *column, *line, 5, &temp1);
            revert(fp);
        }
    }
    else if(c == '!'){

        char buf[2];

        buf[0] = c; buf[1] = '\0';
        (*column) += 1;
        set_token(buf, *column, *line, 4, &temp1);
    }

    if(temp1.type != -1)
        print_token(temp1);
}

void check_special(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

    char c = fgetc(fp);
    if(c == '.' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';'){

        char buf[2];
        buf[0] = c; buf[1] = '\0';

        TOKEN t;

        (*column) += 1;

        set_token(buf, *column, *line, 5, &t);
        print_token(t);
    }
}

void check_literal(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

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

        TOKEN t;
        (*column) += strlen(buf);
        set_token(buf, *column, *line, 7, &t);
        print_token(t);
    }
}

void check_relational(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

    char c = fgetc(fp);
    TOKEN temp1;
    temp1.type = -1;

    if(c == '>' || c == '<' || c == '='){

        char temp = fgetc(fp);
        char buf[3];

        if(temp == '='){

            buf[0] = c; buf[1] = temp; buf[2] = '\0';
            (*column) += 2;
            set_token(buf, *column, *line, 2, &temp1);
        }
        else{

            buf[0] = c; buf[1] = '\0';
            (*column) += 1;
            set_token(buf, *column, *line, 2, &temp1);
            revert(fp);
        }
    }

    if(temp1.type != -1)
        print_token(temp1);

}

void check_number(FILE *fp, int *line, int *column){

    revert(fp);
    keywordDone = 0;

    char c = fgetc(fp);
    TOKEN temp1;
    temp1.type = -1;
    char buf[100];
    int i = 0;

    while(isdigit(c)){

        buf[i++] = c;
        c = fgetc(fp);
        temp1.type = 8;
    }

    buf[i] = '\0';

    if(temp1.type == 8){

        (*column) += strlen(buf);
        set_token(buf, *column, *line, 6, &temp1);
        print_token(temp1);
    }
}

int is_keyword(char *buf){

    //init keys

    char keys[11][100];

    strcpy(keys[0],"int");
    strcpy(keys[1],"float");
    strcpy(keys[2],"char");
    strcpy(keys[3],"main");
    strcpy(keys[4],"if");
    strcpy(keys[5],"while");
    strcpy(keys[6],"for");
    strcpy(keys[7],"break");
    strcpy(keys[8],"printf");
    strcpy(keys[9],"return");
    strcpy(keys[10],"else");
    int length = 11;

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

    FILE *fcopy = fopen("./samples/sample_test2.txt", "r");
    fseek(fcopy, pos, SEEK_CUR);

    char c = fgetc(fcopy);
    if(c == ' '){
        c = fgetc(fcopy); 
    }

    char arg[100];
    int i=0;

    if(c == '('){

        char temp;
        temp = fgetc(fcopy);

        while(isalpha(temp) || isdigit(temp) || temp == ' ' || temp == ','){

            if(temp == ','){
                
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
    }

    entry = create_entry(buf, num, type, args, 0);
    return entry;
}

NODE* check_word(FILE *fp, int *line, int *column, NODE* table){

    revert(fp);
    char c = fgetc(fp);

    char buf[100];
    int i = 0;

    if(isalpha(c) || c == '_'){

        buf[i++] = c;
        char temp = fgetc(fp);

        while(isalnum(temp) || temp == '_'){

            buf[i++] = temp;
            temp = fgetc(fp);
        }

        buf[i] = '\0';
        revert(fp);

        TOKEN t;
        (*column) += strlen(buf);
        if(is_keyword(buf)) {

            keywordDone = 1;
            strcpy(type, buf);
            set_token(buf, *column, *line, 0, &t);
        }
        else{

            if(keywordDone == 1){
                ENTRY entry = check_identifier(fp, buf);
                table = insert_into_list(table, entry);
            }
            keywordDone = 0;
            set_token(buf, *column, *line, 1, &t);
        }
        print_token(t);
    }

    return table;
}

void get_token_stream(FILE *fp){

    char c = fgetc(fp);
    NODE *table = NULL;

    int lines = 0, columns = 1;

    while(c != EOF){

        if(c == '\n'){
            lines++;
            columns=0;
            c = fgetc(fp);
            continue;
        }
        if(c == ' '){
            columns++;
            c = fgetc(fp);
            continue;
        }
        if(isalpha(c) || c == '_'){
            table = check_word(fp, &lines, &columns, table);
            c = fgetc(fp);
            continue;
        }
        if(isdigit(c)){
            check_number(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
        if(c == '>' || c == '<' || c == '='){

            check_relational(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
        if(c == '"'){
            check_literal(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
        if(c == '&' || c == '!' || c == '|'){
            check_logical(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
        if(c == '+' || c == '*' || c == '/' || c == '%' || c == '-'){
            check_arithmetic(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
        if(c == '/')
            ignore_fillers(fp, &lines, &columns);
        else{
            check_special(fp, &lines, &columns);
            c = fgetc(fp);
            continue;
        }
    }

    printf("\nSymbol Table:\n\n");
    print_list(table);
}

#endif //COMPILER_DESIGN_LEXICAL_HELPER_H
