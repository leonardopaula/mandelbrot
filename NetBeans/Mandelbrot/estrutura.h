#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <signal.h>
#include <sys/types.h>
#include "xlib.h"

#define COMPRIMENTO_JANELA 1024
#define ALTURA_JANELA 768
#define NUM_TRABALHADORES 8
#define DIVISOR_PIXEL 32 // dependendo do divisor a imagem e desenhada completa ou nao.
#define MANDELBROT_SIZE 4000
#define DESLOCAR_X -0.6
#define DESLOCAR_Y -0.0
#define MAX_ITERACOES 5000

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int x;
    int y;
    float r;
    float g;
    float b;
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

typedef struct glut_t {
    int id_janela;
} glut_t;
typedef glut_t* h_glut;

typedef struct
{
    unsigned char r, g, b;
} rgb_t;

typedef struct
{
    double escala;
    double cx;
    double cy;
} config_mandelbrot_t;
typedef config_mandelbrot_t* config_mandelbrot;

struct dadosCompartilhados
{
    int comprimento;
    int altura;
    int execucao;
    lista sacoDeTarefas;
    lista sacoDeResultados;
    xlib_dados xlib;
    config_mandelbrot cfgMandelbrot;
};

#ifdef __cplusplus
}
#endif

#endif /* ESTRUTURA_H */

