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

        FOCUS_CHANGED,
        ACTION_PERFORMED
    };

    enum GuiEventCategory
    {
        EVENT_CATEGORY_GUI = Sml::SML_EVENT_CATEGORY_FIRST_UNSPECIFIED
    };

    class MouseEnteredEvent : public Sml::MouseEvent
    {
    public:
        MouseEnteredEvent(int32_t x = 0, int32_t y = 0, Sml::EventTarget* target = nullptr)
            : Sml::MouseEvent(x, y, target)
        {
            m_Type     = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(MOUSE_ENTERED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI | Sml::MouseEvent::getStaticCategory())
    };

    class MouseExitedEvent : public Sml::MouseEvent
    {
    public:
        MouseExitedEvent(int32_t x = 0, int32_t y = 0, Sml::EventTarget* target = nullptr)
            : Sml::MouseEvent(x, y, target)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(MOUSE_EXITED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI | Sml::MouseEvent::getStaticCategory())
    };

    class FocusEvent : public Sml::Event
    {
    public:
        enum class Type
        {
            FOCUS_GOT,
            FOCUS_LOST
        };

    public:
        FocusEvent(Type type, Sml::EventTarget* target = nullptr)
            : Sml::Event(getStaticType(), getStaticCategory(), target), m_Type(type) {}

        bool gotFocus() const { return m_Type == Type::FOCUS_GOT; }
        bool lostFocus() const { return m_Type == Type::FOCUS_LOST; }

        DEFINE_STATIC_EVENT_TYPE(FOCUS_CHANGED)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI)

    private:
        const Type m_Type;
    };

    class FocusListener : public Sml::Listener
    {
    public:
        virtual void onEvent(Sml::Event* event) override final
        {
            FocusEvent* focusEvent = dynamic_cast<FocusEvent*>(event);

            if (focusEvent->gotFocus())
            {
                onFocusGot(focusEvent);
            }
            else
            {
                onFocusLost(focusEvent);
            }
        }

        virtual void onFocusGot(FocusEvent* event) {}
        virtual void onFocusLost(FocusEvent* event) {}
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
            onAction(dynamic_cast<ActionEvent*>(event));
        }

        virtual void onAction(ActionEvent* event) = 0;
    };
}