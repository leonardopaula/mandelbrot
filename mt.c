#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "estruturas.h"

#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

Dados_compartilhados dc;
void * trabalhador(void *str);
pthread_t threads[NUM_TRABALHADORES + 1]; // + Thread que imprime
pthread_mutex_t mut;

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char *argv[])
{
	int comprimento = atoi(argv[1]);
	int altura      = atoi(argv[2]);
	int tam_x       = (int) comprimento / DIVISOR_PIXEL;
	int tam_y       = (int) altura / DIVISOR_PIXEL;

	dc.t = (Trabalho *)malloc((tam_x * tam_y) * sizeof(Trabalho));

	pthread_mutex_init(&mut, NULL);

	// Lança as threads (workers)
	for(int i = 0; i < NUM_TRABALHADORES; i++)
		pthread_create(&threads[i], NULL, trabalhador, &dc);

	for(int i = 0; i < NUM_TRABALHADORES; i++)
		pthread_join(threads[i], NULL);


	int indice = 0;
	for(int j = 0; j < tam_y; j++)
	{
		for(int i = 0; i < tam_x; i++)
		{
			dc.t[indice++].inicial.x = i*DIVISOR_PIXEL;
			dc.t[indice++].inicial.y = j*DIVISOR_PIXEL;

			dc.t[indice++].final.x = i*DIVISOR_PIXEL + DIVISOR_PIXEL;
			dc.t[indice++].final.y = j*DIVISOR_PIXEL + DIVISOR_PIXEL;

			indice++;
			//printf("%d x %d : ", i*DIVISOR_PIXEL, j*DIVISOR_PIXEL);
			//printf(" %d x %d\n", i*DIVISOR_PIXEL + DIVISOR_PIXEL, j*DIVISOR_PIXEL + DIVISOR_PIXEL);
		}
	}
	// Mestre (main thread) gera os trabalhos (x,y inicial/final)
	//printf("%d x %d\n", comprimento, altura);
}

void * trabalhador(void *arg) 
{
	ptr_thread_arg targ = (ptr_thread_arg) arg;
	
	printf("Tem algo? %d x %d\n", arg);
}