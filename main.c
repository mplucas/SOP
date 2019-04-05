#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "libSOP.h"
#include "utils.h"

listaDupla *listaLE;
char *nomearq;

/* inicializa_lanches(arq_ofertas);
cria_threads();
espera_fim_threads();
imprime_valor_total();
imprime_estoque(); */

// compilar: gcc -Wall -pthread libSOP.c main.c
// executar: ./a.out 2 a
int main( int argc, char *argv[] ) {

  setbuf(stdout, NULL);

  int  nthr;
  int  rc;
  long t;
  pthread_t  *threads;

  if( argc != 3 ){
    printf( "Parametros nao informados!! %i", argc );
    return 0;
  }

  nthr    = atoi( argv[1] );
  nomearq = argv[2];
  listaLE = leArqEstoque( nomearq );
  threads = malloc( sizeof( pthread_t ) * nthr );

  printf( "\nLista antes da execucao das threads:\n" );
  mostraLista( listaLE );

  for( t = 0; t < nthr; t++ ){

      rc = pthread_create( threads + t, NULL, processaPredido, ( void* ) t );
      if( rc ){
          printf( "ERRO - rc=%d\n", rc );
          exit( -1 );
      }

  }
  pthread_exit(NULL);

  printf( "\nLista após execucao das threads:\n" );
  mostraLista( listaLE );

  return 0;

}
