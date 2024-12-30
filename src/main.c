#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
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
	XSelectInput(dpy, w, StructureNotifyMask | KeyPressMask | ExposureMask);

	gc = XCreateGC(dpy, w, 0, NULL);
	XSetForeground(dpy, gc, white);

	scr = DefaultScreen(dpy);

	XMapWindow(dpy, w);

	while (e.type != MapNotify) XNextEvent(dpy, &e);
}

int
main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "please provide image path\n");
		return 1;
	}

	struct Image *img = read_png(argv[1]);

	init_client(img->width, img->height);

	XImage *ximg = img_to_ximg(img);

	XPutImage(dpy, w, gc, ximg, 0, 0, 0, 0, ximg->width, ximg->height);

	XSync(dpy, False);
	while (XNextEvent(dpy, &e) == 0) {
		if (e.type == Expose)
			XPutImage(dpy, w, gc, ximg, 0, 0, 0, 0, ximg->width, ximg->height);
	}

	XDestroyImage(ximg);
	XUnmapWindow(dpy, w);
	XDestroyWindow(dpy, w);
	XCloseDisplay(dpy);
	return 0;
}
