#include <ctype.h>

#include <QWidget>
#include <QX11Info>

#include "hangul.h"
#include "qinputcontexthangul.h"

#include <X11/Xlib.h>

void QInputContextHangul::setModeInfo(int mode)
{
    Display* display = NULL;
    int screen = 0;

    QWidget* focus = focusWidget();
    if (focus != NULL) {
	display = focus->x11Info().display();
	screen = focus->x11Info().appScreen();
    }

    if (display != NULL) {
	long data = mode;
	Window window = RootWindow(display, screen);
	Atom status = XInternAtom(display, "_HANGUL_INPUT_MODE", False);
	Atom type = XInternAtom(display, "INTEGER", False);

	if (window != None && status != None && type != None) {
	    XChangeProperty(display, window, status, type,
		    32, PropModeReplace, (const unsigned char *)&data, 1);
	}
    }
}
