#include "listaEncadeada.h"
#include "estrutura.h"
#include "mandelbrot.h"

void * trabalhador(void *arg) 
{
    int executado = 0;
    struct dadosCompartilhados *dc = (struct dadosCompartilhados *)arg;

    while(dc->sacoDeTarefas->tamanhoTrabalho > dc->sacoDeTarefas->computado)
    {
        trab t;
        t = remove_le(dc->sacoDeTarefas);
        
        // Calcula o Mandelbrot
        //mandelbrot_xy(t, 1, -1.0, -1.0);

    }
    printf("Acabei! e fiz: %d\n", executado);
}
