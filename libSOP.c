#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "libSOP.h"
#include "utils.h"

listaDupla* criarListaDupla(){

	listaDupla* lista = malloc( sizeof( listaDupla ) );

	lista->primeiro   = NULL;
	lista->ultimo     = NULL;

	return lista;

}

noDupla* criarNo( lancheEstoque le ){

	noDupla* no  = malloc( sizeof( noDupla ) );

	no->proximo  = NULL;
	no->anterior = NULL;
	no->le       = le;

	return no;

}

void pushBack( listaDupla* lista, lancheEstoque le ){

  noDupla* n = criarNo( le );
	noDupla* temp = lista->ultimo;

	if(temp != NULL){
		temp->proximo = n;
		n->anterior   = temp;
		lista->ultimo = n;
	}else{
		lista->primeiro = n;
		lista->ultimo   = n;
	}

}

void pushFront( listaDupla* lista, lancheEstoque le ){

  noDupla* n    = criarNo( le );
	noDupla* temp = lista->primeiro;

	if(temp != NULL){
		temp->anterior  = n;
		n->proximo      = temp;
		lista->primeiro = n;
	}else{
		lista->primeiro = n;
		lista->ultimo   = n;
	}

}

int size( listaDupla* lista ){

  noDupla* temp = lista->primeiro;
	int size      = 0;

	while(temp != NULL){
		temp = temp->proximo;
		size++;
	}

	return size;

}

lancheEstoque buscaPorNome( listaDupla* lista, char* nomeBusca ){

	noDupla* no;
  lancheEstoque lancheEncontrado;

	no = lista->primeiro;

	while(no != NULL){
		if( strcmp( no->le.nome, nomeBusca ) == 0 ){
			lancheEncontrado = no->le;
      break;
		}
		no = no->proximo;
	}

	return lancheEncontrado;
}

void mostraLista( listaDupla* lista ){

	noDupla* n;

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

listaDupla* leArqEstoque( char* nomearq ){

  FILE* f;
  int   nLinhas;
  int   i;
	char  cAux[50];
  lancheEstoque leAux;
  listaDupla*   lista;

  lista = criarListaDupla();
  f     = fopen( nomearq, "r" );

  if( f == NULL ){
      printf("\nDeu merda no arquivo!\n");
  }

  nLinhas = contaLinhasArq( nomearq );

  for( i = 0; i < nLinhas; i++ ){
    fscanf( f, "%s\t%u\t%u\n", cAux, &leAux.preco, &leAux.quantidade );
		strcpy( leAux.nome, cAux );
		//printf( fmtestoque, leAux.nome, leAux.preco, leAux.quantidade );
    pushBack( lista, leAux );
  }

  fclose( f );

	mostraLista( lista );

  return lista;

}
