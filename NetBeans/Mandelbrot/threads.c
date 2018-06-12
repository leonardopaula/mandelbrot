#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"
#include "xlib.h"

void * trabalhador(void *arg) 
{
    int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado)
    {
        trab t;
        t = remove_le(dc->sacoDeTarefas);
        
        mandelbrot_xy(t, 10, -.6, 0.0, dc->sacoDeResultados);

    }
    //printf("Acabei! e fiz: %d\n", executado);
}

void * desenhista(void *arg)
{
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeResultados->tamanhoTrabalho > dc->sacoDeResultados->computado)
    {
        ponto* p;
        p = remove_le(dc->sacoDeResultados);
        //printf("%d x %d", p->x, p->y);
        //draw(p->x, p->y, p->r, p->g, p->b);
        if (p->r == 0)
            draw(dc->xlib->dpy, dc->xlib->gc, dc->xlib->win, p->x, p->y, dc->xlib->blue.pixel);
        else
            draw(dc->xlib->dpy, dc->xlib->gc, dc->xlib->win, p->x, p->y, dc->xlib->red.pixel);
        //printf("--> %d x %d", p->x, p->y);
    }
}