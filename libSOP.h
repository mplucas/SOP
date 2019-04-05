// Structs

typedef struct le
{

  char     *nome;
  unsigned int preco;
  unsigned int quantidade;

} lancheEstoque;

typedef struct nd
{
	lancheEstoque le;
	struct nd*    proximo;
	struct nd*    anterior;
} noDupla;

typedef struct ld
{
	noDupla* primeiro;
	noDupla* ultimo;
} listaDupla;

// Funcoes

listaDupla* criarListaDupla();
void pushBack( listaDupla* lista, lancheEstoque le );
void pushFront( listaDupla* lista, lancheEstoque le );
int size( listaDupla* lista );
noDupla* buscaPorNome( listaDupla* lista, char* nomeBusca );
listaDupla* leArqEstoque( char* nomearq );
void *PrintHello(void *arg);
void mostraLista( listaDupla* lista );
void *processaPredido( void *arg );
