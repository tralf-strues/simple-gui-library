/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file gui_event_dispatching.cpp
 * @date 2021-10-24
 * 
 * @copyright Copyright (c) 2021
 */

#include <assert.h>
#include "events/gui_event_dispatching.h"
#include "scene/component.h"

Sml::Event* fireEvent(Sml::Event* event)
{
    assert(event);
    assert(event->getTarget());

    static GuiEventDispatchChain baseDispatchChain;

    GuiEventDispatchChain* targetDispatchChain = 
        dynamic_cast<GuiEventDispatchChain*>(event->getTarget()->buildEventDispatchChain(&baseDispatchChain));

    Sml::Event* returnEvent = targetDispatchChain->sendThroughChain(event);
    baseDispatchChain.reset();
    
    return returnEvent;
}

//------------------------------------------------------------------------------
// GuiEventDispatcher 
//------------------------------------------------------------------------------
void GuiEventDispatcher::attachFilter(const std::initializer_list<Sml::EventType>& types,
                                      Sml::Listener* filter)
{
    assert(filter);
    m_FiltersNotifier.attachListener(types, filter);
}

void GuiEventDispatcher::detachFilter(Sml::Listener* filter)
{
    assert(filter);
    m_FiltersNotifier.detachListener(filter);
}

void GuiEventDispatcher::attachHandler(const std::initializer_list<Sml::EventType>& types,
                                       Sml::Listener* handler)
{
    assert(handler);
    m_HandlersNotifier.attachListener(types, handler);
}

void GuiEventDispatcher::detachHandler(Sml::Listener* handler)
{
    assert(handler);
    m_HandlersNotifier.detachListener(handler);
}

Sml::Event* GuiEventDispatcher::dispatchEvent(Sml::Event* event, Sml::EventDispatchChain* chain)
{
    assert(event);
    assert(chain);
    
    event = dispatchCapturingEvent(event);

    if (event->isConsumed())
    {
        return nullptr;
    }

    // Send forward down the chain
    event = chain->sendThroughChain(event);

    // If the event hasn't been consumed
    if (event != nullptr)
    {
        event = dispatchBubblingEvent(event);
        
        if (event->isConsumed())
        {
            return nullptr;
        }
    }

    return event;
}

Sml::Event* GuiEventDispatcher::dispatchCapturingEvent(Sml::Event* event)
{
    assert(event);
    m_FiltersNotifier.notify(event);

    return event;
}

Sml::Event* GuiEventDispatcher::dispatchBubblingEvent(Sml::Event* event)
{
    assert(event);
    m_HandlersNotifier.notify(event);

    return event;
}

//------------------------------------------------------------------------------
// GuiEventDispatchChain
//------------------------------------------------------------------------------
GuiEventDispatchChain::GuiEventDispatchChain(size_t defaultDispatchersCount)
    : m_Dispatchers(defaultDispatchersCount), m_ActiveCount(0) {}

void GuiEventDispatchChain::reset()
{
    for (auto it = m_Dispatchers.begin(); it != m_Dispatchers.end(); ++it)
    {
        *it = nullptr;
    }

    m_Dispatchers.clear();
    m_ActiveCount = 0;
}

void GuiEventDispatchChain::prependDispatcher(Sml::EventDispatcher* dispatcher)
{
    assert(dispatcher);

    if (m_ActiveCount >= m_Dispatchers.size())
    {
        m_Dispatchers.push_back(dynamic_cast<GuiEventDispatcher*>(dispatcher));
    }
    else
    {
        m_Dispatchers[m_ActiveCount] = dynamic_cast<GuiEventDispatcher*>(dispatcher);
    }

    ++m_ActiveCount;
}

Sml::Event* GuiEventDispatchChain::sendThroughChain(Sml::Event* event)
{
    assert(event);

    if (m_ActiveCount == 0)
    {
        return event;
    }

    uint32_t savedActiveCount = m_ActiveCount;

    GuiEventDispatcher* nextDispatcher = m_Dispatchers[--m_ActiveCount];
    Sml::Event* returnEvent = nextDispatcher->dispatchEvent(event, this);

    m_ActiveCount = savedActiveCount;

    return returnEvent;
}