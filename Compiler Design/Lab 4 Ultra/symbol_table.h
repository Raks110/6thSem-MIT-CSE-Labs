//
// Created by hp on 1/29/2020.
//

#ifndef COMPILER_DESIGN_SYMBOL_TABLE_H
#define COMPILER_DESIGN_SYMBOL_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int id = 0;

typedef struct symbol_table_entry{

    int id;
    char name[100];
    int num_args;
    char args[10][100];
    int scope;
    char return_type[10];
} ENTRY;

typedef struct node{

    ENTRY entry;
    struct node *next;
} NODE;

ENTRY create_entry(char name[], int num_args, char return_type[], char args[][10], int scope){

    ENTRY entry;
    strcpy(entry.name, name);
    strcpy(entry.return_type, return_type);
    entry.num_args = num_args;
    entry.id = id++;
    entry.scope = scope;

    for(int i=0;i<num_args;i++){

        strcpy(entry.args[i], args[i]);
    }

    return entry;
}

void print_entry(ENTRY entry){

    printf("%d: %s %s: %d: Scope: %d\n", entry.id, entry.return_type, entry.name, entry.num_args, entry.scope);
    if(entry.num_args > 0){
        printf("<");
        for(int i=0;i<entry.num_args;i++){

            if(i != entry.num_args-1)
                printf("%s, ", entry.args[i]);
            else
                printf("%s", entry.args[i]);
        }

        printf(">\n\n");
    }
    else
        printf("<Arg List not present>\n\n");
}

NODE* init_list(NODE* table, ENTRY entry){

    table = malloc(sizeof(NODE));
    table->entry = entry;
    table->next = NULL;

    return table;
}

NODE* insert_into_list(NODE *table, ENTRY entry){

    NODE* node = malloc(sizeof(NODE));
    node->entry = entry;
    node->next = NULL;

    NODE *ptr = table;

    if(ptr!=NULL) {
        while(ptr->next!=NULL){

            if(strcmp(ptr->entry.name, entry.name) == 0){
                return table;
            }

            ptr = ptr->next;
        }
        if (strcmp(ptr->entry.name, entry.name) == 0) {
            return table;
        }

        ptr->next = malloc(sizeof(NODE));
        ptr->next = node;
    }
    else{

        table = init_list(table, entry);
    }

    return table;
}

void print_list(NODE* table){

    if(table == NULL){

        printf("Table is empty.\n");
    }
    else{

        NODE *ptr = table;
        while(ptr!=NULL){

            print_entry(ptr->entry);
            ptr = ptr->next;
        }
    }
}

#endif //COMPILER_DESIGN_SYMBOL_TABLE_H
