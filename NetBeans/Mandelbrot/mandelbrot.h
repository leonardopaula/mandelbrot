#ifndef MANDELBROT_H
#define MANDELBROT_H

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
#ifdef __cplusplus
extern "C" {
#endif

void mandelbrot_xy(trab trabalho, double escala, double cx, double cy, lista resultados);

#ifdef __cplusplus
}
#endif

#endif /* MANDELBROT_H */

