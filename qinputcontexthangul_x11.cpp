#include <ctype.h>

#include <qapplication.h>
#include <qstring.h>
#include <qevent.h>

#include <X11/Xlib.h>

#include "hangul.h"
#include "qinputcontexthangul.h"

void QInputContextHangul::setModeInfo(int mode)
{
    long data = mode;
    Display *display = QPaintDevice::x11AppDisplay();
    Window window = RootWindow(display, QPaintDevice::x11AppScreen());
    Atom status = XInternAtom(display, "_HANGUL_INPUT_MODE", False);
    Atom type = XInternAtom(display, "INTEGER", False);
    if (window != None && status != None && type != None) {
	XChangeProperty(display, window, status, type,
			32, PropModeReplace, (const unsigned char *)&data, 1);
    }
}
