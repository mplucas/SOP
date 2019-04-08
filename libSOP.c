#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "libSOP.h"
#include "utils.h"

listaEstoque* criarListaDupla(){

	listaEstoque* lista = malloc( sizeof( listaEstoque ) );

	lista->primeiro   = NULL;
	lista->ultimo     = NULL;

	return lista;

}

noEstoque* criarNo( lancheEstoque le ){

	noEstoque* no  = malloc( sizeof( noEstoque ) );

	no->proximo  = NULL;
	no->anterior = NULL;
	no->le       = le;

	return no;

}

void pushBack( listaEstoque* lista, lancheEstoque le ){

  noEstoque* n = criarNo( le );
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

void pushFront( listaEstoque* lista, lancheEstoque le ){

  noEstoque* n    = criarNo( le );
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

int size( listaEstoque* lista ){

  noEstoque* temp = lista->primeiro;
	int size      = 0;

	while(temp != NULL){
		temp = temp->proximo;
		size++;
	}

	return size;

}

noEstoque* buscaPorNome( listaEstoque* lista, char* nomeBusca ){

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

void mostraLista( listaEstoque* lista ){

	noEstoque* n;

	n = lista->primeiro;

	while( n != NULL ){

		printf( fmtestoque, n->le.nome, n->le.preco, n->le.quantidade );
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

  lista = criarListaDupla();
  f     = fopen( nomearq, "r" );

  if( f == NULL ){
      printf("\nErro na abertura do arquivo!\n");
  }

  nLinhas = contaLinhasArq( nomearq );

  for( i = 0; i < nLinhas; i++ ){
    fscanf( f, "%s\t%u\t%u\n", cAux, &leAux.preco, &leAux.quantidade );
		leAux.nome = leNome( cAux );
		//printf( fmtestoque, cAux, leAux.preco, leAux.quantidade );
    pushBack( lista, leAux );
  }

  fclose( f );

  //mostraLista( lista );

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

void *processaPredido( void *arg ){

	long  tid;
	char  cAux[1000];
	char* nomeArqPed;
	char* nomePed;
	int   nLinhas;
	int   i;
	int   quantPed;
	FILE* f;
	noEstoque* noAux;

	tid = (long)arg;
	sprintf( cAux, "%i", tid + 1 );
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
		noAux = buscaPorNome( listaLE, nomePed );

		if( ( noAux != NULL ) && ( noAux->le.quantidade >= quantPed ) ){

			pthread_mutex_lock( &m );
			noAux->le.quantidade -= quantPed;
			pthread_mutex_unlock( &m );

		}

  }

	fimThreads++;
	pthread_cond_signal( &condFim );

  fclose( f );

}
