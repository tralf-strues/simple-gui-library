//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.cpp
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "components/button.h"

Button::Button(Renderer* renderer, const Font& font, Color background)
    : Component(renderer), m_Label(renderer, font)
{
    m_Background = background;
}

Button::Button(Renderer* renderer, const Font& font, const char* label, Color foreground, Color background)
    : Button(renderer, font, background)
{
    m_Label.setForeground(foreground);
}

const char* Button::getLabel() const
{
    return m_Label.getText();
}

void Button::setLabel(const char* label)
{
    m_Label.setText(label);
}

void Button::updateGraphics()
{
    Component::updateGraphics();

    applySkin();

    m_Label.updateGraphics();
    m_Label.setSize(m_Label.getPrefSize().x, m_Label.getPrefSize().y);
    m_Label.updateGraphics(); // FIXME: Rerender text to the newly allocated Texture
    Rectangle<int32_t> centeredRegion = centerRegion(getRegion(), m_Label.getRegion());
    m_Label.setX(centeredRegion.pos.x);
    m_Label.setY(centeredRegion.pos.y);
}

void Button::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    Component::render(target, targetRegion);

    // Rectangle<int32_t> translatedRegion = targetRegion;
    // translatedRegion.pos += getPos();
    // translatedRegion.pos += targetRegion.pos;
    // translatedRegion.pos -= m_Label.getPos();

    m_Label.render(target, targetRegion);
}

void Button::applySkin()
{
    if (m_Skin != nullptr)
    {
        m_Skin->apply(m_Renderer, m_Texture, nullptr);
    }
}