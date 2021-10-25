//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file frame.cpp
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "frame.h"

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

        Component* target = nullptr;

        if (nextEvent->isInCategory(EVENT_CATEGORY_WINDOW))
        {
            target = this;
        }
        else if (nextEvent->isInCategory(EVENT_CATEGORY_MOUSE))
        {
            MouseEvent* mouseEvent = static_cast<MouseEvent*>(nextEvent);
            target = getHitComponent(mouseEvent->getX(), mouseEvent->getY());
        }

        if (target != nullptr)
        {
            nextEvent->setTarget(target);
            fireEvent(nextEvent);
        }

        delete nextEvent;
    }
}

void Frame::show()
{
    for (Component* component : m_Components)
    {
        component->render(m_Texture);
    }

    render(nullptr);

    m_Renderer->present();
}