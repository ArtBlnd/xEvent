#pragma once

#ifndef XEVENT_DEF_H_
#define XEVENT_DEF_H_

#include "xEvent.h"
#include "xEventType.h"

#ifdef XEVENT_DLL
#define XEVENT_API __declspec(dllexport)
#else 
#define XEVENT_API __declspec(dllimport)
#endif //XEVENT_DLL

typedef typename ILinkable<MouseStatus, Position>* MouseEventPtr;
typedef typename ILinkable<KeyboardStatus, Key>* KeyboardEventPtr;

XEVENT_API MouseEventPtr getMouseEvent();
XEVENT_API KeyboardEventPtr getKeyboardEvent();
XEVENT_API bool RegisterToListener(HWND hWindow);

#endif //XEVENT_DEF_H_