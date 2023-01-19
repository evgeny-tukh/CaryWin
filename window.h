#pragma once

#include <Windows.h>
#include <cstdint>
#include <optional>
#include <functional>
#include <unordered_map>

#include "common.h"
#include "windef.h"
#include "common.h"

namespace Cary {
    struct Window {
        HWND wnd;
        class WinClass *cls;
        WinDef winDef;
        std::optional<MsgCb> onCreate, onDestroy, onCommand, onSysCommand, onPaint, onMove, onSize, onTimer;
        std::unordered_map<uint32_t, MsgCb> userMsgHandlers;

        void addUserMsgHandler (uint32_t msg, MsgCb cb) {
            userMsgHandlers.emplace (std::pair<uint32_t, MsgCb> (msg, cb));
        }

        Window (WinDef *_winDef);

        HWND addControl (const char *cls, int x, int y, int width, int height, uint32_t style, uint64_t id = IDC_STATIC, const char *text = "");
        HWND createLabeledControl (
            const char *ctlClass,
            const char *label,
            const uint32_t style,
            uint32_t id,
            int x,
            int& y,
            int labelWidth,
            int ctlWidth,
            int ctlHeight = 22,
            const char *ctlText = ""
        );
        HWND createUpDownButton (HWND editCtl, int min, int max, int value);
    };
}