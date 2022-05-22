/*
 * xhole.c
 *
 * A sample application using the shape extension.
 * Creates a window with a hole in the middle. Works
 * with twm, fvwm, and mwm, but kde refuses to add
 * a title bar to it.
 * Compile with `gcc xhole.c -o xhole -lX11 -lXext'
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

int main(int argc, char* argv[]) {
	Display *d;
	int s;
	Window w;
	Pixmap p;
	GC gw, gp;
	XEvent e;
	int x, y;

	/* open connection with the server */
	d = XOpenDisplay(NULL);
	if (d == NULL) {
		printf("Cannot open display\n");
		exit(1);
	}
	s = DefaultScreen(d);
	gw = DefaultGC(d, s);

	/* create window, select events, map  */
	w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 300, 200, 1,
			BlackPixel(d, s), WhitePixel(d, s));
	XSelectInput(d, w, ExposureMask | KeyPressMask | ButtonPressMask);
	XStoreName(d, w, "XHole");
	XMapWindow(d, w);

	/* create the pixmap that specifies the shape */
	p = XCreatePixmap(d, w, 400, 300, 1);
	gp = XCreateGC(d, p, 0, NULL);
	XSetForeground(d, gp, WhitePixel(d, s));
	XFillRectangle(d, p, gp, 0, 0, 400, 300);
	XSetForeground(d, gp, BlackPixel(d, s));
	XFillArc(d, p, gp, 120, 100, 100, 100, 0, 360*64);

	/* set the pixmap as the new window mask;
	the pixmap is slightly larger than the window
	to allow for the window border and title bar
	(as added by the window manager) to be visible */
	XShapeCombineMask(d, w, ShapeBounding, -20, -50, p, ShapeSet);

	/* event polling loop */
	while (1) {
		XNextEvent(d, &e);
		/* draw or redraw the window */
		if (e.type == Expose) {
			/* not the correct way of drawing text... */
			for (y = 10; y <= 210; y += 11) {
				for (x = 0; x < 300; x += 25) XDrawString(d, w, gw, x, y, "test", 4);
			}
		}

		/* exit on button press */
		if (e.type == ButtonPress)
			break;
	}

	/* close connection to display */
	XCloseDisplay(d);

	return 0;
}