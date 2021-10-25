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
    Texture* savedTarget = m_Renderer->getTarget();

    m_Renderer->setTarget(m_Texture);
    
    m_Renderer->setColor(m_Background);
    m_Renderer->clear();

    if (m_Text.getStr() != nullptr)
    {
        m_Text.load(*m_Renderer);
        setPrefSize(m_Text.getWidth(), m_Text.getHeight());
    }

    m_Text.render(*m_Renderer, Vec2<int32_t>{0, 0});
 
    m_Renderer->setTarget(savedTarget);
}