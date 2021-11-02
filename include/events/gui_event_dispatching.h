//------------------------------------------------------------------------------
//! The event proccessing is split into several stages.
//! 
//! -# Creating @ref GuiEvent.
//!    In most cases, this is done internally by the system. The system
//!    registers a system event (e.g. MouseMoved, KeyPressed, etc.).
//! -# Determining the @ref EventTarget.
//!    Then the system determines to which component the event should be
//!    sent. Some examples include:
//!    - A keyboard event is sent to the currently focused component. 
//!    - A mouse event is sent to the component on which the cursor is pointed.
//!    
//! -# Building a @ref GuiEventDispatchChain from event target.
//!    The system then calls @ref fireEvent(GuiEvent*) function with the
//!    event target determined by the previous stage. @ref fireEvent(GuiEvent*)
//!    builds a dispatch chain and sends the event through it.
//!
//! -# Capturing phase.
//!     /------\     /------------\             /------------------\
//!     | Root | --> | Component1 | --> ... --> | Target component |
//!     \------/     \------------/             \------------------/
//!    Dispatcher      Dispatcher                    Dispatcher
//!     Filter1         Filter1                       Filter1 
//!     ...             ...                           ...     
//!     FilterN         FilterN                       FilterN 
//!
//!    The next stage is moving the event from the root to the target. Each
//!    component has its own @ref GuiEventDispatcher which calls filters on the
//!    event. A filter can consume the event and then the dispatching proccess
//!    terminates (though all of the current dispatcher's filters are called
//!    nonetheless).
//!    
//! -# Bubbling phase.
//!     /------\     /------------\             /------------------\
//!     | Root | --> | Component1 | --> ... --> | Target component |
//!     \------/     \------------/             \------------------/
//!    Dispatcher      Dispatcher                    Dispatcher
//!     Handler1        Handler1                      Handler1 
//!     ...             ...                           ...     
//!     HandlerN        HandlerN                      HandlerN 
//!
//!    The next stage is moving the event from the target to the root. Here each
//!    dispatcher calls handlers on the event. A handler can consume the event
//!    and then the dispatching proccess terminates (though all of the current
//!    dispatcher's handlers are called nonetheless). Most target components
//!    consume the event and the bubbling phase doesn't occur.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_event_dispatching.h
//! @date 2021-10-23
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef GUI_EVENT_DISPATCHING_H
#define GUI_EVENT_DISPATCHING_H

#include <vector>
#include "sml/events/event_dispatching.h"
#include "sml/events/listener_notifier.h"
#include "sml/events/system_events.h"
#include "gui_event.h"

class GuiEventDispatchChain;

class GuiEventDispatcher : public EventDispatcher
{
public:
    ~GuiEventDispatcher() = default;

    /* Listeners */
    void attachFilter(const std::initializer_list<EventType>& types, Listener* filter);    
    void attachHandler(const std::initializer_list<EventType>& types, Listener* handler);    

    /* EventDispatcher */
    virtual Event* dispatchEvent(Event* event, EventDispatchChain* chain) override;

protected:
    Notifier m_FiltersNotifier;
    Notifier m_HandlersNotifier;

    virtual Event* dispatchCapturingEvent(Event* event);
    virtual Event* dispatchBubblingEvent(Event* event);
};

class GuiEventDispatchChain : public EventDispatchChain
{
public:
    GuiEventDispatchChain(size_t defaultDispatchersCount = 8);

    void reset();

    /* EventDispatchChain */
    virtual void prependDispatcher(EventDispatcher* dispatcher) override;
    virtual Event* sendThroughChain(Event* event) override;

private:
    std::vector<GuiEventDispatcher*> m_Dispatchers;
    uint32_t                         m_ActiveCount;
};

//------------------------------------------------------------------------------
//! @brief Fire the @ref event from the specified @code event->target @endcode.
//! 
//! Build a @ref GuiEventDispatchChain from target to the root component and
//! send the event through it.
//! 
//! @param event 
//!
//! @return The return event or nullptr if the event has been consumed.
//------------------------------------------------------------------------------
Event* fireEvent(Event* event);

#endif // GUI_EVENT_DISPATCHING_H