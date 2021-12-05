/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file fill.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/graphics_wrapper/primitives.h"
#include "scene/paint/fill.h"

namespace Sgl
{
    ColorFill::ColorFill(Sml::Color color)
        : m_Color(color) {}

    void ColorFill::fillLine(const Sml::Vec2i& start, const Sml::Vec2i& end,
                             const Sml::Rectangle<int32_t>& targetRegion) const
    {
        Sml::Renderer::getInstance().setColor(m_Color);

        Sml::Vec2i translatedStart = start + targetRegion.pos;
        Sml::Vec2i translatedEnd   = end + targetRegion.pos;

        Sml::renderLine(translatedStart, translatedEnd);
    }

    void ColorFill::fillArea(const Sml::Rectangle<int32_t>& area,
                             const Sml::Rectangle<int32_t>& targetRegion) const
    {
        Sml::Renderer::getInstance().setColor(m_Color);

        Sml::Rectangle<int32_t> translatedArea = area;
        translatedArea.pos += targetRegion.pos;

        Sml::renderFilledRect(translatedArea);
    }

    void ColorFill::fillPoint(const Sml::Vec2i& point,
                              const Sml::Rectangle<int32_t>& targetRegion) const
    {
        Sml::Renderer::getInstance().setColor(m_Color);

        Sml::Vec2i translatedPoint = point + targetRegion.pos;
        Sml::renderPoint(translatedPoint);
    }
}
