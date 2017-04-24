#include "xEvent.h"
#include "xEventListener.h"
#include "xEventType.h"
#include "xEventDef.h"

#include <Windows.h>

IEventFactory<IMouseEvent, IKeyboardEvent> factory;
IKeyboardEvent keyboardListener;
IMouseEvent mouseListener;


WNDPROC originalWndProc;
LRESULT CALLBACK xEventListener(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
        factory.DispatchEvent<IMouseEvent>(hwnd, uMsg, wParam, lParam);
        break;
    case WM_UNICHAR:
        factory.DispatchEvent<IKeyboardEvent>(hwnd, uMsg, wParam, lParam);
        break;
    }
    return CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam);
}

XEVENT_API bool RegisterToListener(HWND hWindow)
{
    factory.RegisterEvent(mouseListener);
    factory.RegisterEvent(keyboardListener);

    originalWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)xEventListener);
    return true;
}
XEVENT_API MouseEventPtr getMouseEvent()
{
    return &mouseListener;
}
XEVENT_API KeyboardEventPtr getKeyboardEvent()
{
    return &keyboardListener;
}