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
void display(void);
void mouse(int button, int state, int x, int y);
void draw(int x, int y, float r, float g, float b);
void drawObj(ponto_t p);

int tex_w, tex_h;
rgb_t **tex = 0;
int gwin;

ponto_t point;

void display(void) {
    //glClearColor(1.0, 1.0, 1.0, 0.0);
    //glClear(GL_COLOR_BUFFER_BIT); // clear display window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet(GLUT_WINDOW_WIDTH);
    const double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);

    //glPointSize(5.0f);
    glBegin(GL_POINTS);
    int x = point.x;
    int y = point.y;

    glColor3f(point.r, point.g, point.b);
    glVertex2i(x, h - y);

    glEnd();

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        point.x = x;
        point.y = y;
        point.r = 0.0;
        point.g = 1.0;
        point.b = 0.0;
    }
    glutPostRedisplay();
}

void draw(int x, int y, float r, float g, float b) {
    point.x = x;
    point.y = y;
    point.r = r;
    point.g = g;
    point.b = b;
    display();
    //printf("Desnhei");
}

void drawObj(ponto_t p) {
    point = p;
    display();
    //printf("Desnhei");
}

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char *argv[]) {
    dc.comprimento = COMPRIMENTO_JANELA;
    dc.altura = ALTURA_JANELA;
    int tamanhoTrabalho = (int) dc.comprimento / DIVISOR_PIXEL * (int) dc.altura / DIVISOR_PIXEL;

    dc.sacoDeTarefas = inicia_le(sizeof (trabalho_t), tamanhoTrabalho);

    // Lança as threads (workers)
    for (int i = 0; i < NUM_TRABALHADORES; i++)
        pthread_create(&threads[i], NULL, &trabalhador, &dc);

    divide_trabalhos();

    for (int i = 0; i < NUM_TRABALHADORES; i++)
        pthread_join(threads[i], NULL);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(COMPRIMENTO_JANELA, ALTURA_JANELA);
    glutCreateWindow("Trabalho PAD");

    //glutMouseFunc(mouse);
    glutDisplayFunc(display);

    //Exemplo passando os paramentros
    int i;
    int valuex = 100;
    int valuey = 50;
    for (i = 0; i < 150; i++) {
        draw(valuex, valuey, 1.0, 0.0, 0.0);
        valuex += 20;
        valuey += 20;
    }
    
    //exemplo passando um objeto
    ponto_t ponto;
    ponto.x = 20;
    ponto.y = 300;
    ponto.r = 0.6;
    ponto.g = 0.2;
    ponto.b = 0.2;
    drawObj(ponto);

    glutMainLoop();

    printf("Fim do programa");
}

void * trabalhador(void *arg) {
    int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *) arg;

    while (dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado) {
        trab t;
        t = remove_le(dc->sacoDeTarefas);

        // Calcula o Mandelbrot

    }
    printf("Acabei! e fiz: %d\n", executado);
}

void divide_trabalhos() {
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
