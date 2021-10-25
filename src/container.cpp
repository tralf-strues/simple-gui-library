//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file container.cpp
//! @date 2021-10-24
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "container.h"

void Container::updateGraphics()
{
    if (m_Texture == nullptr)
    {
        return;
    }

    Texture* savedTarget = m_Renderer->getTarget();

    m_Renderer->setTarget(m_Texture);
    
    m_Renderer->setColor(m_Background);
    m_Renderer->clear();

    for (Component* component : m_Components)
    {
        component->updateGraphics();
    }

    m_Renderer->setTarget(savedTarget);
}

Component* Container::getHitComponent(int32_t x, int32_t y)
{
    if (Component::getHitComponent(x, y) == nullptr)
    {
        return nullptr;
    }

    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        Component* hitComponent = (*it)->getHitComponent(x - m_X, y - m_Y);

        if (hitComponent != nullptr)
        {
            return hitComponent;
        }
    }

    return this;
}

Dimension Container::getPrefferedLayoutSize()
{
    return Dimension{0, 0};
}

Dimension Container::getMinLayoutSize()
{
    return Dimension{0, 0};
}

void Container::addComponent(Component* component)
{
    component->setParent(this);
    m_Components.pushBack(component);
}

void Container::removeComponent(Component* component)
{
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        if (*it == component)
        {
            (*it)->setParent(nullptr);
            m_Components.remove(it);
            break;
        }
    }
}

const List<Component*>& Container::getComponents() const
{
    return m_Components;
}

void Container::layOutComponents()
{
}