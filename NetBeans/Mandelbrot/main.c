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
void display();

int tex_w, tex_h;
rgb_t **tex = 0;
int gwin;

void iniciarGlut(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(COMPRIMENTO_JANELA, ALTURA_JANELA);
    gwin = glutCreateWindow("Mandelbrot");
    glutDisplayFunc(display);
    glutMainLoop();
}

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
    
    iniciarGlut(argc, argv);
    
    printf("Fim do programa");
}

void display() { 
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       /* drawing commands would go here, if we had any yet... */
    glutSwapBuffers();
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
            memset(t, 0, sizeof(trabalho_t)); //aloca esta parte da memoria para esta tarefa
            t->inicial.x = i * DIVISOR_PIXEL;
            t->inicial.y = j * DIVISOR_PIXEL;
            t->final.x = i * DIVISOR_PIXEL + DIVISOR_PIXEL;
            t->final.y = j * DIVISOR_PIXEL + DIVISOR_PIXEL;
            
            adiciona_le(dc.sacoDeTarefas, t);
	}
    }
}

void alloc_tex()
{
	int i;
	int ow = tex_w; //text width
	int oh = tex_h; // text height

	for (tex_w = 1; tex_w < COMPRIMENTO_JANELA; tex_w <<= 1);
	for (tex_h = 1; tex_h < ALTURA_JANELA; tex_h <<= 1);

	if (tex_h != oh || tex_w != ow)
	{
		tex = realloc(tex, tex_h * tex_w * 3 + tex_h * sizeof(rgb_t *));
	}

	for (tex[0] = (rgb_t *)(tex + tex_h), i = 1; i < tex_h; i++)
		tex[i] = tex[i - 1] + tex_w;
}