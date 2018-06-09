#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <pthread.h>

//gcc -o mandelbrot  mandelbrot.c -L/System/Library/Frameworks -framework GLUT -framework OpenGL

#define VAL 255
#define NUM_THREADS 5

void set_texture();

struct thread_data
{
	int thread_id;
	char *message;
};

struct parametro_divisao
{
	double x_from;
	double x_to;
	double y_from;
	double y_to;
};

typedef struct
{
	unsigned char r, g, b;
} rgb_t;

rgb_t **tex = 0;
int gwin;
GLuint texture;
int width, height;
int tex_w, tex_h;
double scale = 1. / 256;
double cx = -.6, cy = 0;
int color_rotate = 0;
int saturation = 1;
int invert = 0;
int max_iter = 4096;
int dump = 1; //Screen Dump

//Teste threads , printa hello world
void *PrintHello(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *)threadarg;

	printf("Thread ID : %d\n", my_data->thread_id);
	printf("Message : %s\n", my_data->message);

	pthread_exit(NULL);
}

void render()
{
	printf("Renderizando... \n");
	double x = (double)width / tex_w;
	double y = (double)height / tex_h;

	glClear(GL_COLOR_BUFFER_BIT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2i(0, 0);
	glTexCoord2f(x, 0);
	glVertex2i(width, 0);
	glTexCoord2f(x, y);
	glVertex2i(width, height);
	glTexCoord2f(0, y);
	glVertex2i(0, height);

	glEnd();

	glFlush();
	glFinish();
}

void screen_shot()
{
	char fileArray[100];
	int i;
	sprintf(fileArray, "screenshot%03d.ppm", dump++);
	FILE *fp = fopen(fileArray, "w");
	fprintf(fp, "P6\n%d %d\n255\n", width, height);
	for (i = height - 1; i >= 0; i--)
		fwrite(tex[i], 1, width * 3, fp);
	fclose(fp);
	printf("%s retirado\n", fileArray);
}

void comandos(unsigned char comando, int x, int y)
{
	switch (comando)
	{
	case 'q':
		glFinish();
		glutDestroyWindow(gwin);
		return;
	case 'r':
		color_rotate = (color_rotate + 1) % 6;
		break;
	case 'c':
		saturation = 1 - saturation;
		break;
	case 's':
		screen_shot();
		return;
	case ' ':
		invert = !invert;
		break;
	}
	set_texture();
}

//Conversão de corres HSV para RGB
//https://www.rapidtables.com/convert/color/hsv-to-rgb.html
void hsv_to_rgb(int hue, int min, int max, rgb_t *p)
{
	if (min == max)
		max = min + 1;
	if (invert)
		hue = max - (hue - min);
	if (!saturation)
	{
		p->r = 255 * (max - hue) / (max - min);
		p->g = 255 * (max - hue) / (max - min);
		p->b = 255 * (max - hue) / (max - min);
		return;
	}

	double h = fmod(color_rotate + 1e-4 + 4.0 * (hue - min) / (max - min), 6);

	double c = VAL * saturation;
	double X = c * (1 - fabs(fmod(h, 2) - 1));

	p->r = 0;
	p->g = 0;
	p->b = 0;

	switch ((int)h)
	{
	case 0:
		p->r = c;
		p->g = X;
		return;
	case 1:
		p->r = X;
		p->g = c;
		return;
	case 2:
		p->g = c;
		p->b = X;
		return;
	case 3:
		p->g = X;
		p->b = c;
		return;
	case 4:
		p->r = X;
		p->b = c;
		return;
	default:
		p->r = c;
		p->b = X;
	}
}

void *calcular_mandelbrot(void *threadarg)
{
	long tid;
	tid = (long)threadarg;
	printf("Hello World! Thread ID,  %ld\n", tid);

	int i, j, iter, min, max;
	rgb_t *px;
	double x, y, zx, zy, zx2, zy2;
	min = max_iter;
	max = 0;
	for (i = 0; i < height; i++)
	{
		px = tex[i];
		y = (i - height / 2) * scale + cy;
		for (j = 0; j < width; j++, px++)
		{
			x = (j - width / 2) * scale + cx;
			iter = 0;

			zx = hypot(x - .25, y);
			if (x < zx - 2 * zx * zx + .25)
				iter = max_iter;
			if ((x + 1) * (x + 1) + y * y < 1 / 16)
				iter = max_iter;

			zx = zy = zx2 = zy2 = 0;
			for (; iter < max_iter && zx2 + zy2 < 4; iter++)
			{
				zy = 2 * zx * zy + y;
				zx = zx2 - zy2 + x;
				zx2 = zx * zx;
				zy2 = zy * zy;
			}
			if (iter < min)
				min = iter;
			if (iter > max)
				max = iter;
			*(unsigned short *)px = iter;
		}
	}

	for (i = 0; i < height; i++)
		for (j = 0, px = tex[i]; j < width; j++, px++)
			hsv_to_rgb(*(unsigned short *)px, min, max, px);

	pthread_exit(NULL);
}

void *calc_mandel_by_xy(void *param_thread)
{
	printf("Thread %d starting \n", pthread_self());
	struct parametro_divisao *param = (struct parametro_divisao *)param_thread;

	int i;
	int j;
	int iter;
	int min;
	int max;
	rgb_t *px;
	double x;
	double y;
	double zx;
	double zy;
	double zx2;
	double zy2;

	min = max_iter;
	max = 0;

	for (i = param->y_from; i < param->y_to; i++)
	{
		px = tex[i];
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

	// printf("Desenhar...\n");
	// for (i = param->y_from; i < param->y_to; i++)
	// 	for (j = param->x_from, px = tex[i]; j < param->x_to; j++, px++)
	// 		hsv_to_rgb(*(unsigned short*)px, min, max, px);

	printf("Thread %d ending \n", pthread_self());

	return NULL;
}

void calc_mandel()
{
	int i;
	int j;
	int iter;

	rgb_t *px;

	int min = max_iter;
	int max = 0;

	double y_half = height / 2;
	double x_half = width / 2;

	pthread_t thread_id1;
	pthread_t thread_id2;
	pthread_t thread_id3;
	pthread_t thread_id4;

	struct parametro_divisao param1;
	struct parametro_divisao param2;
	struct parametro_divisao param3;
	struct parametro_divisao param4;

	if (1 == 1)
	{
		// param1.x_from = 0, 0;
		// param1.x_to = x_half;
		// param1.y_from = 0, 0;
		// param1.y_to = y_half;

		param2.x_from = x_half;
		param2.x_to = height;
		param2.y_from = 0, 0;
		param2.y_to = y_half;

		param3.x_from = 0;
		param3.x_to = x_half;
		param3.y_from = y_half;
		param3.y_to = width;

		// param4.x_from = x_half;
		// param4.x_to = height;
		// param4.y_from = y_half;
		// param4.y_to = width;

		// pthread_create(&thread_id1, NULL, &calc_mandel_by_xy, &param1);
		pthread_create(&thread_id2, NULL, &calc_mandel_by_xy, &param2);
		pthread_create(&thread_id3, NULL, &calc_mandel_by_xy, &param3);
		// pthread_create(&thread_id4, NULL, &calc_mandel_by_xy, &param4);

		// pthread_join(thread_id1, NULL);
		pthread_join(thread_id2, NULL);
		pthread_join(thread_id3, NULL);
		// pthread_join(thread_id4, NULL);
	}
	else
	{
		param1.x_from = 0;
		param1.x_to = height;
		param1.y_from = 0;
		param1.y_to = width;

		calc_mandel_by_xy((void *)&param1);
	}

	// printf("Desenhar...\n");

	for (i = 0; i < height; i++)
		for (j = 0, px = tex[i]; j < width; j++, px++)
			hsv_to_rgb(*(unsigned short *)px, min, max, px);
}

void alloc_tex()
{
	int i;
	int ow = tex_w; //text width
	int oh = tex_h; // text height

	for (tex_w = 1; tex_w < width; tex_w <<= 1);
	for (tex_h = 1; tex_h < height; tex_h <<= 1);

	if (tex_h != oh || tex_w != ow)
	{
		tex = realloc(tex, tex_h * tex_w * 3 + tex_h * sizeof(rgb_t *));
	}

	for (tex[0] = (rgb_t *)(tex + tex_h), i = 1; i < tex_h; i++)
		tex[i] = tex[i - 1] + tex_w;
}

// void set_texture()
// {
// 	alloc_tex();

// 	pthread_t threads[NUM_THREADS];
// 	pthread_attr_t attr;
//     void *status;

// 	 // Initialize and set thread joinable
//   	 pthread_attr_init(&attr);
//   	 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

// 	int rc = pthread_create(&threads[0], NULL, calcular_mandelbrot, (void *)0);
// 	// free attribute and wait for the other threads
// 	pthread_attr_destroy(&attr);
// 	rc = pthread_join(threads[0], &status);

// 	// calcular_mandelbrot();

// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, texture);
// 	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_w, tex_h,
// 				 0, GL_RGB, GL_UNSIGNED_BYTE, tex[0]);

// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 	render();
// }

void set_texture()
{
	alloc_tex();
	calc_mandel();
	printf("Setando textura\n");
	// calcular_mandelbrot();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_w, tex_h,
				 0, GL_RGB, GL_UNSIGNED_BYTE, tex[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	render();
}

void mouseclick(int button, int state, int x, int y)
{
	if (state != GLUT_UP)
		return;

	cx += (x - width / 2) * scale;
	cy -= (y - height / 2) * scale;

	switch (button)
	{
	case GLUT_LEFT_BUTTON: // aumentar zoom
		if (scale > abs(x) * 1e-16 && scale > abs(y) * 1e-16)
			scale /= 2;
		break;
	case GLUT_RIGHT_BUTTON: // diminuir zoom
		scale *= 2;
		break;
	}
	printf("Mouse...");
	set_texture();
}

void resize(int w, int h)
{

	//Alterar Resize para sempre ser divisivel por 2
	printf("resizing... %d %d\n", w, h);

	while(height % 2 != 0){
		height--;
	}
	while(width % 2 != 0){
		width--;
	}

	printf("resizing... %d %d\n", w, h);
	width = w;
	height = h;

	glViewport(0, 0, w, h);
	glOrtho(0, w, 0, h, -1, 1);
	

	set_texture();
}

void iniciar_graphics(int *c, char **v)
{
	printf("iniciando...\n");
	glutInit(c, v);
	glutInitDisplayMode(GLUT_RGB);

	/*Tamanho da janela*/
	width = 1000;
	height = 700;

	glutInitWindowSize(width, height);

	gwin = glutCreateWindow("Mandelbrot");
	glutDisplayFunc(render);

	glutKeyboardFunc(comandos);
	glutMouseFunc(mouseclick);
	glutReshapeFunc(resize);
	glGenTextures(1, &texture);
	set_texture();
}

int main(int c, char **v)
{
	iniciar_graphics(&c, v);
	printf("Opções:\n\tr: Rotação de Cores\n\tc: Monocromático\n\ts: Screen Dump\n\t"
		   "q: Sair\n\tMouse para Zoom\n");

	glutMainLoop();
	return 0;
}