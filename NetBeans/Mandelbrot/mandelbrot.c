#include "listaEncadeada.h"
#include "estrutura.h"
#include "glut.h"
#include <stdio.h>

#define MAX_ITERACOES 5000

void mandelbrot_xy(trab trabalho, double escala, int cx, int cy, lista resultados) {

    int linha, coluna = 0;
    int comprimento = trabalho->final.x - trabalho->inicial.x;
    int altura = trabalho->final.y - trabalho->inicial.y;
    unsigned char branco[3] = {0, 0, 0};
    unsigned char preto[3] = {255, 255, 255};

    for (linha = trabalho->inicial.y; linha < trabalho->final.y; linha++)
    {
        for (coluna = trabalho->inicial.x; coluna < trabalho->final.x; coluna++)
	{
            double real_c = (coluna - comprimento / 2) * cx / comprimento;
            double im_c = (linha - altura / 2) * cy / comprimento;
            double x = 0, y = 0;
            int i = 0;
            while (x * x + y * y < 4 && i < MAX_ITERACOES)
            {
                double x_new = x * x - y * y + real_c;
                y = 2 * x * y + im_c;
		x = x_new;
		i++;
            }

            ponto* p;
            p = (ponto_t *) malloc(sizeof(ponto_t));
            memset(p, 0, sizeof(ponto_t));

            if (i < MAX_ITERACOES)
            {
                p->r = 1.0;
                p->g = 1.0;
                p->b = 1.0;
                p->x = coluna;
                p->y = linha;
                // draw(coluna, linha, 1.0, 1.0, 1.0);
                // printf("(%d,%d) Branco\n",coluna, linha);
            }
            else
            {
                p->r = 0.0;
                p->g = 0.0;
                p->b = 0.0;
                p->x = coluna;
                p->y = linha;
                // draw(coluna, linha, 0.0, 0.0, 0.0);
                // printf("(%d,%d) Preto\n",coluna, linha);
            }
            adiciona_le(resultados, p);
        }
    }
}