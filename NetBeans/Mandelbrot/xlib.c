#include <stdio.h>
#include <stdlib.h>
#include "xlib.h"

void draw(Display* display, GC gc, Window win, int x, int y, long cor) 
{
    XSetForeground(display, gc, cor);
    XDrawPoint(display, win, gc, x, y);
    XFlush(display);
}