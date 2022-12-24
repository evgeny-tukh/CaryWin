#include "window.h"
#include "windef.h"
#include "winclass.h"
#include "app.h"

Cary::Window::Window (WinDef *_winDef): winDef (*_winDef) {
    cls = findClass (_winDef->cls);
    if (!cls) {
        cls = WinClass::createCls (_winDef->cls.c_str ());
        cls->registerCls ();
    }
}

HWND Cary::Window::addControl (const char *ctlCls, int x, int y, int width, int height, uint32_t style, uint64_t id, const char *text) {
    return CreateWindowA (ctlCls, text, WS_CHILD | style, x, y, width, height, wnd, (HMENU) id, App::getInst (), nullptr);
}