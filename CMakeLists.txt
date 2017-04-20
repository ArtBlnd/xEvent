PROJECT(xEvent)

SET(xEvent_SRC_xEvent_CPP 
        xEvent.cpp
        xEventListener.cpp)
SET(xEvent_SRC_xEvent_H
        xEvent.h
        xEventListener.h
        xEventType.h)

ADD_LIBRARY(xEvent SHARED
        ${xEvent_SRC_xEvent_CPP}
        ${xEvent_SRC_xEvent_H})