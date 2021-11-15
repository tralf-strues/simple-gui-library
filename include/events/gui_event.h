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
#include "../core.h"

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

    #define DEFINE_STATIC_LISTENED_EVENT_TYPES(...) \
        static constexpr std::initializer_list<Sml::EventType> EVENT_TYPES{__VA_ARGS__};

    #define DEFINE_STATIC_LISTENED_EVENT_TYPES_FROM_BASE_CLASS(baseClass) \
        static constexpr std::initializer_list<Sml::EventType> EVENT_TYPES = baseClass::EVENT_TYPES;

    //------------------------------------------------------------------------------
    // Event category [GUI]
    //------------------------------------------------------------------------------
    template<typename C>
    class ComponentEventListener : public Sml::Listener
    {
    public:
        ComponentEventListener(C* component) : m_Component(component) {}
        C* getComponent() { return m_Component; }
        void setComponent(C* component) { m_Component = component; }

    private:
        C* m_Component = nullptr;
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

    template<typename C>
    class HoverListener : public ComponentEventListener<C>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(MouseEnteredEvent::getStaticType(),
                                           MouseExitedEvent::getStaticType())
    public:
        HoverListener(C* component) : ComponentEventListener<C>(component) {}

        virtual void onEvent(Sml::Event* event) override final
        {
            switch (event->getType())
            {
                case MouseEnteredEvent::getStaticType():
                {
                    onMouseEntered(dynamic_cast<MouseEnteredEvent*>(event));
                    break;
                }

                case MouseExitedEvent::getStaticType():
                {
                    onMouseExited(dynamic_cast<MouseExitedEvent*>(event));
                    break;
                }

                default:
                {
                    LOG_ERROR("HoverListener got invalid Event with type %" PRIu32, event->getType());
                    break;
                }
            };

            event->consume();
        }

        virtual void onMouseEntered(MouseEnteredEvent* event) {}
        virtual void onMouseExited(MouseExitedEvent* event) {}

    private:
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

    template<typename C>
    class FocusListener : public ComponentEventListener<C>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(FocusEvent::getStaticType())

    public:
        FocusListener(C* component) : ComponentEventListener<C>(component) {}

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
    class ActionListener : public ComponentEventListener<C>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(ActionEvent::getStaticType())

    public:
        ActionListener(C* component = nullptr) : ComponentEventListener<C>(component) {}

        virtual void onEvent(Sml::Event* event) override final
        {
            onAction(dynamic_cast<ActionEvent*>(event));
            event->consume();
        }

        virtual void onAction(ActionEvent* event) = 0;
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

    template<typename C>
    class DragListener : public ComponentEventListener<C>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(DragStartEvent::getStaticType(),
                                           DragEndEvent::getStaticType(),
                                           DragMoveEvent::getStaticType())

    public:
        DragListener(C* component) : ComponentEventListener<C>(component) {}

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