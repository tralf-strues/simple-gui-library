//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file container.cpp
//! @date 2021-10-24
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "container.h"

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

void Container::updateGraphics()
{
    Component::updateGraphics();

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(m_Texture);
    
    for (Component* component : m_Components)
    {
        component->updateGraphics();
    }

    m_Renderer->setTarget(savedTarget);
}

void Container::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    Component::render(target, targetRegion);

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(target);

    Rectangle<int32_t> translatedRegion = targetRegion;
    translatedRegion.pos += Vec2<int32_t>{m_X, m_Y};

    for (Component* component : m_Components)
    {
        component->render(target, translatedRegion);
    }

    m_Renderer->setTarget(savedTarget);
}

Vec2<int32_t> Container::getPrefferedLayoutSize()
{
    return Vec2<int32_t>{0, 0};
}

Vec2<int32_t> Container::getMinLayoutSize()
{
    return Vec2<int32_t>{0, 0};
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