#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "libSOP.h"

// compilar: gcc -Wall -pthread libSOP.c main.c
// executar: ./a.out 2 a
int main( int argc, char *argv[] ) {

  setbuf(stdout, NULL);

  int   nthr;
  char  *nomearq;
  listaDupla *listaLE;
  pthread_t  *threads;
  int  rc;
  long t;

  if( argc != 3 ){
    printf( "Parametros nao informados!! %i", argc );
    return 0;
  }

  nthr    = atoi( argv[1] );
  nomearq = argv[2];

  listaLE = leArqEstoque( nomearq );

  threads = malloc( sizeof( pthread_t ) * nthr );

  for( t = 0; t < nthr; t++ ){

      printf( "main criando uma thread %ld\n", t );
      rc = pthread_create( threads + t, NULL, PrintHello, ( void* ) t );
      if( rc ){
          printf( "ERRO - rc=%d\n", rc );
          exit( -1 );
      }

  }
  pthread_exit(NULL);

  return 0;

}
