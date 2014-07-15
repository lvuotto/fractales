
/* comentarios.c - gcc -Wall -Werror -pedantic -g comentarios.c lib/lv_pnm.o -o comentarios */

#include <stdio.h>
#include "lib/lv_pnm.h"

int main (int argc, char **argv) {
  
  int i, j;
  pnm_context_t *pnm;
  
  for (i = 1; i < argc; i++) {
    pnm = pnm_create_from_file(*(argv + i));
    printf("Comentarios para '%s':\n", *(argv + i));
    for (j = 0; j < pnm->__comm_q; j++) {
      printf("%s\n", *(pnm->comments + j));
    }
    printf("\n");
  }
  
  return 0;
  
}
