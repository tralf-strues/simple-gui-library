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
        ACTION_PERFORMED,

        /* Drag & drop events */
        DRAG_START,
        DRAG_END,
        DRAG_MOVE
    };

    enum GuiEventCategory
    {
        EVENT_CATEGORY_GUI = Sml::SML_EVENT_CATEGORY_FIRST_UNSPECIFIED,
        EVENT_CATEGORY_DRAG_AND_DROP
    };

    //------------------------------------------------------------------------------
    // Event category [GUI]
    //------------------------------------------------------------------------------
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

            event->consume();
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

    template<typename C>
    class ActionListener : public Sml::Listener
    {
    public:
        ActionListener(C* control = nullptr) : m_Control(control) { }

        virtual void onEvent(Sml::Event* event) override final
        {
            onAction(dynamic_cast<ActionEvent*>(event));
            event->consume();
        }

        virtual void onAction(ActionEvent* event) = 0;
    
        C* getControl() { return m_Control; }
        void setControl(C* control) { m_Control = control; }

    private:
        C* m_Control;
    };

    //------------------------------------------------------------------------------
    // Event category [GUI->DRAG_AND_DROP]
    //------------------------------------------------------------------------------
    class DragEvent : public Sml::Event
    {
    public:
        DragEvent(Sml::EventTarget* target = nullptr) 
            : Sml::Event(getStaticType(), getStaticCategory(), target) {}

        DEFINE_STATIC_EVENT_TYPE(Sml::INVALID_EVENT_TYPE)
        DEFINE_STATIC_EVENT_CATEGORY(EVENT_CATEGORY_GUI | EVENT_CATEGORY_DRAG_AND_DROP)
    };

    class DragStartEvent : public DragEvent
    {
    public:
        DragStartEvent(Sml::EventTarget* target = nullptr)
            : DragEvent(target)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(DRAG_START)
        DEFINE_STATIC_EVENT_CATEGORY(DragEvent::getStaticCategory())
    };

    class DragEndEvent : public DragEvent
    {
    public:
        DragEndEvent(Sml::EventTarget* target = nullptr)
            : DragEvent(target)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        DEFINE_STATIC_EVENT_TYPE(DRAG_END)
        DEFINE_STATIC_EVENT_CATEGORY(DragEvent::getStaticCategory())
    };

    class DragMoveEvent : public DragEvent
    {
    public:
        DragMoveEvent(int32_t deltaX, int32_t deltaY, Sml::EventTarget* target = nullptr)
            : DragEvent(target), m_DeltaX(deltaX), m_DeltaY(deltaY)
        {
            m_Type = getStaticType();
            m_Category = getStaticCategory();
        }

        int32_t getDeltaX() const { return m_DeltaX; }
        int32_t getDeltaY() const { return m_DeltaY; }

        DEFINE_STATIC_EVENT_TYPE(DRAG_MOVE)
        DEFINE_STATIC_EVENT_CATEGORY(DragEvent::getStaticCategory())
    
    private:
        int32_t m_DeltaX = 0;
        int32_t m_DeltaY = 0;
    };

    class DragListener : public Sml::Listener
    {
        virtual void onEvent(Sml::Event* event) override final
        {
            switch (event->getType())
            {
                case DragStartEvent::getStaticType():
                {
                    onDragStart(dynamic_cast<DragStartEvent*>(event));
                    break;
                }

                case DragEndEvent::getStaticType():
                {
                    onDragEnd(dynamic_cast<DragEndEvent*>(event));
                    break;
                }

                case DragMoveEvent::getStaticType():
                {
                    onDragMove(dynamic_cast<DragMoveEvent*>(event));
                    break;
                }
            }

            event->consume();
        }

        virtual void onDragStart(DragStartEvent* event) {}
        virtual void onDragEnd(DragEndEvent* event) {}
        virtual void onDragMove(DragMoveEvent* event) {}
    };
}