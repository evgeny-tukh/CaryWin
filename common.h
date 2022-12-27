#pragma once

#include <Windows.h>
#include <unordered_map>
#include <vector>
#include <functional>

#define IDC_STATIC (-1)

namespace Cary {
    typedef std::function<LRESULT (HWND, UINT, WPARAM, LPARAM)> MsgCb;

    inline std::unordered_map<std::string, class WinClass *> classes;
    inline std::unordered_map<HWND, class Window *> windows;
    inline std::vector<class Window *> windowPoll;

    inline auto findClass (std::string clsName) {
        auto pos = classes.find (clsName);
        return pos == classes.end () ? nullptr : pos->second;
    }
    inline auto findWindow (HWND wnd) {
        auto pos = windows.find (wnd);
        return pos == windows.end () ? nullptr : pos->second;
    }
}