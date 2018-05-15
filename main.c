#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 
 * https://rosettacode.org/wiki/Mandelbrot_set#PPM_Interactive <---- OLHAR
 */

void * helloWorld( void *str ) {
	printf("Hello. I am thread %d\n", (int) pthread_self());
}

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char* argv[]) 
{
	const int comprimento = atoi(argv[1]);
	const int altura      = atoi(argv[2]);
	const int max_i       = atoi(argv[3]);
	const float c_real      = atof(argv[4]);
	const float c_img       = atof(argv[5]);
	//double real_c         = atof(argv[4]);
	//double im_c           = atof(argv[5]);
	int linha, coluna = 0;
	unsigned char branco[3] = {0, 0, 0};
	unsigned char preto[3] = {255, 255, 255};

	printf("MThread: Iniciando geração da imagem: %d x %d\n", comprimento, altura);
	FILE *fp = fopen("first.ppm", "wb");
	(void) fprintf(fp, "P6\n%d %d\n255\n", comprimento, altura);

	for (linha = 0; linha < altura; linha++)
	{

		for(coluna = 0; coluna < comprimento; coluna++)
		{
			double real_c = (coluna - comprimento/2)*c_real/comprimento;
        	double im_c   = (linha - altura/2)*c_img/comprimento;
			double x = 0, y  = 0;
			int i = 0;

			while(x*x+y*y < 4 && i < max_i)
			{
				double x_new = x*x - y*y + real_c;
				y = 2*x*y+im_c;
				x = x_new;
				i++;
			}
			if (i < max_i) 
			{
				(void) fwrite(branco, 1, 3, fp);
			} else {
				(void) fwrite(preto, 1, 3, fp);
			}
		}
	}

	(void) fclose(fp);
	return 0;
}