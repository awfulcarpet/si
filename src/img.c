#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "img.h"

extern Display *dpy;
extern int scr;

XImage *
img_to_ximg(struct Image *img) {
	XImage *ximg = XCreateImage(dpy, CopyFromParent, DefaultDepth(dpy, scr), ZPixmap, 0, NULL, img->width, img->height, 32, 0);
	ximg->data = (char *)img->data;

	XInitImage(ximg);

	return ximg;
}
