#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>

#include "libSOP.h"

// compilar: gcc -Wall -pthread libSOP.c main.c
// ou gcc -Wall libSOP.c main.c
// executar: ./a.out 1 a
int main( int argc, char *argv[] ) {

  setbuf(stdout, NULL);

  int   nthr;
  char* nomearq;
  listaDupla* listaLE;

  if( argc != 3 ){
    printf( "Parametros nao informados!! %i", argc );
    return 0;
  }

  nthr = atoi( argv[1] );
  nomearq = argv[2];

  listaLE = leArqEstoque( nomearq );

  return 0;

}
