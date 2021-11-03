//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file container.cpp
//! @date 2021-10-24
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "containers/container.h"

Component* Container::getHitComponent(int32_t x, int32_t y)
{
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        Component* hitComponent = (*it)->getHitComponent(x - m_X, y - m_Y);

        if (hitComponent != nullptr)
        {
            return hitComponent;
        }
    }

    if (Component::getHitComponent(x, y) == nullptr)
    {
        return nullptr;
    }

    return this;
}

void Container::updateGraphics()
{
    Component::updateGraphics();

    layOutComponents();

    for (Component* component : m_Components)
    {
        component->updateGraphics();
    }
}

void Container::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    if (!m_IsVisible)
    {
        return;
    }

    Component::render(target, targetRegion);

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(target);

    Rectangle<int32_t> translatedRegion = targetRegion;
    translatedRegion.pos += Vec2<int32_t>{m_X, m_Y};

    for (Component* component : m_Components)
    {
        if (component->isVisible())
        {
            component->render(target, translatedRegion);
        }
    }

    m_Renderer->setTarget(savedTarget);
}

// Vec2<int32_t> Container::getPrefferedLayoutSize()
// {
//     return Vec2<int32_t>{0, 0};
// }

// Vec2<int32_t> Container::getMinLayoutSize()
// {
//     return Vec2<int32_t>{0, 0};
// }

void Container::addComponent(Component* component)
{
    component->setParent(this);
    m_Components.push_back(component);

    layOutComponents();
}

void Container::removeComponent(Component* component)
{
    for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
    {
        if (*it == component)
        {
            (*it)->setParent(nullptr);
            m_Components.erase(it);
            break;
        }
    }

    layOutComponents();
}

const std::list<Component*>& Container::getComponents() const
{
    return m_Components;
}

void Container::layOutComponents()
{
    for (Component* component : m_Components)
    {
        if (component->isPrefSizeEnabled())
        {
            component->setWidth(component->getPrefWidth());
            component->setHeight(component->getPrefHeight());
        }
    }
}