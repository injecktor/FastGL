#include <iostream>
#include <fstream>
#include <cstdint>

#include "logging.hpp"
#include "fastgl.hpp"

#include <X11/Xlib.h>

using namespace std;

int main() {
    Display *d; int s; Window w; XEvent ev;
    int should_quit = 0;

    d = XOpenDisplay(NULL);
    s = XDefaultScreen(d);

    w = XCreateSimpleWindow(d, XRootWindow(d, s), 0, 0,
                            200, 200, 0,
                            XBlackPixel(d, s),
                            XWhitePixel(d, s));

    XSelectInput(d, w, ButtonPressMask);
    XMapWindow(d, w);

    while(!should_quit)
    {
        XNextEvent(d, &ev);
        switch(ev.type)
        {
            case ButtonPress:
                should_quit = 1;
                break;
            default:
                break;
        }
    }

    return 0;
}
