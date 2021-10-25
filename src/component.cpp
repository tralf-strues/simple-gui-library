//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file component.cpp
//! @date 2021-10-18
//!
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include "component.h"

Component* Component::getHitComponent(int32_t x, int32_t y)
{
    if (x >= m_X && x < m_X + m_Size.width && y >= m_Y && y < m_Y + m_Size.height)
    {
        return this;
    }

    return nullptr;
}

void Component::render(Texture* target)
{
    Texture* savedTarget = m_Renderer->getTarget();

    m_Renderer->setTarget(target);
    Rectangle region{Vec2<int32_t>{m_X, m_Y}, m_Size.width, m_Size.height};
    renderTexture(*m_Renderer, *m_Texture, region);

    m_Renderer->setTarget(savedTarget);
}

EventDispatchChain* Component::buildEventDispatchChain(EventDispatchChain* chain)
{
    assert(chain);

    chain->prependDispatcher(&m_Dispatcher);

    if (m_Parent == nullptr)
    {
        return chain;
    }
    
    return m_Parent->buildEventDispatchChain(chain);
}

//------------------------------------Setters-----------------------------------
void Component::setRenderer(Renderer* renderer)
{
    m_Renderer = renderer;
}

void Component::setId(const id128_t &id)
{
    m_Id = id;
}

void Component::setParent(Component* parent)
{
    assert(parent);
    m_Parent = parent;
}

void Component::setEventDispatcher(GuiEventDispatcher* dispatcher)
{
    assert(dispatcher);
    m_Dispatcher = *dispatcher;
}

void Component::setVisible(bool visible)
{ 
    m_IsVisible = visible;
}

void Component::setX(int32_t x)
{
    m_X = x;
}

void Component::setY(int32_t y)
{
    m_Y = y;
}

void Component::setSize(int32_t width, int32_t height)
{
    m_Size.width  = width;
    m_Size.height = height;

    updateTextureSize();
}

void Component::setMinSize(int32_t minWidth, int32_t minHeight)
{
    m_MinSize.width  = minWidth;
    m_MinSize.height = minHeight;
    setMinSizeEnabled(true);
}

void Component::setMaxSize(int32_t maxWidth, int32_t maxHeight)
{
    m_MaxSize.width  = maxWidth;
    m_MaxSize.height = maxHeight;
    setMaxSizeEnabled(true);
}

void Component::setPrefSize(int32_t prefWidth, int32_t prefHeight)
{
    m_PrefSize.width  = prefWidth;
    m_PrefSize.height = prefHeight;
    setPrefSizeEnabled(true);
}

void Component::setMinSizeEnabled(bool enabled)
{
    m_MinSizeSet = enabled;
}

void Component::setMaxSizeEnabled(bool enabled)
{
    m_MaxSizeSet = enabled;
}

void Component::setPrefSizeEnabled(bool enabled)
{
    m_PrefSizeSet = enabled;
}

void Component::setForeground(Color foreground)
{
    m_Foreground = foreground;
}

void Component::setBackground(Color background)
{
    m_Background = background;
}
//------------------------------------------------------------------------------

//------------------------------------Getters-----------------------------------
Renderer* Component::getRenderer() const
{
    return m_Renderer;
}

Texture* Component::getTexture() const
{
    return m_Texture;
}

id128_t Component::getId() const
{
    return m_Id;
}

Component* Component::getParent() const
{
    return m_Parent;
}

GuiEventDispatcher* Component::getEventDispatcher()
{
    return &m_Dispatcher;
}

bool Component::isVisible() const
{
    return m_IsVisible;
}

bool Component::isFocused() const
{
    return m_IsFocused;
}

int32_t Component::getX() const
{
    return m_X;
}

int32_t Component::getY() const
{
    return m_Y;
}

int32_t Component::getWidth() const
{
    return m_Size.width;
}

int32_t Component::getHeight() const
{
    return m_Size.height;
}

const Dimension& Component::getSize() const
{
    return m_Size;
}

const Dimension& Component::getMinSize() const
{
    return m_MinSize;
}

const Dimension& Component::getMaxSize() const
{
    return m_MaxSize;
}

const Dimension& Component::getPrefSize() const
{
    return m_PrefSize;
}

bool Component::isMinSizeSet() const
{
    return m_MinSizeSet;
}

bool Component::isMaxSizeSet() const
{
    return m_MaxSizeSet;
}

bool Component::isPrefSizeSet() const
{
    return m_PrefSizeSet;
}

Color Component::getForeground() const
{
    return m_Foreground;
}

Color Component::getBackground() const
{
    return m_Background;
}
//------------------------------------------------------------------------------

void Component::updateTextureSize()
{
    assert(m_Renderer);

    if (m_Size.width <= 0 || m_Size.height <= 0)
    {
        return;
    }

    if (m_Texture == nullptr)
    {
        m_Texture = new Texture(*m_Renderer, m_Size.width, m_Size.height);
    }
    else if (static_cast<int32_t>(m_Texture->getWidth()) < m_Size.width ||
             static_cast<int32_t>(m_Texture->getHeight()) < m_Size.height)
    {
        delete m_Texture;
        m_Texture = new Texture(*m_Renderer, m_Size.width, m_Size.height);
    }
}