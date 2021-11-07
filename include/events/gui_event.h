/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file gui_event.h
 * @date 2021-10-23
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/events/event.h"
#include "sml/events/system_events.h"

namespace Sgl
{
    enum GuiEventType
    {
        MOUSE_ENTERED = Sml::SML_EVENT_TYPE_FIRST_UNSPECIFIED,
        MOUSE_EXITED,

        ACTION_PERFORMED
    };

    enum GuiEventCategory
    {
        EVENT_CATEGORY_GUI = Sml::SML_EVENT_CATEGORY_FIRST_UNSPECIFIED
    };

    class MouseEnteredEvent : public Sml::MouseEvent
    {
    public:
        MouseEnteredEvent(int32_t x = 0, int32_t y = 0) : Sml::MouseEvent(x, y)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(MOUSE_ENTERED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI | Sml::MouseEvent::getStaticCategory())
    };

    class MouseExitedEvent : public Sml::MouseEvent
    {
    public:
        MouseExitedEvent(int32_t x = 0, int32_t y = 0) : Sml::MouseEvent(x, y)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(MOUSE_EXITED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI | Sml::MouseEvent::getStaticCategory())
    };

    class ActionEvent : public Sml::Event
    {
    public:
        ActionEvent(Sml::EventTarget* target = nullptr)
            : Sml::Event(getStaticType(), getStaticCategory(), target) {}

        DEFINE_STATIC_EVENT_TYPE(ACTION_PERFORMED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI)
    };

    class ActionListener : public Sml::Listener
    {
    public:
        virtual void onEvent(Sml::Event* event) override final
        {
            onActionPerformed(static_cast<ActionEvent*>(event));
        }

        virtual void onActionPerformed(ActionEvent* event) = 0;
    };
}