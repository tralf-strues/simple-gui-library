/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file border.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/border.h"

namespace Sgl
{
    Border::Border(int32_t thickness, Sml::Color color)
        : m_Thickness(thickness), m_Color(color) {}

    int32_t Border::getThickness() const
    {
        return m_Thickness;
    }

    Sml::Color Border::getColor() const
    {
        return m_Color;
    }

    Insets Border::getInsets() const
    {
        return Insets{m_Thickness};
    }
}