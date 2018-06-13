#include <X11/Xlib.h>
#ifndef XLIB_H
#define XLIB_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct xlib_dados_t
{
    Display* dpy;
    GC gc;
    Window win;
    XColor red, blue, white, black;
} xlib_dados_t;
typedef xlib_dados_t* xlib_dados;

void draw(Display* display, GC gc, Window win, int x, int y, long cor);

#ifdef __cplusplus
}
#endif

#endif /* XLIB_H */

