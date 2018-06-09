#include "estrutura.h"
#define MAX_ITERACOES 5000

/*
void mandelbrot_xy(trab trabalho, double escala, int cx, int cy) {
    
    int i, j, iter, min, max = 0;
    gl_cor *px;
    double x, y, zx, zy, zx2, zy2;

    min = max_iter;
    max = 0;

    for (i = trabalho->inicial.y; i < trabalho->final.y; i++)
    {
        //px = tex[i];
	y = (i - height / 2) * scale + cy;
	for (j = 0; j < width; j++, px++)
	{
            x = (j - width / 2) * scale + cx;
            iter = 0;
            zx = hypot(x - .25, y);

            if ((x < zx - 2 * zx * zx + .25) || ((x + 1) * (x + 1) + y * y < 1 / 16))
            {
                iter = max_iter;
            }

			zx = zy = zx2 = zy2 = 0;
			for (; iter < max_iter && zx2 + zy2 < 4; iter++)
			{
				zy = 2 * zx * zy + y;
				zx = zx2 - zy2 + x;
				zx2 = zx * zx;
				zy2 = zy * zy;
			}

			if (iter < min)
			{
				min = iter;
			}

			if (iter > max)
			{
				max = iter;
			}

			*(unsigned short *)px = iter;
		}
	}
}*/