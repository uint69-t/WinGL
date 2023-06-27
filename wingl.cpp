#include "wingl.h"

/* Creates and displays a window. */
WinGL::WinGL(const unsigned int width, const unsigned int height, const char *title) {
    int attribList[] = { GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };

    XSetWindowAttributes wAttribs;
    XSizeHints *sizeHints = XAllocSizeHints();
    XWMHints *wmHints = XAllocWMHints();
    
    this->dpy = XOpenDisplay(NULL);
    this->vis = glXChooseVisual(this->dpy, DefaultScreen(this->dpy), attribList);

    wAttribs.colormap = XCreateColormap(this->dpy, RootWindow(this->dpy, this->vis->screen),
                                        this->vis->visual, AllocNone);
    wAttribs.border_pixel = 0;
    wAttribs.event_mask = ExposureMask | KeyPressMask;

    this->win = XCreateWindow(this->dpy, RootWindow(this->dpy, this->vis->screen), 0, 0, width, height, 0,
                              this->vis->depth, InputOutput, this->vis->visual,
                              CWBorderPixel | CWColormap | CWEventMask, &wAttribs);

	this->wmDeleteMessage = XInternAtom(this->dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(this->dpy, this->win, &this->wmDeleteMessage, 1);

    XStoreName(this->dpy, this->win, title);

    sizeHints->flags = PMinSize | PMaxSize;
    sizeHints->min_width = width;
    sizeHints->min_height = height;
    sizeHints->max_width = width;
    sizeHints->max_height = height;

    wmHints->flags = InputHint | StateHint;
    wmHints->input = True;
    wmHints->initial_state = NormalState;

    XSetWMNormalHints(this->dpy, this->win, sizeHints);
    XSetWMHints(this->dpy, this->win, wmHints);

    XFree(sizeHints);
    XFree(wmHints);

    this->ctx = glXCreateContext(this->dpy, this->vis, None, GL_TRUE);
    glXMakeCurrent(this->dpy, this->win, this->ctx);

    XMapWindow(this->dpy, this->win);
}

/* Swaps the buffers and checks for exit events. Returns false if the user closed the window. */
bool WinGL::Update() {
    glXSwapBuffers(this->dpy, this->win);
    while (XPending(this->dpy)) {
        XNextEvent(this->dpy, &this->evt);
        if (this->evt.type == ClientMessage) {
			if ((Atom) this->evt.xclient.data.l[0] == this->wmDeleteMessage) {
                return false;
			}
		} else if (this->evt.type == DestroyNotify) {
            return false;
        }
    }

    return true;
}

/* Closes the window and frees the resources used. */
void WinGL::Close() {
    glXMakeCurrent(this->dpy, None, NULL);
    glXDestroyContext(this->dpy, this->ctx);
    XDestroyWindow(this->dpy, this->win);
    XCloseDisplay(this->dpy);
}
