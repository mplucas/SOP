// Structs
// Estoque
typedef struct le
{
  char *nome;
  unsigned int preco;
  unsigned int quantidade;
  unsigned int quantidadeInicial;
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
	struct np*   proximo;
	struct np*   anterior;
} noPedido;

typedef struct ldp
{
	noPedido* primeiro;
	noPedido* ultimo;
} listaPedido;

// Funcoes

listaEstoque* leArqEstoque( char* nomearq );
void *processaPedido( void *arg );
listaPedido* criarLDP();
void *processaCaixa( void *arg );
void printDiffEstoque( listaEstoque *le );
