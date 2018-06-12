#ifndef THREADS_H
#define THREADS_H

#ifdef __cplusplus
extern "C" {
#endif

void * trabalhador(void *arg);
void divide_trabalhos();


#ifdef __cplusplus
}
#endif

#endif /* THREADS_H */

