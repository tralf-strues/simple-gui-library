/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file border.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/color.h"
#include "insets.h"

namespace Sgl
{
    class Border
    {
    public:
        Border() = default;
        Border(int32_t thickness, Sml::Color color);

        int32_t getThickness() const;
        Sml::Color getColor() const;
        Insets getInsets() const;

    private:
        int32_t    m_Thickness = 0;
        Sml::Color m_Color     = Sml::COLOR_TRANSPARENT;
    };
}