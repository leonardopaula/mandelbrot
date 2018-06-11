#include "listaEncadeada.h"

lista inicia_le(int tamanhoDados, int tamanhoTrabalho)
{
    int tamanho = sizeof(lista_encadeada_t);
    lista l = (lista) malloc(tamanho);

    memset(l, 0, tamanho);
    l->inicial = NULL;
    l->final   = NULL;
    
    pthread_mutex_init(&l->mutex, NULL);
    pthread_cond_init(&l->vcProdutor, NULL);
    pthread_cond_init(&l->vcConsumidor, NULL);

    l->tamanho = 0;
    l->tamanhoDados = tamanho;
    l->tamanhoTrabalho = tamanhoTrabalho;

    return l;
}

no aloca_no(const lista l, const void *dados)
{
    no n = (no) malloc(sizeof(no_t));
    memset(n, 0, sizeof(no_t));

    n->dados = malloc(l->tamanhoDados);
    memcpy(n->dados, dados, l->tamanhoDados);
    n->proximo = NULL;

    return n;
}

void adiciona_le(const lista l, const void *dados)
{
    no n;
    
    pthread_mutex_lock(&l->mutex);

    n = aloca_no(l, dados);
    
    if (l->tamanho == 0)
    {
        l->inicial = n;
        l->final = n;
    } else {
        l->final->proximo = n;
        l->final = n;
    }
        
    l->tamanho++;
    
    // Produzimos algo, hora de acordar os trabalhadores.
    pthread_cond_signal(&l->vcConsumidor);

    pthread_mutex_unlock(&l->mutex);
    
}

void *remove_le(const lista l)
{
    pthread_mutex_lock(&l->mutex);
    
    // While the buffer is empty, the consumer will sleep.
    while (l->tamanho == 0 && (l->tamanhoTrabalho > l->computado))
        pthread_cond_wait(&l->vcConsumidor, &l->mutex);
    
    if (l->tamanhoTrabalho > l->computado)
    {
        // Tira o primeiro da Lista (FIFO)
        void *dados = l->inicial->dados;
        l->inicial = l->inicial->proximo; // Próximo
        l->tamanho--;
        l->computado++;
        // Se o produtor estiver precisando de memória...
        pthread_cond_signal(&l->vcProdutor);
        pthread_mutex_unlock(&l->mutex);

        return dados;
    }
    pthread_cond_signal(&l->vcProdutor);
    pthread_cond_signal(&l->vcConsumidor);
    pthread_mutex_unlock(&l->mutex);
    
    return NULL;
    
    
}
