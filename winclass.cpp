#include "winclass.h"
#include "app.h"

Cary::WinClass::WinClass (const char *_clsName, WinCb _winCb): bg (nullptr) {
    memset (&cls, 0, sizeof (cls));
    cls.lpszClassName = _clsName;
    cls.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    cls.hCursor = (HCURSOR) LoadCursor (nullptr, IDC_ARROW);
    cls.hIcon = (HICON) LoadIcon (nullptr, IDI_APPLICATION);
    cls.hInstance = App::getInst ();
    cls.lpfnWndProc = wndProc;
    winCb = _winCb;
    cls.style = CS_HREDRAW | CS_VREDRAW;
    cls.lpszMenuName = nullptr;
}

Cary::WinClass::~WinClass () {
    if (bg) DeleteObject (bg);
}

Cary::WinClass *Cary::WinClass::createCls (const char *_clsName, WinCb _winCb) {
    auto cls = new WinClass (_clsName, _winCb);
    classes.emplace (std::pair<std::string, WinClass*> (_clsName, cls));
    return cls;
}

LRESULT Cary::WinClass::wndProc (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto wndInstPos = windows.find (wnd);

    auto msgProcessor = [wnd, msg, wParam, lParam] (UINT actualMsg, std::optional<MsgCb>& cb) -> std::optional<LRESULT> {
        std::optional<LRESULT> result;
        if (msg == actualMsg) {
            if (cb.has_value ()) result = cb.value () (wnd, msg, wParam, lParam);
        }
        return result;
    };

    if (wndInstPos == windows.end ()) {
        if (msg == WM_CREATE) {
            if (!windowPoll.empty ()) {
                if (windowPoll.back ()->onCreate.has_value ()) {
                    windowPoll.back ()->wnd = wnd;
                    windowPoll.back ()->onCreate.value () (wnd, msg, wParam, lParam);
                }
            }
        }
        return DefWindowProc (wnd, msg, wParam, lParam);
    }

    auto result = msgProcessor (WM_CREATE, wndInstPos->second->onCreate);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_DESTROY, wndInstPos->second->onDestroy);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_COMMAND, wndInstPos->second->onCommand);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_SYSCOMMAND, wndInstPos->second->onSysCommand);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_PAINT, wndInstPos->second->onPaint);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_MOVE, wndInstPos->second->onMove);
    if (result.has_value ()) return result.value ();
    result = msgProcessor (WM_SIZE, wndInstPos->second->onSize);
    if (result.has_value ()) return result.value ();

    return DefWindowProc (wnd, msg, wParam, lParam);
}

bool Cary::WinClass::registerCls () {
    auto atom = RegisterClassA (&cls);
    return (bool) atom;
}

void Cary::WinClass::setBg (COLORREF clr) {
    if (bg) DeleteObject (bg);
    bg = CreateSolidBrush (clr);
    cls.hbrBackground = bg;
}

void Cary::WinClass::setPublicBg (int brush) {
    if (bg) DeleteObject (bg);
    bg = nullptr;
    cls.hbrBackground = (HBRUSH) GetStockObject (brush);
}

void Cary::App::run () {
    MSG msg;

    while (GetMessage (&msg, nullptr, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
}

HWND Cary::WinClass::createWnd (Cary::WinDef *winDef, std::function<void (Window *)> beforeCreate, HWND parent) {
    auto wndInst = new Window (winDef);
    beforeCreate (wndInst);
    windowPoll.push_back (wndInst);
    wndInst->wnd = CreateWindowA (
        cls.lpszClassName,
        winDef->title.c_str (),
        winDef->style,
        winDef->x,
        winDef->y,
        winDef->width,
        winDef->height,
        parent,
        winDef->menu,
        cls.hInstance,
        param
    );
    windows.emplace (std::pair<HWND, Cary::Window *> (wndInst->wnd, wndInst));
    return wndInst->wnd;
}
