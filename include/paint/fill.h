/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file fill.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/math/vec2.h"
#include "sml/math/rectangle.h"
#include "sml/graphics_wrapper/texture.h"

namespace Sgl
{
    class Fill
    {
    public:
        virtual void fillLine(const Sml::Vec2i& start,
                              const Sml::Vec2i& end,
                              const Sml::Rectangle<int32_t>& targetRegion) const = 0;

        virtual void fillArea(const Sml::Rectangle<int32_t>& area,
                              const Sml::Rectangle<int32_t>& targetRegion) const = 0;

        virtual void fillPoint(const Sml::Vec2i& point,
                               const Sml::Rectangle<int32_t>& targetRegion) const = 0;
    };

    class ColorFill : public Fill
    {
    public:
        ColorFill(Sml::Color color = Sml::COLOR_TRANSPARENT);

        virtual void fillLine(const Sml::Vec2i& start,
                              const Sml::Vec2i& end,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillArea(const Sml::Rectangle<int32_t>& area,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillPoint(const Sml::Vec2i& point,
                               const Sml::Rectangle<int32_t>& targetRegion) const override;

        Sml::Color getColor() const;
        void setColor(Sml::Color color);

    protected:
        Sml::Color m_Color;
    };

    // TODO:
    // class LinearGradientFill : public Fill
    // {
    // };
}
