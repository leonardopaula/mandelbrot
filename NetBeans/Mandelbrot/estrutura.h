#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <signal.h>
#include <GL/gl.h>
#include <GL/glut.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int x;
    int y;
} ponto_t;
typedef ponto_t ponto;

typedef struct
{
    ponto inicial;
    ponto final;
} trabalho_t;
typedef trabalho_t* trab;

typedef struct {
    void   *dados;
    void *proximo;
} no_t;
typedef no_t* no;

typedef struct lista_encadeada_t {
    no                  inicial;
    no                    final;
    int                 tamanho;
    int            tamanhoDados; // Define o tamanho dos dados (Trabalho ou Resultado)
    int               computado;
    int         tamanhoTrabalho;
    int               executado;
    pthread_mutex_t       mutex;
    pthread_cond_t   vcProdutor;
    pthread_cond_t vcConsumidor;
} lista_encadeada_t;

typedef lista_encadeada_t* lista;

struct dadosCompartilhados
{
    int comprimento;
    int altura;
    lista sacoDeTarefas;
};

typedef GLfloat gl_cor[3];
typedef GLfloat gl_pt[3];

typedef struct
{
    gl_cor ponto;
    gl_pt  cor;
} resultado_t;
typedef resultado_t* resultado;

typedef struct
{
    resultado *result;
    int         total;
} resultado_trabalho_t;
typedef resultado_trabalho_t* resultado_trabalho;

#ifdef __cplusplus
}
#endif

#endif /* ESTRUTURA_H */

