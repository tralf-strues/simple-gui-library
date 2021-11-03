//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file frame.cpp
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "containers/frame.h"

Frame::Frame(int32_t width, int32_t height, const char* title)
    : m_Window(width, height, title)
{
    setRenderer(new Renderer(m_Window));
    setSize(width, height);
}

void Frame::proccessSystemEvents()
{
    Event* nextEvent = nullptr;

    while ((nextEvent = m_SystemEventManager.pollEvent(false)) != nullptr)
    {
        if (nextEvent->getType() == INVALID_EVENT_TYPE)
        {
            continue;
        }

        if (nextEvent->isInCategory(EVENT_CATEGORY_WINDOW))
        {
            proccessWindowEvent(static_cast<WindowEvent*>(nextEvent));
        }
        else if (nextEvent->isInCategory(EVENT_CATEGORY_MOUSE))
        {
            proccessMouseEvent(static_cast<MouseEvent*>(nextEvent));
        }
        else if (nextEvent->isInCategory(EVENT_CATEGORY_KEYBOARD))
        {
            proccessKeyboardEvent(static_cast<KeyEvent*>(nextEvent));
        }

        delete nextEvent;
    }
}

void Frame::proccessWindowEvent(WindowEvent* windowEvent)
{
    assert(windowEvent);

    windowEvent->setTarget(this);
    fireEvent(windowEvent);
}

void Frame::proccessKeyboardEvent(KeyEvent* keyEvent)
{
    assert(keyEvent);
}

void Frame::proccessMouseEvent(MouseEvent* mouseEvent)
{
    assert(mouseEvent);

    Component* curHoverComponent = getHitComponent(mouseEvent->getX(), mouseEvent->getY());
    
    if (curHoverComponent != nullptr)
    {
        mouseEvent->setTarget(curHoverComponent);
        fireEvent(mouseEvent);
    }

    if (mouseEvent->getType() == MouseMovedEvent::getStaticType() &&
        m_PrevMouseHoverComponent != curHoverComponent)
    {
        if (m_PrevMouseHoverComponent != nullptr)
        {
            MouseExitedEvent mouseExitedEvent{mouseEvent->getX(), mouseEvent->getY()};
            mouseExitedEvent.setTarget(m_PrevMouseHoverComponent);
            fireEvent(&mouseExitedEvent);
        }

        if (curHoverComponent != nullptr)
        {
            MouseEnteredEvent mouseEnteredEvent{mouseEvent->getX(), mouseEvent->getY()};
            mouseEnteredEvent.setTarget(curHoverComponent);
            fireEvent(&mouseEnteredEvent);
        }
    }

    m_PrevMouseHoverComponent = curHoverComponent;
}


void Frame::show()
{
    render(nullptr, Rectangle<int32_t>{Vec2<int32_t>{0, 0}, m_Size.x, m_Size.y});

    m_Renderer->present();
}

Window* Frame::getWindow()
{
    return &m_Window;
}