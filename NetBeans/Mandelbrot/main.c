#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glut.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"


#define COMPRIMENTO_JANELA 800
#define ALTURA_JANELA 600
#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

struct dadosCompartilhados dc;

void * trabalhador(void *str);
pthread_t threads[NUM_TRABALHADORES + 1]; // + Thread que imprime
void divide_trabalhos();

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char *argv[])
{
    dc.comprimento = COMPRIMENTO_JANELA;
    dc.altura      = ALTURA_JANELA;
    int tamanhoTrabalho = (int) dc.comprimento / DIVISOR_PIXEL * (int) dc.altura / DIVISOR_PIXEL;

    dc.sacoDeTarefas = inicia_le(sizeof(trabalho_t), tamanhoTrabalho);

    // Lança as threads (workers)
    for(int i = 0; i < NUM_TRABALHADORES; i++)
	pthread_create(&threads[i], NULL, &trabalhador, &dc);

    divide_trabalhos();

    for(int i = 0; i < NUM_TRABALHADORES; i++)
	pthread_join(threads[i], NULL);

    printf("Fim do programa");
}

void * trabalhador(void *arg) 
{
    int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado)
    {
        trab t;
        t = remove_le(dc->sacoDeTarefas);
        
        // Calcula o Mandelbrot

    }
    printf("Acabei! e fiz: %d\n", executado);
}

void divide_trabalhos()
{
    int tam_x       = (int) dc.comprimento / DIVISOR_PIXEL;
    int tam_y       = (int) dc.altura / DIVISOR_PIXEL;
    trabalho_t* t;

    // Tratar o resto da divisão, caso haja
    int indice = 0;
    for(int j = 0; j < tam_y; j++)
    {
	for(int i = 0; i < tam_x; i++)
	{
            t = (trabalho_t *) malloc(sizeof(trabalho_t));
            memset(t, 0, sizeof(trabalho_t));
            t->inicial.x = i*DIVISOR_PIXEL;
            t->inicial.y = j*DIVISOR_PIXEL;
            t->final.x = i*DIVISOR_PIXEL + DIVISOR_PIXEL;
            t->final.y = j*DIVISOR_PIXEL + DIVISOR_PIXEL;
            
            adiciona_le(dc.sacoDeTarefas, t);
	}
    }
}