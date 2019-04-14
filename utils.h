/* strings de formato */
#define fmtfinanc "%9u   %7lu   %10lu\n"
#define fmtestoque "%-15s   %7u   %8u\n"

extern char *nomearq;
extern int  nthr;
extern int  fimThreads;
extern listaPedido  *lPedido;
extern listaEstoque *lEstoque;
extern pthread_mutex_t mtxPedido;
extern pthread_mutex_t mtxFimPedido;
extern pthread_cond_t  condCheio;
