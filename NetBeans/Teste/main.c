#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <X11/Xlib.h>

#define COMPRIMENTO_JANELA 800
#define ALTURA_JANELA 600
#define NUM_TRABALHADORES 4
#define DIVISOR_PIXEL 200

pthread_t thread;
typedef struct dadosCompartilhados_t
{
    Display* dpy;
    GC gc;
    Window win;
    XColor red, brown, blue, yellow, green;
    
} dadosCompartilhados_t;
typedef dadosCompartilhados_t dadosCompartilhados;
dadosCompartilhados dc;

void draw(Display* display, GC gc, Window win, int x, int y, long cor) {

    printf("------------------------------- DESENHA ------------------------------------\n");
    
    XSetForeground(display, gc, cor);
    XDrawPoint(display, win, gc, x, y);
}

void opa()
{
    int i = 0;
    for (i = 0; i < 800; i++)
    {
        //draw(i, 600-2, 1.0, 0.0, 0.0);
    }
}

void * desenhista(void *arg)
{
    dadosCompartilhados *dc = (dadosCompartilhados *)arg;
    int i = 0;
    for (i = 0; i < 800; i++)
    {
        draw(dc->dpy, dc->gc, dc->win, i, 10, dc->red.pixel);
        sleep(1);
        XFlush(dc->dpy);
    }
}

int main(int argc, char *argv[]) 
{
    int screen;
    XEvent event;
    XGCValues val;
   
    dc.dpy = XOpenDisplay(NULL);
    
    screen = DefaultScreen(dc.dpy);
    
    dc.win = XCreateSimpleWindow(dc.dpy, RootWindow(dc.dpy, screen), 
                            200, 200, COMPRIMENTO_JANELA, ALTURA_JANELA, 
                            1, BlackPixel(dc.dpy, screen), WhitePixel(dc.dpy, screen));
    XSelectInput(dc.dpy, dc.win, ExposureMask | KeyPressMask);
    XMapWindow(dc.dpy, dc.win);

    Colormap screen_colormap;
    screen_colormap = DefaultColormap(dc.dpy, DefaultScreen(dc.dpy));
    XAllocNamedColor(dc.dpy, screen_colormap, "red", &dc.red, &dc.red);
    
    dc.gc = XCreateGC(dc.dpy, dc.win, 0, &val);
    XSetForeground(dc.dpy, dc.gc, WhitePixel(dc.dpy, screen));
    XSetBackground(dc.dpy, dc.gc, BlackPixel(dc.dpy, screen));  
    XSetFillStyle(dc.dpy, dc.gc, FillSolid);
    
    XSetForeground(dc.dpy, dc.gc, dc.red.pixel);
    XDrawPoint(dc.dpy, dc.win, dc.gc, 95, 5);
    
/*    int i = 0;
    for(i = 0; i < 500; i++)
    {
        draw(dc.dpy, gc, dc.win, i, 10, red.pixel);
    }*/
    
    while(1)
    {
        pthread_create(&thread, NULL, &desenhista, &dc);
        pthread_join(thread, NULL);
    
        XNextEvent(dc.dpy, &event);
    }
            
    
    printf("Fim do programa");
}