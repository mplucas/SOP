#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "libSOP.h"
#include "utils.h"

char *nomearq;
int  nthr;
int  fimThreads = 0;
noPedido* proximoPedido = NULL;
listaPedido  *lPedido;
listaEstoque *lEstoque;
pthread_mutex_t mtxPedido;
pthread_mutex_t mtxFimPedido;
pthread_cond_t  condPedido;
pthread_barrier_t barreira;

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

    int   rc;
    long  t;
    long  receitaTotal;
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

    // cira barreira para que as threads inciem juntas
    rc = pthread_barrier_init( &barreira, NULL, nthr + 1 );
    if (rc != 0) {
        printf( "ERRO barreira - rc=%d\n", rc );
        exit(1);
    }

    // cria threads de atendentes
    for( t = 0; t < nthr; t++ ){

        rc = pthread_create( tAtendente[ t ], NULL, processaPedido, ( void* ) t );
        if( rc ){
            printf( "ERRO - rc=%d\n", rc );
            exit( -1 );
        }

    }

    // cria thread de caixa
    rc = pthread_create( &tCaixa, NULL, (void *)processaCaixa, NULL );
    if( rc ){
        printf( "ERRO - rc=%d\n", rc );
        exit( -1 );
    }

    // recupera valor da thread caixa
    pthread_join( tCaixa, &receitaTotal );

    // imprime receita total
    printf("\nReceita total: R$ %li\n", receitaTotal );

    // imprime estoque de lanches inicial e final
    printf( "\n***** Estoque de lanches *****\n" );
    printf( "\nLanche            Inicial      Final\n" );
    printDiffEstoque( lEstoque );
    printf( "\n" );

    pthread_exit(NULL);

    return 0;

}
