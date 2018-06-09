#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "estruturas.h"
#include "lista_encadeada.c"

#define COMPRIMENTO_JANELA 800
#define ALTURA_JANELA 600
#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

struct dadosCompartilhados dc;

void * trabalhador(void *str);
pthread_t threads[NUM_TRABALHADORES + 1]; // + Thread que imprime
pthread_mutex_t mut;
void divide_trabalhos();

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char *argv[])
{
	dc.comprimento = COMPRIMENTO_JANELA;
	dc.altura      = ALTURA_JANELA;

	dc.sacoDeTarefas = inicia_le(sizeof(Trabalho));

	// Lança as threads (workers)
	for(int i = 0; i < NUM_TRABALHADORES; i++)
		pthread_create(&threads[i], NULL, &trabalhador, &dc);

	divide_trabalhos();

	for(int i = 0; i < NUM_TRABALHADORES; i++)
		pthread_join(threads[i], NULL);

	// Mestre (main thread) gera os trabalhos (x,y inicial/final)
	//printf("%d x %d\n", comprimento, altura);
}

void * trabalhador(void *arg) 
{
	struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;
	
	printf("Opa => %d\n", dc->sacoDeTarefas->tamanho);

	if (dc->sacoDeTarefas->tamanho > 0)
	{
		printf("-> Opa => %d\n", dc->sacoDeTarefas->tamanho);
		trab t;
		t = remove_le(dc->sacoDeTarefas);
		//printf("Opa => %d\n", dc->sacoDeTarefas->tamanho);
	}

    //printf("Tem algo? %d x %d\n", t->inicial.x, t->inicial.y);

    
}

void divide_trabalhos()
{
	int tam_x       = (int) dc.comprimento / DIVISOR_PIXEL;
	int tam_y       = (int) dc.altura / DIVISOR_PIXEL;
	Trabalho* t;

	// Tratar o resto da divisão, caso haja
	int indice = 0;
	for(int j = 0; j < tam_y; j++)
	{
		for(int i = 0; i < tam_x; i++)
		{
			t = (Trabalho *) malloc(sizeof(Trabalho));
			memset(t, 0, sizeof(Trabalho));

			t->inicial.x = i*DIVISOR_PIXEL;
			t->inicial.y = j*DIVISOR_PIXEL;

			t->final.x = i*DIVISOR_PIXEL + DIVISOR_PIXEL;
			t->final.y = j*DIVISOR_PIXEL + DIVISOR_PIXEL;

			adiciona_le(dc.sacoDeTarefas, t);
		}
	}
}