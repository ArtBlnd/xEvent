#pragma once

#ifndef XEVENT_TYPE_H_
#define XEVENT_TYPE_H_

#include <Windows.h>

enum class EventType
{
    TypeMouseEvent,
    TypeKeyboardEvent
};

enum class MouseStatus
{
    Move            = WM_MOUSEMOVE,
    LButtonDown     = WM_LBUTTONDOWN,
    LButtonUp       = WM_LBUTTONUP,
    RButtonDown     = WM_RBUTTONDOWN,
    RButtonUp       = WM_RBUTTONUP
};

enum class KeyboardStatus
{

};

struct Position
{
    int x;
    int y;
};

struct Key
{
    wchar_t KeyCode;
};

inline void wParamToStatus(const LPARAM lParam, MouseStatus& status)
{
    status = (MouseStatus)lParam;
}
inline void lParamToPosition(const WPARAM wParam, Position& position)
{
    position.x = LOWORD(wParam);
    position.y = HIWORD(wParam);
}

#endif