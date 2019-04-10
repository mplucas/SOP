#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "libSOP.h"
#include "utils.h"

listaEstoque* criarLDE(){

	listaEstoque* lista = malloc( sizeof( listaEstoque ) );

	lista->primeiro   = NULL;
	lista->ultimo     = NULL;

	return lista;

}

noEstoque* criarNoLDE( lancheEstoque le ){

	noEstoque* no  = malloc( sizeof( noEstoque ) );

	no->proximo  = NULL;
	no->anterior = NULL;
	no->le       = le;

	return no;

}

void pushBackLDE( listaEstoque* lista, lancheEstoque le ){

  noEstoque* n = criarNoLDE( le );
	noEstoque* temp = lista->ultimo;

	if(temp != NULL){
		temp->proximo = n;
		n->anterior   = temp;
		lista->ultimo = n;
	}else{
		lista->primeiro = n;
		lista->ultimo   = n;
	}

}

void pushFrontLDE( listaEstoque* lista, lancheEstoque le ){

  noEstoque* n    = criarNoLDE( le );
	noEstoque* temp = lista->primeiro;

	if(temp != NULL){
		temp->anterior  = n;
		n->proximo      = temp;
		lista->primeiro = n;
	}else{
		lista->primeiro = n;
		lista->ultimo   = n;
	}

}

int sizeLDE( listaEstoque* lista ){

  noEstoque* temp = lista->primeiro;
	int sizeLDE     = 0;

	while(temp != NULL){
		temp = temp->proximo;
		sizeLDE++;
	}

	return sizeLDE;

}

noEstoque* buscaPorNomeLDE( listaEstoque* lista, char* nomeBusca ){

	noEstoque *no;

	no = lista->primeiro;

	while(no != NULL){
		if( strcmp( no->le.nome, nomeBusca ) == 0 ){
      		break;
		}
		no = no->proximo;
	}

	return no;
}

void mostraLDE( listaEstoque* lista ){

	noEstoque* n;

	n = lista->primeiro;

	while( n != NULL ){

		printf( fmtestoque, n->le.nome, n->le.preco, n->le.quantidade );
		n = n->proximo;

	}

}


listaPedido* criarLDP(){

	listaPedido* lista = malloc( sizeof( listaPedido ) );

	lista->primeiro   = NULL;
	lista->ultimo     = NULL;

	return lista;

}

noPedido* criarNoLDP( unsigned int atendente, unsigned int valor ){

	lanchePedido lp;
	noPedido* no;

	lp.atendente = atendente;
	lp.valor = valor;
	no  = malloc( sizeof( noPedido ) );
	no->proximo  = NULL;
	no->anterior = NULL;
	no->lp       = lp;

	return no;

}

void pushBackLDP( listaPedido* lista, unsigned int atendente, unsigned int valor ){

  noPedido* n = criarNoLDP( atendente, valor );
	noPedido* temp = lista->ultimo;

	if(temp != NULL){
		temp->proximo = n;
		n->anterior   = temp;
		lista->ultimo = n;
	}else{
		lista->primeiro = n;
		lista->ultimo   = n;
	}

}

void popBackLDP( listaPedido* lista ){

	noPedido* ultimo = lista->ultimo;

	if(ultimo != NULL){
		ultimo->anterior->proximo = NULL;
		free( ultimo );
	}

}

int sizeLDP( listaPedido* lista ){

  noPedido* temp = lista->primeiro;
	int sizeLDP    = 0;

	while(temp != NULL){
		temp = temp->proximo;
		sizeLDP++;
	}

	return sizeLDP;

}

void mostraLDP( listaPedido* lista ){

	noPedido* n;

	n = lista->primeiro;

	while( n != NULL ){

		printf( "%7u   %10u\n", n->lp.atendente, n->lp.valor );
		n = n->proximo;

	}

}

int contaLinhasArq( char* nomearq ){

  FILE* f;
  char  c;
  int   nLinhas = 0;

  // Open the file
  f = fopen( nomearq, "r" );

  // Check if file exists
  if (f == NULL)
  {
      printf("Could not open file %s", nomearq);
      return 0;
  }

  // Extract characters from file and store in character c
  for (c = getc(f); c != EOF; c = getc(f))
      if (c == '\n') // Increment count if this character is newline
          nLinhas = nLinhas + 1;

  // Close the file
  fclose(f);

  return nLinhas;

}

char* leNome( char buffer[1000] ){

	char* result;

	result = malloc( sizeof( char ) * strlen( buffer ) );
	strcpy( result, buffer );

	return result;

}

