#include <X11/Xutil.h>
#include <stdlib.h>

#include "img.h"

extern Display *dpy;
extern int scr;

XImage *
img_to_ximg(struct Image *img) {
	XImage *ximg = XCreateImage(dpy, CopyFromParent, DefaultDepth(dpy, scr), ZPixmap, 0, NULL, img->width, img->height, 32, 0);
	ximg->data = calloc(ximg->bytes_per_line * ximg->height, 1);
	XInitImage(ximg);

	for (int i = 0; i < 100; i++) {
		XPutPixel(ximg, i, 25, 0xFFFF00);
	}

	return ximg;
}
