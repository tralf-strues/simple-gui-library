//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.cpp
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "components/button.h"

Button::Button(Renderer* renderer, const Font& font, const char* label)
    : Component(renderer), m_Label(renderer, font, label), m_DefaultSkin(this)
{
    m_Skin = &m_DefaultSkin;
}

Label* Button::getLabel()
{
    return &m_Label;
}

void Button::setLabel(const char* label)
{
    m_Label.setText(label);
}

void Button::updateGraphics()
{
    m_Label.setForeground(getForeground());
    m_Label.updateGraphics();
    applySkin();
    // setMinSize(m_Label.getMinSize() + BUTTON_DEFAULT_MIN_MARGINS);
    // setPrefSize(getMinSize());

    // if (m_Label.getText() != nullptr)
    // {
    //     setSize(m_Label.getSize() + BUTTON_DEFAULT_MIN_MARGINS);
    // }
}

// void Button::render(Texture* target, const Rectangle<int32_t>& targetRegion)
// {
//     Component::render(target, targetRegion);

//     Rectangle<int32_t> centeredRegion = centerRegion(getRegion(), m_Label.getRegion());
//     m_Label.setX(centeredRegion.pos.x);
//     m_Label.setY(centeredRegion.pos.y);

//     m_Label.render(target, targetRegion);
// }

int32_t Button::getPrefWidth() const
{
    if (isPrefSizeEnabled())
    {
        return Component::getPrefWidth();
    }
    
    if (m_Skin != nullptr)
    {
        return m_Skin->computePrefWidth();
    }

    return DEFAULT_BUTTON_PREF_SIZE.x;
}

int32_t Button::getPrefHeight() const
{
    if (isPrefSizeEnabled())
    {
        return Component::getPrefHeight();
    }
    
    if (m_Skin != nullptr)
    {
        return m_Skin->computePrefHeight();
    }

    return DEFAULT_BUTTON_PREF_SIZE.y;
}