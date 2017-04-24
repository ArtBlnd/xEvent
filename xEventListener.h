#pragma once

#ifndef XEVENT_LISTNER_H_
#define XEVENT_LISTNER_H_

#include "xEvent.h"
#include "xEventType.h"

#pragma warning(push)
#pragma warning(disable:4700)

// Recv Raw Windows Event.
template<typename EventType>
class RawEvent : public IEvent<HWND, UINT, WPARAM, LPARAM>
{
private:
    struct
    {
        HWND        hWindow;
        UINT        message;
        WPARAM      wParam;
        LPARAM      lParam;
    } LastestMessage;

    // recent message.
    LastestMessage lastMessage;

protected:
    HWND   getWindowHandle()  const { return lastMessage.hWindow; }
    UINT   getWindowMessage() const { return lastMessage.message; }
    WPARAM getWindowWParam()  const { return lastMessage.wParam;  }
    LPARAM getWindowLParam()  const { return lastMessage.lParam;  }

public:
    RawEvent() {}
    virtual ~RawEvent() {}
    // Raw Event Base.
    virtual bool Dispatch(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // Store Event Message;
        lastMessage.hWindow = hWindow;
        lastMessage.message = message;
        lastMessage.wParam  = wParam;
        lastMessage.lParam  = lParam;

        return true;
    }
};

class IMouseEvent : public RawEvent<EventType>, public ILinkable<MouseStatus, Position>
{
private:
    typedef typename RawEvent<EventType>                 Original;
    typedef typename ILinkable<MouseStatus, Position>    Linkable;
public:
    IMouseEvent() {}
    virtual ~IMouseEvent() {}

    virtual bool Dispatch(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // Infomations
        Position      currentPosition;
        MouseStatus   currentStatus;

        // Get Infomations from wParam, lParam;
        lParamToPosition(lParam, currentPosition);
        wParamToStatus(wParam, currentStatus);

        // Pre dispatching.
        Original::Dispatch(hWindow, message, wParam, lParam);
        Linkable::Dispatch(currentStatus, currentPosition);
       
        return onMouseEvent(currentStatus, currentPosition);
    }
    virtual bool onMouseEvent(MouseStatus status, Position position)
    {
        return true;
    }
};

class IKeyboardEvent : public RawEvent<EventType>, public ILinkable<KeyboardStatus, Key>
{
private:    
    typedef typename RawEvent<EventType>             Original;
    typedef typename ILinkable<KeyboardStatus, Key>  Linkable;
public:
    IKeyboardEvent() {}
    virtual ~IKeyboardEvent() {}

    virtual bool Dispatch(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Key            currentKey;
        KeyboardStatus currentStatus;

        // Pre dispatching.
        Original::Dispatch(hWindow, message, wParam, lParam);
        Linkable::Dispatch(currentStatus, currentKey);

        return onKeyboardEvent(currentStatus, currentKey);
    }

    virtual bool onKeyboardEvent(KeyboardStatus status, Key key)
    {
        return true;
    }
};

#pragma warning(pop)
#endif