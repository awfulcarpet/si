#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	scr = DefaultScreen(dpy);

	int black = BlackPixel(dpy, DefaultScreen(dpy));
	int white = WhitePixel(dpy, DefaultScreen(dpy));

	int mid_x = DisplayWidth(dpy, scr) / 2 - width / 2;
	int mid_y = DisplayHeight(dpy, scr) / 2 - height / 2;

	w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), mid_x, mid_y,
		width, height, 0, black, black);
	XSelectInput(dpy, w, StructureNotifyMask | KeyPressMask | ExposureMask);

	XSizeHints hints = {
		.flags = PMaxSize,
		.max_width = width,
		.max_height = height,
	};

	XSetNormalHints(dpy, w, &hints);

	gc = XCreateGC(dpy, w, 0, NULL);
	XSetForeground(dpy, gc, white);


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

	int i = 0;
	for (i = 0; argv[1][i] != '\0'; i++) {
		if (argv[1][i] == '.')
			break;
	}

	struct Image *img = NULL;

	/* TODO: don't differentiate with file extensions */
	if (!strcmp(&argv[1][i], ".png")) {
		img = read_png(argv[1]);
	}

	if (!strcmp(&argv[1][i], ".bmp")) {
		img = read_bmp(argv[1]);
	}

	if (img == NULL) {
		fprintf(stderr, "file format %s not supported\n", &argv[1][i]);
		exit(1);
	}

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
