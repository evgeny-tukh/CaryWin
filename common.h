#pragma once

#include <Windows.h>
#include <unordered_map>
#include <vector>
#include <functional>

#define IDC_STATIC (-1)

#define IDC_PORT        100
#define IDC_BAUD        101
#define IDC_BRG         102
#define IDC_BRG_SPIN    103
#define IDC_ELEV        104
#define IDC_ELEV_SPIN   105
#define IDC_SEND        106
#define IDC_START_STOP  107
#define IDC_TERMINAL    108
#define IDC_ID          109
#define IDC_ID_SPIN     110
#define IDC_ENABLE      111
#define IDC_INIT        112
#define IDC_MODE        113
#define IDC_RANGE       114
#define IDC_RANGE_SPIN  115

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