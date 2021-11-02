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
    if (x >= m_X && x < m_X + m_Size.x && y >= m_Y && y < m_Y + m_Size.y)
    {
        return this;
    }

    return nullptr;
}

void Component::updateGraphics()
{
    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(m_Texture);
    
    m_Renderer->setColor(m_Background);
    m_Renderer->clear();

    applySkin();

    m_Renderer->setTarget(savedTarget);
}

void Component::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    if (!m_IsVisible)
    {
        return;
    }

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(target);

    Rectangle<int32_t> region = getRegion();
    region.pos += targetRegion.pos;
    renderTexture(m_Renderer, *m_Texture, &region, nullptr);

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

void Component::attachHandler(const std::initializer_list<EventType>& types, Listener* handler)
{
    m_Dispatcher.attachHandler(types, handler);
}

void Component::attachFilter(const std::initializer_list<EventType>& types, Listener* filter)
{
    m_Dispatcher.attachFilter(types, filter);
}

void Component::applySkin()
{
    if (m_Skin != nullptr)
    {
        m_Skin->apply(m_Renderer, m_Texture, nullptr);
    }
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
    m_Size.x = width;
    m_Size.y = height;

    updateTextureSize();
}

void Component::setSize(const Vec2<int32_t>& size)
{
    setSize(size.x, size.y);
}

void Component::setWidth(int32_t width)
{
    setSize(width, getHeight());
}

void Component::setHeight(int32_t height)
{
    setSize(getWidth(), height);
}

// void Component::setMinSize(int32_t minWidth, int32_t minHeight)
// {
//     m_MinSize.x = minWidth;
//     m_MinSize.y = minHeight;
//     setMinSizeEnabled(true);

//     Vec2<int32_t> newSize = getSize();

//     if (getWidth() < m_MinSize.x)
//     {
//         newSize.x = m_MinSize.x;
//     }
    
//     if (getHeight() < m_MinSize.y)
//     {
//         newSize.y = m_MinSize.y;
//     }

//     setSize(newSize);
// }

// void Component::setMinSize(const Vec2<int32_t>& minSize)
// {
//     setMinSize(minSize.x, minSize.y);
// }

// void Component::setMaxSize(int32_t maxWidth, int32_t maxHeight)
// {
//     m_MaxSize.x = maxWidth;
//     m_MaxSize.y = maxHeight;
//     setMaxSizeEnabled(true);
// }

// void Component::setMaxSize(const Vec2<int32_t>& maxSize)
// {
//     setMaxSize(maxSize.x, maxSize.y);
// }

// void Component::setPrefSize(int32_t prefWidth, int32_t prefHeight)
// {
//     m_PrefSize.x = prefWidth;
//     m_PrefSize.y = prefHeight;
//     setPrefSizeEnabled(true);
// }

// void Component::setPrefSize(const Vec2<int32_t>& prefSize)
// {
//     setPrefSize(prefSize.x, prefSize.y);
// }

// void Component::setMinSizeEnabled(bool enabled)
// {
//     m_MinSizeSet = enabled;
// }

// void Component::setMaxSizeEnabled(bool enabled)
// {
//     m_MaxSizeSet = enabled;
// }

// void Component::setPrefSizeEnabled(bool enabled)
// {
//     m_PrefSizeSet = enabled;
// }

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

Vec2<int32_t> Component::getPos() const
{
    return Vec2<int32_t>{m_X, m_Y};
}

Rectangle<int32_t> Component::getRegion() const
{
    return Rectangle<int32_t>{getPos(), m_Size.x, m_Size.y};
}

int32_t Component::getWidth() const
{
    return m_Size.x;
}

int32_t Component::getHeight() const
{
    return m_Size.y;
}

const Vec2<int32_t>& Component::getSize() const
{
    return m_Size;
}

// const Vec2<int32_t>& Component::getMinSize() const
// {
//     return m_MinSize;
// }

// const Vec2<int32_t>& Component::getMaxSize() const
// {
//     return m_MaxSize;
// }

// const Vec2<int32_t>& Component::getPrefSize() const
// {
//     return m_PrefSize;
// }

// bool Component::isMinSizeSet() const
// {
//     return m_MinSizeSet;
// }

// bool Component::isMaxSizeSet() const
// {
//     return m_MaxSizeSet;
// }

// bool Component::isPrefSizeSet() const
// {
//     return m_PrefSizeSet;
// }

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

    if (m_Size.x <= 0 || m_Size.y <= 0)
    {
        return;
    }

    if (m_Texture == nullptr)
    {
        m_Texture = new Texture(m_Renderer, m_Size.x, m_Size.y);
    }
    else if (static_cast<int32_t>(m_Texture->getWidth()) < m_Size.x ||
             static_cast<int32_t>(m_Texture->getHeight()) < m_Size.y)
    {
        delete m_Texture;
        m_Texture = new Texture(m_Renderer, m_Size.x, m_Size.y);
    }
}