listaEstoque* leArqEstoque( char* nomearq ){

  FILE* f;
  int   nLinhas;
  int   i;
  char  cAux[1000];
  lancheEstoque leAux;
  listaEstoque*   lista;

  lista = criarLDE();
  f     = fopen( nomearq, "r" );

  if( f == NULL ){
      printf("\nErro na abertura do arquivo!\n");
  }

  nLinhas = contaLinhasArq( nomearq );

  for( i = 0; i < nLinhas; i++ ){
    fscanf( f, "%s\t%u\t%u\n", cAux, &leAux.preco, &leAux.quantidade );
		leAux.nome = leNome( cAux );
		//printf( fmtestoque, cAux, leAux.preco, leAux.quantidade );
    pushBackLDE( lista, leAux );
  }

  fclose( f );

  //mostraLDE( lista );

  return lista;

}

void *PrintHello(void *arg){

	long tid = (long)arg;
	printf("Alo da thread %ld\n",tid);
	pthread_exit(NULL);

}

/* sinaliza_thread_pronta_e_espera_demais();
enquanto (houver pedidos a processar) {
	pedido = le_prox_linha_arq_pedidos();
	se (pedido puder ser atendido) {
		retira_lanches_estoque();
		enfileira_caixa(id, valor);
	}
} */

void *processaPedido( void *arg ){

	long  tid;
	char  cAux[1000];
	char* nomeArqPed;
	char* nomePed;
	int   nLinhas;
	int   i;
	int   quantPed;
	FILE* f;
	noEstoque* noAux;

	tid = ( long )arg;
	sprintf( cAux, "%li", tid + 1 );
	nomeArqPed = malloc( sizeof( char ) * ( strlen( nomearq ) + strlen( cAux ) + 1 ) );
	strcpy( nomeArqPed, nomearq );
	strcat( nomeArqPed, "-" );
	strcat( nomeArqPed, cAux );
	f = fopen( nomeArqPed, "r" );

  if( f == NULL ){
      printf("\nErro na abertura do arquivo!\n");
  }

	nLinhas = contaLinhasArq( nomeArqPed );

  for( i = 0; i < nLinhas; i++ ){

    fscanf( f, "%s\t%u\n", cAux, &quantPed );
		nomePed = leNome( cAux );
		noAux = buscaPorNomeLDE( lEstoque, nomePed );

		if( ( noAux != NULL ) && ( noAux->le.quantidade >= quantPed ) ){

			pthread_mutex_lock( &mtxPedido );
			// retira produtos do estoque
			noAux->le.quantidade -= quantPed;
			// adiciona pedido na fila para processamento do caixa
			pushBackLDP( lPedido, tid + 1, noAux->le.preco * quantPed );
			pthread_mutex_unlock( &mtxPedido );

		}

  }

	pthread_mutex_lock( &mtxFimPedido );
	fimThreads++;
	pthread_cond_signal( &condFim );
	pthread_mutex_unlock( &mtxFimPedido );

  fclose( f );

}

void *processaCaixa( void *arg ){
	printf( "\nEntrou em caixa.\n" );

	// matriz para mostrar as informacoes do relatorio financeiro
	// [x][0] - Atendente
	// [x][1] - Pedidos
	// [x][2] - Valor
	int **relatorio;
	int i;
	noPedido *noAux;

	printf( "\nEntrou em caixa.\n" );
	// incializa matriz para guardar valores do relatorio
	relatorio = malloc( sizeof( int* ) * nthr );
	for( i = 0; i < nthr; i++ ){
		relatorio[ i ] = malloc( sizeof( int ) * 3 );
		relatorio[ i ][ 0 ] = 0;
		relatorio[ i ][ 1 ] = 0;
		relatorio[ i ][ 2 ] = 0;
	}
	printf( "\n Alocado matriz de relatorio[%i][3].\n", nthr );

	while( fimThreads != nthr ){

		pthread_mutex_lock( &mtxPedido );

		if( sizeLDP( lPedido ) != 0 ){

			noAux = lPedido->ultimo;
			relatorio[ noAux->lp.atendente - 1 ][ 0 ] = noAux->lp.atendente;
			relatorio[ noAux->lp.atendente - 1 ][ 1 ]++;
			relatorio[ noAux->lp.atendente - 1 ][ 2 ] += noAux->lp.valor;
			popBackLDP( lPedido );

		}

		pthread_mutex_unlock( &mtxPedido );

	}

}
