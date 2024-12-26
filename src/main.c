#include <X11/X.h>
#include <X11/Xlib.h>
#include <assert.h>

Display *dpy;
Window w;
GC gc;
XEvent e;

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


	XMapWindow(dpy, w);

	while (e.type != MapNotify) XNextEvent(dpy, &e);
}

int
main(void)
{
	init_client(200, 100);

	XDrawLine(dpy, w, gc, 10, 60, 180, 20);
	XFlush(dpy);

	while (XNextEvent(dpy, &e) == 0);

	XUnmapWindow(dpy, w);
	XDestroyWindow(dpy, w);
	XCloseDisplay(dpy);
	return 0;
}
