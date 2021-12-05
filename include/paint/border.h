/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file border.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/color.h"
#include "sml/math/rectangle.h"
#include "insets.h"

namespace Sgl
{
    class Border
    {
    public:
        static void encloseArea(const Border* border, const Sml::Rectangle<int32_t>& targetRegion);

    public:
        Border() = default;
        Border(int32_t thickness, Sml::Color color);
        Border(const Insets& thickness, Sml::Color color);

        const Insets& getThickness() const;
        Sml::Color getColor() const;

    private:
        Insets     m_Thickness = 0;
        Sml::Color m_Color     = Sml::COLOR_TRANSPARENT;
    };
}