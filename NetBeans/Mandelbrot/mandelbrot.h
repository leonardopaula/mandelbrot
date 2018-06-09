#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <GL/gl.h>
#include <GL/glut.h>

#ifdef __cplusplus
extern "C" {
#endif

void mandelbrot_xy(trab trabalho, double escala, int cx, int cy);

#ifdef __cplusplus
}
#endif

#endif /* MANDELBROT_H */

