#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "libSOP.h"
#include "utils.h"

char *nomearq;
int  nthr;
int  fimThreads = 0;
int  cheio = 0;
listaPedido  *lPedido;
listaEstoque *lEstoque;
pthread_mutex_t mtxPedido;
pthread_mutex_t mtxFimPedido;
pthread_cond_t  condCheio;
pthread_cond_t  condFim;

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

    int  rc;
    long t;
    pthread_t *tAtendente;
    pthread_t tCaixa;

    if( argc != 3 ){
        printf( "Parametros nao informados!! %i", argc );
        return 0;
    }

    nthr       = atoi( argv[1] );
    nomearq    = argv[2];
    lEstoque   = leArqEstoque( nomearq );
    lPedido    = criarLDP();
    tAtendente = malloc( sizeof( pthread_t ) * nthr );
    tCaixa     = malloc( sizeof( pthread_t ) );

    printf( "\n------------------------------------\nLista antes da execucao das threads:\n" );
    mostraLDE( lEstoque );

    for( t = 0; t < nthr; t++ ){

        rc = pthread_create( tAtendente[ t ], NULL, processaPedido, ( void* ) t );
        if( rc ){
            printf( "ERRO - rc=%d\n", rc );
            exit( -1 );
        }
        printf("\nCriou atendente %i", t );

    }

    printf("\n tentando criar caixa" );
    rc = pthread_create( tCaixa, NULL, (void *)processaCaixa, NULL );
    if( rc ){
        printf( "ERRO - rc=%d\n", rc );
        exit( -1 );
    }
    printf("\nCriou caixa" );

    pthread_mutex_lock( &mtxFimPedido );
    while( fimThreads != nthr ){
        pthread_cond_wait( &condFim, &mtxFimPedido );
    }
    pthread_mutex_unlock( &mtxFimPedido );

    // printf( "\nLista após execucao das threads:\n" );
    // mostraLDE( lEstoque );
    // printf( "------------------------------------\n" );
    // printf( "Lista de valores de pedido por atendente:\n" );
    // mostraLDP( lPedido );

    pthread_exit(NULL);

    return 0;

}
