#include <Windows.h>
#include <CommCtrl.h>
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
    if (strcmp (ctlCls,"EDIT") == 0) {
        return CreateWindowExA (
            WS_EX_CLIENTEDGE,
            "EDIT",
            text,
            WS_CHILD | style,
            x,
            y,
            width,
            height,
            wnd,
            (HMENU) id,
            App::getInst (),
            nullptr
        );
    }
    return CreateWindowA (ctlCls, text, WS_CHILD | style, x, y, width, height, wnd, (HMENU) id, App::getInst (), nullptr);
}    

HWND Cary::Window::createUpDownButton (HWND editCtl, int min, int max, int value) {
    static uint64_t id = 1000;

    auto ctl = CreateWindow (
        "msctls_updown32",
        "",
        UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | WS_VISIBLE | WS_CHILD,
        0,
        2,
        10,
        22,
        wnd,
        (HMENU) id ++,
        App::getInst (),
        nullptr
    );
    SendMessage (ctl, UDM_SETBUDDY, (WPARAM) editCtl, 0);
    SendMessage (ctl, UDM_SETRANGE32, min, max);
    SendMessage (ctl, UDM_SETPOS32, 0, value);

    return ctl;
}

HWND Cary::Window::createLabeledControl (
    const char *ctlCls,
    const char *label,
    const uint32_t style,
    uint32_t id,
    int x,
    int& y,
    int labelWidth,
    int ctlWidth,
    int ctlHeight,
    const char *ctlText
) {
    addControl ("STATIC", x, y, labelWidth, 25, SS_LEFT | WS_VISIBLE, IDC_STATIC, label);
    auto ctl = addControl (ctlCls, x + labelWidth, y, ctlWidth, ctlHeight, style, id, ctlText);
    y += 30;
    return ctl;
}
