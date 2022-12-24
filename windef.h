#pragma once

#include <Windows.h>
#include <string>
#include "common.h"

namespace Cary {
    struct WinDef {
        std::string cls, title;
        int x, y, width, height;
        HMENU menu;
        UINT style;

        WinDef (const char *_cls, const char *_title = "", UINT _style = WS_OVERLAPPEDWINDOW | WS_VISIBLE):
            cls (_cls),
            title (_title),
            x (CW_USEDEFAULT),
            y (CW_USEDEFAULT),
            width (CW_USEDEFAULT),
            height (CW_USEDEFAULT),
            menu (nullptr),
            style (_style) {
        }
    };
}
