#include <assert.h>
#include "components/label.h"

Label::Label(Renderer* renderer, const Font& font, Color foreground)
    : Component(renderer)
{
    assert(renderer);

    m_Text.setFont(font);
    m_Text.setColor(foreground);    
}

Label::Label(Renderer* renderer, const Font& font, const char* str, Color foreground)
    : Label(renderer, font, foreground)
{
    setText(str);
}

void Label::setText(const char* str)
{
    m_Text.setString(str);
    updateGraphics();
}

const char* Label::getText() const
{
    return m_Text.getStr();
}

bool Label::isEmpty() const
{
    return m_Text.getStr() == nullptr;
}

void Label::updateGraphics()
{
    if (m_Text.getStr() == nullptr)
    {
        return;
    }

    m_Text.setColor(getForeground());
    m_Text.load(*m_Renderer);

    // FIXME: create a separate function ensureSize()
    if (m_Text.getWidth() > getWidth())
    {
        setWidth(m_Text.getWidth());
    }

    if (m_Text.getHeight() > getHeight())
    {
        setHeight(m_Text.getHeight());
    }
    // setSize(m_Text.getWidth(), m_Text.getHeight());
    // setMinSize(m_Text.getWidth(),  m_Text.getHeight());
    // setPrefSize(m_Text.getWidth(), m_Text.getHeight());
}

void Label::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    Component::render(target, targetRegion);

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(target);

    m_Text.render(*m_Renderer, targetRegion.pos + getPos());

    m_Renderer->setTarget(savedTarget);    
}

int32_t Label::getPrefWidth() const
{
    if (!isEmpty())
    {
        return m_Text.getWidth();
    }

    return 0;
}

int32_t Label::getPrefHeight() const
{
    if (!isEmpty())
    {
        return m_Text.getHeight();
    }

    return 0;
}