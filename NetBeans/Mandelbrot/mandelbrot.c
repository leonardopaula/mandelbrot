#include "estrutura.h"
#include <stdio.h>

#define MAX_ITERACOES 5000

void mandelbrot_xy(trab trabalho, double escala, int cx, int cy) {

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

            if (i < MAX_ITERACOES)
            {
                printf("(%d,%d) Branco\n",coluna, linha);
            }
            else
            {
                printf("(%d,%d) Preto\n",coluna, linha);
            }
        }
    }
}