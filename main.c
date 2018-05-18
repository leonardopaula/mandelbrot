#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/* 
 * https://rosettacode.org/wiki/Mandelbrot_set#PPM_Interactive <---- OLHAR
 */

struct thread_data
{
	int comprimento;
	int altura;
	int max_i;
	float c_real;
	float c_img;
};

void *helloWorld(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *)threadarg;

	printf("MThread: Iniciando geração da imagem: %d x %d\n", my_data->comprimento, my_data->altura);

	int linha, coluna = 0;
	unsigned char branco[3] = {0, 0, 0};
	unsigned char preto[3] = {255, 255, 255};

	char nome_arquivo[20];

	sprintf(nome_arquivo, "%ds.ppm", pthread_self());

	FILE *fp = fopen(nome_arquivo, "wb");
	(void)fprintf(fp, "P6\n%d %d\n255\n", my_data->comprimento, my_data->altura);

	for (linha = 0; linha < my_data->altura; linha++)
	{
		for (coluna = 0; coluna < my_data->comprimento; coluna++)
		{
			double real_c = (coluna - my_data->comprimento / 2) * my_data->c_real / my_data->comprimento;
			double im_c = (linha - my_data->altura / 2) * my_data->c_img / my_data->comprimento;
			double x = 0, y = 0;
			int i = 0;

			while (x * x + y * y < 4 && i < my_data->max_i)
			{
				double x_new = x * x - y * y + real_c;
				y = 2 * x * y + im_c;
				x = x_new;
				i++;
			}

			if (i < my_data->max_i)
			{
				(void)fwrite(branco, 1, 3, fp);
				//printf("Hello. I am thread white %d\n", coluna);
			}
			else
			{
				(void)fwrite(preto, 1, 3, fp);
				//printf("Hello. I am thread black %d\n", coluna);
			}
		}
	}

	(void)fclose(fp);
}

/*
 * argv[1] = width
 * argv[2] = height
 */
int main(int argc, char *argv[])
{
	struct thread_data td;
	td.comprimento = atoi(argv[1]);
	td.altura = atoi(argv[2]);
	td.max_i = atoi(argv[3]);
	td.c_real = atof(argv[4]);
	td.c_img = atof(argv[5]);

	//double real_c         = atof(argv[4]);
	//double im_c           = atof(argv[5]);
	int numThreads = 5;
	int rc;
	int i;

	pthread_t threads[numThreads];
	struct thread_data thread_d[numThreads];

	for (i = 0; i < numThreads; i++)
	{
		//cout <<"main() : creating thread, " << i << endl;

		thread_d[i] = td;
		rc = pthread_create(&threads[i], NULL, helloWorld, (void *)&thread_d[i]);

		if (rc)
		{
			//cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}

		//int rc = pthread_create(&threads, NULL, helloWorld, (void *)&td);
	}

	pthread_exit(NULL);
	return 0;
}
