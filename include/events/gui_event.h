//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file gui_event.h
//! @date 2021-10-23
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef GUI_EVENT_H
#define GUI_EVENT_H

#include "sml/events/event.h"
#include "sml/events/system_events.h"

enum GuiEventType
{
    MOUSE_ENTERED = SML_EVENT_TYPE_FIRST_UNSPECIFIED,
    MOUSE_EXITED
};

class MouseEnteredEvent : public MouseEvent
{
public:
    MouseEnteredEvent(int32_t x = 0, int32_t y = 0) : MouseEvent(x, y)
    {
        m_Type = getStaticType();
        m_Category = getStaticCategory();
    }

    DEFINE_STATIC_EVENT_TYPE(MOUSE_ENTERED)
    DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)
};

class MouseExitedEvent : public MouseEvent
{
public:
    MouseExitedEvent(int32_t x = 0, int32_t y = 0) : MouseEvent(x, y)
    {
        m_Type = getStaticType();
        m_Category = getStaticCategory();
    }

    DEFINE_STATIC_EVENT_TYPE(MOUSE_EXITED)
    DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_INPUT | EVENT_CATEGORY_MOUSE)
};

#endif // GUI_EVENT_H