#pragma once

#ifndef XEVENT_LISTNER_H_
#define XEVENT_LISTNER_H_

#include "xEvent.h"
#include "xEventType.h"

// Recv Raw Windows Event.
template<typename EventType>
class RawEvent : public IEvent<HWND, UINT, WPARAM, LPARAM>
{
private:
    typedef struct __LastestMessage
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
    bool Dispatch(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam) 
    {
        // Store Event Message;
        lastMessage.hWindow = hWindow;
        lastMessage.message = message;
        lastMessage.wParam  = wParam;
        lastMessage.lParam  = lParam;

        return true;
    }

    virtual EventType getEventType() = 0;
};

class MouseEvent : public RawEvent<EventType>, public ILinkable<MouseStatus, Position>
{
private:
    typedef RawEvent<EventType>              OriginalEvent;
    typedef ILinkable<MouseStatus, Position> OriginalLinkable;

public:
    MouseEvent() {}
    virtual ~MouseEvent() {}

    bool Dispatch(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
    {
        // Calling Original.
        OriginalEvent::Dispatch(hWindow, message, wParam, lParam);

        // Infomations
        Position      currentPosition;
        MouseStatus   currentStatus;

        // Get Infomations from wParam, lParam;
        lParamToPosition(lParam, currentPosition);
        wParamToStatus(wParam, currentStatus);

        OriginalLinkable::Dispatch(currentStatus, currentPosition);
        return onMouseEvent(currentStatus, currentPosition);
    }
    virtual bool onMouseEvent(MouseStatus status, Position position)
    {
        return true;
    }
    EventType getEventType() override
    {
        return EventType::TypeMouseEvent;
    }
};

#endif