#pragma once

#ifndef XEVENT_H_
#define XEVENT_H_

#include <vector>
#include <functional>

template<class... ArgumentType>
class IDispatchable
{
protected:
    virtual bool Dispatch(ArgumentType...) = 0;
public:
    bool operator()(ArgumentType... Arguments)
    {
        return Dispatch(Arguments...);
    }
};

// Basic IEvent Implementation.
template<class... ArgumentType>
class IEvent : public IDispatchable<ArgumentType...>
{
public:
    // Original IEvent Type(with Original Template ArgumentType)
    using event_type = IEvent;

    IEvent() {}
    virtual ~IEvent() = default;
};

// Function Linkable IEvent Implementation
template<typename EventType, typename... ArgumentType>
class ILinkable : public IDispatchable<EventType, ArgumentType...>
{
    typedef bool (FuncType)(ArgumentType...);
    typedef std::function<FuncType> FunctionType;

    typedef struct __FunctionObject
    {
        FunctionType Proc;
        EventType    Type;
    } FunctionObject;
    
    // Function that linked.
    std::vector<FunctionObject> LinkedProcs;
protected:
    bool Dispatch(EventType Type, ArgumentType... Arguments)
    {
        for(const FunctionObject& object : LinkedProcs)
        {
            if(object.Type != Type) 
            {
                continue;
            }
            object.Proc(Arguments...);
        }

        return true;
    }
public:
    // Link function to this Event Interface.
    void Link(const FunctionType& function, EventType Type)
    {
        FunctionObject Object;

        Object.Proc = function;
        Object.Type = Type;

        LinkedProcs.push_back(Object);
    }
};

// IEventChunk predefine.
template<
    typename IEventType, 
    typename IOriginalEvent
>
class IEventChunk;

// IEventChunk Implementation.
template<typename EventType, typename... ArgumentType>
class IEventChunk<EventType, IEvent<ArgumentType...>>
{
private:
    // Cancelable Events
    std::vector<EventType>     m_PreEvents;
    // None-Cancelable Events
    std::vector<EventType>     m_Events;

    virtual bool CheckEventVailed(const EventType& event)
    {
        return true;
    }

public:
    // NOTE : PreEvent CAN BE CANCLED WHILE DISPATCHING.
    // Register Prefetch Event Interface
    void RegisterPreEvent(EventType& eventObject)
    {
        m_PreEvents.push_back(eventObject);
    }
    // Register Event Interface.
    void RegisterEvent(EventType& eventObject)
    {
        m_Events.push_back(eventObject);
    }
    // Dispatch all event that registered on IEventChunk.
    bool DispatchEvent(ArgumentType... Arguments);
};

// Dispatch Event Implementation.
template <typename EventType, typename ...ArgumentType>
bool IEventChunk<EventType, IEvent<ArgumentType...>>::DispatchEvent(ArgumentType... Arguments)
{
    bool isSuccessed = false;

    // Arguments : 
    //      object : vector object that we are going to Dispatch.
    //      fKeepExcute : boolean that keep execute if Dispatch return false.
    auto tryDispatch = [this, Arguments...](auto& object, const bool fKeepExcute) -> bool
    {
        if (object.empty() == true)
        {
            return true;
        }
        for (unsigned i = 0; i < object.size(); ++i)
        {
            EventType& eventObject = object[i];

            if (CheckEventVailed(eventObject) == false)
            {
                continue;
            }
            if (eventObject(Arguments...) == false && fKeepExcute == false)
            {
                // if fKeeExecute is false. Stop execute if failed.
                return false;
            }
        }
        return true;
    };

    // Try PreEvents First. if its Canceled(false return). don't dispatch None-Cancelable Events first.
    if (tryDispatch(m_PreEvents, false))
    {
        isSuccessed = true;
        tryDispatch(m_Events, true);
    }
    return isSuccessed;
}

// IEventFactory can register multiple Event Interface Type Chunks.
template<typename ...IEventTypes>
class IEventFactory : 
    public IEventChunk<
        typename IEventTypes, 
        typename IEventTypes::event_type>...
{
public:
    // Register Prefetch Event Interface.
    template<class EventType>
    void RegisterPreEvent(EventType& eventObject)
    {
        IEventChunk<
            EventType, 
            EventType::event_type>::RegisterPreEvent(eventObject);
    }
    // Register Event Interface.
    template<class EventType>
    void RegisterEvent(EventType& eventObject)
    {
        IEventChunk<
            EventType, 
            EventType::event_type>::RegisterEvent(eventObject);
    }
    // Dispatch all Events that registered in Factory.
    template<class EventType, class ...ArgumentType>
    bool DispatchEvent(ArgumentType... Arguments)
    {
        return IEventChunk<
            EventType, 
            EventType::event_type>::DispatchEvent(Arguments...);
    }
};

#endif