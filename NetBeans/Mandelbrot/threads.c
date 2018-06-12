#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"
#include "glut.h"

void * trabalhador(void *arg) 
{
    int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado)
    {
        trab t;
        t = remove_le(dc->sacoDeTarefas);
        
        mandelbrot_xy(t, 1, -1.0, -1.0, dc->sacoDeResultados);

    }
    //printf("Acabei! e fiz: %d\n", executado);
}

void * desenhista(void *arg)
{
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;
    
    glColor3f(1.0, 0.0, 1.0);
    /*
    while(dc->sacoDeResultados->tamanhoTrabalho > dc->sacoDeResultados->computado)
    {
        ponto* p;
        p = remove_le(dc->sacoDeResultados);
        //printf("%d x %d", p->x, p->y);
        
        draw(p->x, p->y, p->r, p->g, p->b);
        
        //printf("--> %d x %d", p->x, p->y);
    }*/
}