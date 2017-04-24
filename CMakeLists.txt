project(xEvent)

add_definitions(-DXEVENT_DLL)

set(xEvent_SRC_xEvent_CPP 
        xEvent.cpp
        xEventListener.cpp)
set(xEvent_SRC_xEvent_H
        xEvent.h
        xEventListener.h
        xEventType.h)

add_library(xEvent SHARED
        ${xEvent_SRC_xEvent_CPP}
        ${xEvent_SRC_xEvent_H})