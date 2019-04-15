#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "libSOP.h"
#include "utils.h"

listaEstoque* criarLDE(){

	listaEstoque* lista = malloc( sizeof( listaEstoque ) );

	lista->primeiro = NULL;
	lista->ultimo   = NULL;

	return lista;

}

noEstoque* criarNoLDE( lancheEstoque le ){

	noEstoque* no = malloc( sizeof( noEstoque ) );

	no->proximo   = NULL;
	no->anterior  = NULL;
	no->le        = le;

	return no;

}

void pushBackLDE( listaEstoque* lista, lancheEstoque le ){

	noEstoque* n    = criarNoLDE( le );
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

		printf( "\n%s\t%u\t%u\t%u", n->le.nome, n->le.preco, n->le.quantidade, n->le.quantidadeInicial );
		n = n->proximo;

	}

}


listaPedido* criarLDP(){

	listaPedido* lista = malloc( sizeof( listaPedido ) );

	lista->primeiro    = NULL;
	lista->ultimo      = NULL;

	return lista;

}

noPedido* criarNoLDP( unsigned int atendente, unsigned int valor ){

	lanchePedido lp;
	noPedido*    no;

	lp.atendente = atendente;
	lp.valor     = valor;
	no           = malloc( sizeof( noPedido ) );
	no->proximo  = NULL;
	no->anterior = NULL;
	no->lp       = lp;

	return no;

}

void pushBackLDP( listaPedido* lista, unsigned int atendente, unsigned int valor ){

	noPedido* n    = criarNoLDP( atendente, valor );
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

void popFrontLDP( listaPedido* lista ){

	noPedido* primeiro = lista->primeiro;
	noPedido* segundo;

	if( primeiro != NULL ){
		segundo = primeiro->proximo;
		if( segundo != NULL ){
			segundo->anterior = NULL;
		}else{
			lista->ultimo = NULL;
		}
		lista->primeiro = segundo;
		free( primeiro );
	}

}

int sizeLDP( listaPedido* lista ){

  noPedido* temp = lista->primeiro;
	int sizeLDP = 0;

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
	listaEstoque* lista;

	lista = criarLDE();
	f     = fopen( nomearq, "r" );

	if( f == NULL ){
		printf("\nErro na abertura do arquivo!\n");
	}

	nLinhas = contaLinhasArq( nomearq );

	for( i = 0; i < nLinhas; i++ ){
		fscanf( f, "%s\t%u\t%u\n", cAux, &leAux.preco, &leAux.quantidade );
		leAux.nome = leNome( cAux );
		leAux.quantidadeInicial = leAux.quantidade;
		pushBackLDE( lista, leAux );
	}

	fclose( f );

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

	// Espera barreira para que threads comecem ao mesmo tempo
	pthread_barrier_wait( &barreira );

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
		noAux   = buscaPorNomeLDE( lEstoque, nomePed );

		if( ( noAux != NULL ) && ( noAux->le.quantidade >= quantPed ) ){

			// protecao da variavel global lPedido que contem os pedidos a serem
			// processados pelo caixa
			pthread_mutex_lock( &mtxPedido );

			// processamento
			// retira produtos do estoque
			noAux->le.quantidade -= quantPed;
			// adiciona pedido na fila para processamento do caixa
			pushBackLDP( lPedido, tid + 1, noAux->le.preco * quantPed );

			// Sinaliza thread caixa que há pedidos a serem processados
			proximoPedido = lPedido->primeiro;
			//signal
			pthread_cond_signal( &condPedido );

			pthread_mutex_unlock( &mtxPedido );

		}

	}

	// verifica se todas as threads de atendente terminaram para adicionar um
	// pedido marcado na lista de pedidos processada pelo caixa, nesse caso a
	// ultima thread de atendente a ser processada marca o final da lista de
	// pedidos
	pthread_mutex_lock( &mtxFimPedido );
	fimThreads++;
	if( fimThreads == nthr ){
		pthread_mutex_lock( &mtxPedido );
		pushBackLDP( lPedido, 0, 0 );
		pthread_mutex_unlock( &mtxPedido );
	}
	pthread_mutex_unlock( &mtxFimPedido );

	fclose( f );

}


void printRelatorio( int** m ){

	int i;

	printf( "\n" );
	for( i = 0; i < nthr; i++ ){
		printf( fmtfinanc, m[ i ][ 0 ], m[ i ][ 1 ], m[ i ][ 2 ] );
	}

}

void *processaCaixa( void *arg ){

	// Espera barreira para que threads comecem ao mesmo tempo
	pthread_barrier_wait( &barreira );

	// matriz para mostrar as informacoes do relatorio financeiro
	// [x][0] - Atendente
	// [x][1] - Pedidos
	// [x][2] - Valor
	int **relatorio;
	int i;
	int idAtendente = -1;
	unsigned int receitaTotal = 0;

	// incializa matriz para guardar valores do relatorio
	relatorio = malloc( sizeof( int* ) * nthr );
	for( i = 0; i < nthr; i++ ){
		relatorio[ i ] = malloc( sizeof( int ) * 3 );
		relatorio[ i ][ 0 ] = 0;
		relatorio[ i ][ 1 ] = 0;
		relatorio[ i ][ 2 ] = 0;
	}

	// condicao que verifica se a fila chegou ao nó marcado pela ultima threads
	// de atendente processada, desse modo se conclui que não há mais pedidos
	while( idAtendente != 0 ){

		// protecao da variavel global lPedido que contem os pedidos a serem
		// processados pelo caixa
		pthread_mutex_lock( &mtxPedido );

		if( proximoPedido != NULL ){

			idAtendente = proximoPedido->lp.atendente;
			if( idAtendente != 0 ){
				relatorio[ proximoPedido->lp.atendente - 1 ][ 0 ] = idAtendente;
				relatorio[ proximoPedido->lp.atendente - 1 ][ 1 ]++;
				relatorio[ proximoPedido->lp.atendente - 1 ][ 2 ] += proximoPedido->lp.valor;
				receitaTotal += proximoPedido->lp.valor;
				popFrontLDP( lPedido );
				proximoPedido = lPedido->primeiro;
			}

		}else{
			//espera alguma thread de atendente realizar um pedido
			//wait
			pthread_cond_wait( &condPedido, &mtxFimPedido );
		}

		pthread_mutex_unlock( &mtxPedido );

	}

	// mostra relatorio financeiro
	printf( "\n***** Relatorio financeiro *****\n" );
	printf("\nAtendente   Pedidos   Valor (R$)" );
	printRelatorio( relatorio );

	// retorna valor da receita total obtida para a main
	pthread_exit( receitaTotal );

}

void printDiffEstoque( listaEstoque *lista ){

	noEstoque* n;

	n = lista->primeiro;

	while( n != NULL ){

		printf( fmtestoque, n->le.nome, n->le.quantidadeInicial, n->le.quantidade );
		n = n->proximo;

	}

}
