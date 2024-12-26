#include <X11/Xlib.h>
#include <assert.h>

Display *dpy;
Window w;

void
init(void)
{
	dpy = XOpenDisplay(NULL);
	assert(dpy);

	int blackColor = BlackPixel(dpy, DefaultScreen(dpy));

	w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
	100, 100, 0, blackColor, blackColor);

	XSelectInput(dpy, w, StructureNotifyMask | KeyPressMask);

	XMapWindow(dpy, w);
}

int
main(void)
{
	init();

	XEvent e;
	while (XNextEvent(dpy, &e) == 0);

	XUnmapWindow(dpy, w);
	XDestroyWindow(dpy, w);
	XCloseDisplay(dpy);
	return 0;
}
