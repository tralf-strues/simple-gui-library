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