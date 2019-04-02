#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>

// compilar: gcc -Wall -pthread -o prog prog.c
int main( int argc, char *argv[] ) {

  int nthr;
  char* nomearq;

  if( argc != 3 ){
    printf( "Parametros nao informados!! %i", argc );
    return 0;
  }

  nthr = atoi( argv[1] );
  nomearq = argv[2];

  printf( "\n nthr: %i \n nomearq: %s ", nthr, nomearq );

  return 0;

}
