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
    assert(str);

    m_Text.setString(str);
    updateGraphics();
}

const char* Label::getText() const
{
    return m_Text.getStr();
}

void Label::updateGraphics()
{
    Component::updateGraphics();

    if (m_Text.getStr() == nullptr)
    {
        return;
    }

    m_Text.load(*m_Renderer);
    setPrefSize(m_Text.getWidth(), m_Text.getHeight());
}

void Label::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    Component::render(target, targetRegion);

    Texture* savedTarget = m_Renderer->getTarget();
    m_Renderer->setTarget(target);

    m_Text.render(*m_Renderer, targetRegion.pos + getPos());

    m_Renderer->setTarget(savedTarget);    
}