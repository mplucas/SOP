#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "libSOP.h"
#include "utils.h"

listaEstoque *lEstoque;
char *nomearq;
pthread_mutex_t mtxPedido;
pthread_mutex_t mtxFimPedido;
int fimThreads = 0;
pthread_cond_t condFim;
listaPedido *lPedido;

/* inicializa_lanches(arq_ofertas);
cria_threads();
espera_fim_threads();
imprime_valor_total();
imprime_estoque(); */

// compilar: gcc -Wall -pthread libSOP.c main.c
// executar: ./a.out 2 a
// testes: for i in `seq 1 10`; do ./a.out 2 a; done
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
  lEstoque = leArqEstoque( nomearq );
  lPedido = criarLDP();
  threads = malloc( sizeof( pthread_t ) * nthr );

  printf( "\n------------------------------------\nLista antes da execucao das threads:\n" );
  mostraLDE( lEstoque );

  for( t = 0; t < nthr; t++ ){

      rc = pthread_create( threads + t, NULL, processaPredido, ( void* ) t );
      if( rc ){
          printf( "ERRO - rc=%d\n", rc );
          exit( -1 );
      }

  }

  pthread_mutex_lock( &mtxFimPedido );
  while( fimThreads != nthr ){
    printf( "\n fimThreads: %i | nthr: %i", fimThreads, nthr );
    pthread_cond_wait( &condFim, &mtxFimPedido );
    printf( "\n sai do condwait fimThreads: %i", fimThreads );
  }
  pthread_mutex_unlock( &mtxFimPedido );

  printf( "\nLista após execucao das threads:\n" );
  mostraLDE( lEstoque );
  printf( "------------------------------------\n" );
  printf( "Lista de valores de pedido por atendente:\n" );
  mostraLDP( lPedido );

  pthread_exit(NULL);

  return 0;

}
