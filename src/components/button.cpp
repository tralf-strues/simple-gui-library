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
    setDefaultStyle();
}

Button::Button(Renderer* renderer, const Font& font, const char* label, Color foreground, Color background)
    : Component(renderer), m_Label(renderer, font, label, foreground)
{
    m_Background = background;
    setDefaultStyle();
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
    m_Label.setForeground(getForeground());
    m_Label.updateGraphics();
    // setMinSize(m_Label.getMinSize() + BUTTON_DEFAULT_MIN_MARGINS);
    // setPrefSize(getMinSize());

    if (m_Label.getText() != nullptr)
    {
        setSize(m_Label.getSize() + BUTTON_DEFAULT_MIN_MARGINS);
    }

    Component::updateGraphics();
}

void Button::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    Component::render(target, targetRegion);

    Rectangle<int32_t> centeredRegion = centerRegion(getRegion(), m_Label.getRegion());
    m_Label.setX(centeredRegion.pos.x);
    m_Label.setY(centeredRegion.pos.y);

    m_Label.render(target, targetRegion);
}

void Button::setDefaultStyle()
{
    setSkin(&DEFAULT_SKIN_BUTTON);
    m_Dispatcher.attachHandler({MouseEnteredEvent::getStaticType(), MouseExitedEvent::getStaticType()},
                               new HoverListener{this, &DEFAULT_HOVER_STYLE_BUTTON});
}