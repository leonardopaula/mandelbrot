#ifndef LISTAENCADEADA_H
#define LISTAENCADEADA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "estrutura.h"


#ifdef __cplusplus
extern "C" {
#endif

lista inicia_le(int tamanhoDados, int tamanhoTrabalho);
void reinicia_le(const lista l);
no aloca_no(const lista l, const void *dados);
void adiciona_le(const lista l, const void *dados);
void *remove_le(const lista l);


#ifdef __cplusplus
}
#endif

#endif /* LISTAENCADEADA_H */

