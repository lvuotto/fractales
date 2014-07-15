
/* pnm_list.c - gcc -Wall -O2 -c pnm_list.c -o pnm_list.o */

#include "pnm_list.h"
#include <stdlib.h>
#include <string.h>


#include <stdio.h>


pnm_list_t * create_list () {
    pnm_list_t *l;
    
    l = (pnm_list_t *) malloc(sizeof(pnm_list_t));
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
    
    return l;
}

void append_node (pnm_list_t *list, const char *s) {
    pnm_comment_t *n;
    
    n = (pnm_comment_t *) malloc(sizeof(pnm_comment_t));
    n->comment = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    strcpy(n->comment, s);
    n->next = NULL;
    
    if (list->size == 0) {
        list->first = n;
    } else {
        list->last->next = n;
    }
    list->last = n;
    
    list->size++;
}

void free_list (pnm_list_t *list) {
    pnm_comment_t *n;
    
    while (list->first != NULL) {
        n = list->first;
        list->first = list->first->next;
        free(n->comment);
        free(n);
    }
    n = NULL;
    
    list->first = NULL;
    list->last = NULL;
    
    free(list);
}


/*
int main (int argc, char **argv) {
    
    pnm_list_t *l;
    pnm_comment_t *n;
    l = create_list();
    
    append_node(l, "NODO 1");
    append_node(l, "NODO 2");
    append_node(l, "NODO 3");
    append_node(l, "NODO 4");
    append_node(l, "NODO 5");
    append_node(l, "NODO 6");
    append_node(l, "NODO 7");
    
    n = l->first;
    while (n != NULL) {
        printf("%s\n", n->comment);
        n = n->next;
    }
    
    free_list(l);
    
    return 0;
    
}//*/
