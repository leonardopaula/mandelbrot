#ifndef GLUT_H
#define GLUT_H

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "estrutura.h"

#ifdef __cplusplus
extern "C" {
#endif

void display(void);
void mouse(int button, int state, int x, int y);
void draw(int x, int y, float r, float g, float b);
void drawObj(ponto_t p);
void keyboard(unsigned char key, int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* GLUT_H */

