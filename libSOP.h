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
	noPedido* primeiro;
	noPedido* ultimo;
} listaPedido;

// Funcoes

listaEstoque* leArqEstoque( char* nomearq );
void mostraLDE( listaEstoque* lista );
void *processaPedido( void *arg );
listaPedido* criarLDP();
void mostraLDP( listaPedido* lista );
void *processaCaixa( void *arg );
