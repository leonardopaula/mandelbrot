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
#include "threads.h"
#include "glut.h"


#define COMPRIMENTO_JANELA 800
#define ALTURA_JANELA 600
#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

struct dadosCompartilhados dc;
h_glut oGL = NULL;

void * trabalhador(void *str);
void * desenhista(void *str);
pthread_t threads[NUM_TRABALHADORES + 1]; // + Thread que imprime
void divide_trabalhos();
void display(void);
void mouse(int button, int state, int x, int y);
void draw(int x, int y, float r, float g, float b);
//void* draw(int x, int y, float r, float g, float b);
void drawObj(ponto_t p);

int tex_w, tex_h;
rgb_t **tex = 0;
int gwin;

ponto_t point;

int main(int argc, char *argv[]) {
    dc.comprimento = COMPRIMENTO_JANELA;
    dc.altura = ALTURA_JANELA;
    int tamanhoTrabalho = (int) dc.comprimento / DIVISOR_PIXEL * (int) dc.altura / DIVISOR_PIXEL;

    dc.sacoDeTarefas    = inicia_le(sizeof (trabalho_t), tamanhoTrabalho);
    dc.sacoDeResultados = inicia_le(sizeof (ponto_t), dc.comprimento*dc.altura);

    oGL = malloc(sizeof(glut_t));
    memset(oGL, 0, sizeof(glut_t));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(COMPRIMENTO_JANELA, ALTURA_JANELA);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    oGL->id_janela = glutCreateWindow("Fractal de Mandelbrot");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT); // clear display window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();    

    printf("Fim do programa");
}

void divide_trabalhos() {
    printf("Dividindo...");
    int tam_x = (int) dc.comprimento / DIVISOR_PIXEL;
    int tam_y = (int) dc.altura / DIVISOR_PIXEL;

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

void alloc_tex() {
    int i;
    int ow = tex_w; //text width
    int oh = tex_h; // text height

    for (tex_w = 1; tex_w < COMPRIMENTO_JANELA; tex_w <<= 1);
    for (tex_h = 1; tex_h < ALTURA_JANELA; tex_h <<= 1);

    if (tex_h != oh || tex_w != ow) {
        tex = realloc(tex, tex_h * tex_w * 3 + tex_h * sizeof (rgb_t *));
    }

    for (tex[0] = (rgb_t *) (tex + tex_h), i = 1; i < tex_h; i++)
        tex[i] = tex[i - 1] + tex_w;
}


void rodar()
{
    // Lança as threads (workers)
    for (int i = 0; i <= NUM_TRABALHADORES; i++)
    {
        if (i == NUM_TRABALHADORES){
            pthread_create(&threads[i], NULL, &desenhista, &dc);
        } else
            pthread_create(&threads[i], NULL, &trabalhador, &dc);
    }

    divide_trabalhos();

    for (int i = 0; i <= NUM_TRABALHADORES; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Join %d \n", i);
    }
}

void opa()
{
    draw(1, 2, 1.0, 0.0, 0.0);
}