// Structs
// Estoque
typedef struct le
{
  char *nome;
  unsigned int preco;
  unsigned int quantidade;
} lancheEstoque;

typedef struct ne
{
	lancheEstoque le;
	struct ne*    proximo;
	struct ne*    anterior;
} noEstoque;

typedef struct lde
{
	noEstoque* primeiro;
	noEstoque* ultimo;
} listaEstoque;

// Pedidos
typedef struct lp
{
  unsigned int atendente;
  unsigned int valor;
} lanchePedido;

typedef struct np
{
	lanchePedido lp;
	struct ne*   proximo;
	struct ne*   anterior;
} noPedido;

typedef struct ldp
{
	noEstoque* primeiro;
	noEstoque* ultimo;
} listaPedido;

// Funcoes

listaEstoque* criarListaDupla();
void pushBack( listaEstoque* lista, lancheEstoque le );
void pushFront( listaEstoque* lista, lancheEstoque le );
int size( listaEstoque* lista );
noEstoque* buscaPorNome( listaEstoque* lista, char* nomeBusca );
listaEstoque* leArqEstoque( char* nomearq );
void *PrintHello(void *arg);
void mostraLista( listaEstoque* lista );
void *processaPredido( void *arg );
