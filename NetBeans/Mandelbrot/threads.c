#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"
#include "xlib.h"

void * trabalhador(void *arg) 
{
    //int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado)
    {
        trab t;
        t = remove_le(dc->sacoDeTarefas);
        
        mandelbrot_xy(t, dc->cfgMandelbrot->escala, dc->cfgMandelbrot->cx, dc->cfgMandelbrot->cy, dc->sacoDeResultados);
        free(t);
    }
    printf("Acabei de calcular\n");
    return EXIT_SUCCESS;
}

void * desenhista(void *arg)
{
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeResultados->tamanhoTrabalho > dc->sacoDeResultados->computado)
    {
        ponto* p;
        p = remove_le(dc->sacoDeResultados);

        if (p->r == 0)
            draw(dc->xlib->dpy, dc->xlib->gc, dc->xlib->win, p->x, p->y, dc->xlib->blue.pixel);
        else
            draw(dc->xlib->dpy, dc->xlib->gc, dc->xlib->win, p->x, p->y, dc->xlib->red.pixel);
        
        free(p);
    }
    printf("Acabei de desenhar\n");
    return EXIT_SUCCESS;
}