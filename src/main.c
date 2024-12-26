#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "img.h"

Display *dpy;
Window w;
GC gc;
XEvent e;
int scr;

void
init_client(int width, int height)
{
	dpy = XOpenDisplay(NULL);
	assert(dpy);

	int black = BlackPixel(dpy, DefaultScreen(dpy));
	int white = WhitePixel(dpy, DefaultScreen(dpy));

	w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
		width, height, 0, black, black);
	XSelectInput(dpy, w, StructureNotifyMask | KeyPressMask);

	gc = XCreateGC(dpy, w, 0, NULL);
	XSetForeground(dpy, gc, white);

	scr = DefaultScreen(dpy);

	XMapWindow(dpy, w);

	while (e.type != MapNotify) XNextEvent(dpy, &e);
}

int
main(void)
{
	init_client(200, 100);

	struct Image tmp = {
		.width = 100,
		.height = 100,
		.data = NULL,
	};

	XImage *img = img_to_ximg(&tmp);

	XPutImage(dpy, w, gc, img, 0, 0, 0, 0, 100, 50);


	XSync(dpy, False);
	while (XNextEvent(dpy, &e) == 0);

	XDestroyImage(img);
	XUnmapWindow(dpy, w);
	XDestroyWindow(dpy, w);
	XCloseDisplay(dpy);
	return 0;
}
