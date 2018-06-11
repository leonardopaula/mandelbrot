#include "glut.h"

void display(void) {
    
    const double w = glutGet(GLUT_WINDOW_WIDTH);
    const double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);

    printf("------------------------------- PRONTO ------------------------------------\n");
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    /*
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        point.x = x;
        point.y = y;
        point.r = 0.0;
        point.g = 1.0;
        point.b = 0.0;
    }*/
    glBegin(GLUT_SINGLE | GLUT_RGB);
        glVertex2i(x, 600 - y);
        printf("%dx%d\n", x, (600-y));
    glEnd();
    
    glFlush();
    //glutPostRedisplay();
}

void draw(int x, int y, float r, float g, float b) {

    /*
    point.x = x;
    point.y = y;
    point.r = r;
    point.g = g;
    point.b = b;*/
    printf("------------------------------- DESENHA ------------------------------------\n");
    glColor3f(r, g, b);
    
    glBegin(GLUT_SINGLE | GLUT_RGB);
        glVertex2i(x, 600 - y);
    glEnd;
    
    glFlush();
    
    /*printf("Desnhei");*/
}

void drawObj(ponto_t p) {
    //point = p;
    //display();
}