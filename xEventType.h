#pragma once

#ifndef XEVENT_TYPE_H_
#define XEVENT_TYPE_H_

#include <Windows.h>

typedef enum class __EventType
{
    TypeMouseEvent,
    TypeKeyboardEvent
} EventType;

typedef enum class __MouseStatus
{
    Move            = WM_MOUSEMOVE,
    LButtonDown     = WM_LBUTTONDOWN,
    LButtonUp       = WM_LBUTTONUP,
    RButtonDown     = WM_RBUTTONDOWN,
    RButtonUp       = WM_RBUTTONUP
} MouseStatus;

typedef struct __Position
{
    int x;
    int y;
} Position;

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