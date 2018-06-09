#include "lista_encadeada.h"

lista inicia_le(int tamanhoDados)
{
	int tamanho = sizeof(ListaEncadeada);
	lista l = (lista) malloc(tamanho);

	memset(l, 0, tamanho);
	l->inicial = NULL;
	l->tamanho = 0;
	l->tamanhoDados = tamanho;

	return l;
}

no aloca_no(const lista l, const void *dados)
{
	no n = (no) malloc(sizeof(No));
	memset(n, 0, sizeof(No));

	n->dados = malloc(l->tamanho);
	memcpy(n->dados, dados, l->tamanhoDados);
    n->proximo = NULL;

	return n;
}

int adiciona_le(const lista l, const void *dados)
{
	no n = aloca_no(l, dados);
	l->tamanho++;
}

void *remove_le(const lista l)
{
//    pthread_mutex_lock(&d->mutex);
    
    // While the buffer is empty, the consumer will sleep.
    //while (d->count == 0)
//        pthread_cond_wait(&d->condConsumer, &d->mutex);
    
    // Tira o primeiro da Lista (FIFO)
    printf("%d\n", l->inicial->dados);
    void *dados = l->inicial->dados;
    //l->inicial = l->inicial->proximo; // Próximo
    //l->tamanho--;
    
    // Notify a producer if it is waiting for space
	// pthread_cond_signal(&d->condProducer);
	// phread_mutex_unlock(&d->mutex);
    
    return dados;
}
