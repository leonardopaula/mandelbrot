#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>

#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"
#include "threads.h"
#include "xlib.h"


#define COMPRIMENTO_JANELA 800
#define ALTURA_JANELA 600
#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

struct dadosCompartilhados dc;
xlib_dados_t xlib;

void * trabalhador(void *str);
void * desenhista(void *str);
pthread_t threads[NUM_TRABALHADORES + 1]; // + Thread que imprime
void divide_trabalhos();
void rodar();

int main(int argc, char *argv[]) 
{
    dc.comprimento = COMPRIMENTO_JANELA;
    dc.altura = ALTURA_JANELA;
    int tamanhoTrabalho = (int) dc.comprimento / DIVISOR_PIXEL * (int) dc.altura / DIVISOR_PIXEL;

    dc.sacoDeTarefas    = inicia_le(sizeof (trabalho_t), tamanhoTrabalho);
    dc.sacoDeResultados = inicia_le(sizeof (ponto_t), dc.comprimento*dc.altura);

    // Levanta a interface
    int screen;
    XEvent event;
    XGCValues val;
   
    xlib.dpy = XOpenDisplay(NULL);
    
    screen = DefaultScreen(xlib.dpy);
    
    xlib.win = XCreateSimpleWindow(xlib.dpy, RootWindow(xlib.dpy, screen),
                            200, 200, COMPRIMENTO_JANELA, ALTURA_JANELA, 
                            1, BlackPixel(xlib.dpy, screen), WhitePixel(xlib.dpy, screen));
    XSelectInput(xlib.dpy, xlib.win, ExposureMask | KeyPressMask);
    XMapWindow(xlib.dpy, xlib.win);

    Colormap screen_colormap;
    screen_colormap = DefaultColormap(xlib.dpy, DefaultScreen(xlib.dpy));
    XAllocNamedColor(xlib.dpy, screen_colormap, "red", &xlib.red, &xlib.red);
    XAllocNamedColor(xlib.dpy, screen_colormap, "white", &xlib.white, &xlib.white);
    XAllocNamedColor(xlib.dpy, screen_colormap, "blue", &xlib.blue, &xlib.blue);
    
    xlib.gc = XCreateGC(xlib.dpy, xlib.win, 0, &val);
    XSetForeground(xlib.dpy, xlib.gc, WhitePixel(xlib.dpy, screen));
    XSetBackground(xlib.dpy, xlib.gc, BlackPixel(xlib.dpy, screen));  
    XSetFillStyle(xlib.dpy, xlib.gc, FillSolid);
   
    dc.xlib = &xlib;
    while(1)
    {
        rodar();
        XNextEvent(xlib.dpy, &event);
    }
    // Lança as threads e divide os trabalhos
    
    printf("Fim do programa");
}

void divide_trabalhos() 
{
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