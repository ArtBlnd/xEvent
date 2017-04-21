#include "xEvent.h"
#include "xEventListener.h"
#include "xEventType.h"

#include <Windows.h>

MouseEvent                          mouseEvent;
IEventFactory<MouseEvent>           factory;

bool onLButtonDown(Position pos)
{
    return true;
}
bool onRButtonDown(Position pos)
{
    return true;
}

WNDPROC originalWndProc;
LRESULT CALLBACK xEventListener(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    factory.DispatchEvent<MouseEvent>(hwnd, uMsg, wParam, lParam);
    return CallWindowProc(originalWndProc, hwnd, uMsg, wParam, lParam);
}

bool RegisterToListener(HWND hWindow)
{
    mouseEvent.Link(onLButtonDown, MouseStatus::LButtonDown);
    mouseEvent.Link(onRButtonDown, MouseStatus::RButtonDown);
    factory.RegisterEvent(mouseEvent);


    originalWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)xEventListener);
    return true;
}