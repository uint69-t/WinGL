#include <X11/Xlib.h>
#include <GL/glx.h>

class WinGL {
private:
    Display *dpy;
    XVisualInfo *vis;
    Window win;
    Atom wmDeleteMessage;
    GLXContext ctx;
    XEvent evt;

public:
    /* Creates and displays a window. */
    WinGL(const unsigned int width, const unsigned int height, const char *title);

    /* Swaps the buffers and checks for exit events. Returns false if the user closed the window. */
    bool Update();

    /* Closes the window and frees the resources used. */
    void Close();
};