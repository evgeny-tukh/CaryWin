#pragma once

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <functional>
#include <cstdint>
#include <memory>
#include <optional>

#include "windef.h"
#include "window.h"
#include "common.h"
#include "app.h"

namespace Cary {
    class WinClass {
        public:
            typedef std::function<LRESULT (HWND, UINT, WPARAM, LPARAM)> WinCb;
            WinClass (const char *_clsName, WinCb _winCb = [] (HWND _wnd, UINT _msg, WPARAM _wp, LPARAM _lp) {
                return _msg == WM_CREATE ? 1 : DefWindowProc (_wnd, _msg, _wp, _lp);
            });
            ~WinClass ();

            static WinClass *createCls (const char *_clsName, WinCb _winCb = [] (HWND, UINT, WPARAM, LPARAM) { return 0; });

            void setIcon (const char *icon) { cls.hIcon = LoadIcon (App::getInst (), icon); }
            void setIcon (const uint32_t icon) { setIcon (MAKEINTRESOURCE (icon)); }
            void setPublicIcon (const char *icon) { cls.hIcon = LoadIcon (nullptr, icon); }
            void setCursor (const char *cursor) { cls.hCursor = LoadCursor (cls.hInstance, cursor); }
            void setCursor (const uint32_t cursor) { setCursor (MAKEINTRESOURCE (cursor)); }
            void setPublicCursor (const char *cursor) { cls.hCursor = LoadIcon (nullptr, cursor); }
            void setBg (COLORREF clr);
            void setPublicBg (int brush);

            HINSTANCE getInstance () { return App::getInst (); }

            bool registerCls ();

            HWND createWnd (WinDef *winDef, std::function<void (Window *)> beforeCreate = [] (Window *) {}, HWND parent = HWND_DESKTOP);

        private:
            WNDCLASS cls;
            HBRUSH bg;
            WinCb winCb;
            void *param;

            static LRESULT wndProc (HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };
}