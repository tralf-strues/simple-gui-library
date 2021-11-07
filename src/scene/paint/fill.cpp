/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file fill.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/graphics_wrapper/primitives.h"
#include "core.h"
#include "scene/paint/fill.h"

namespace Sgl
{
    ColorFill::ColorFill(Sml::Color color)
        : m_Color(color) {}

    void ColorFill::fillLine(const Sml::Vec2<int32_t>& start, const Sml::Vec2<int32_t>& end,
                             const Sml::Rectangle<int32_t>& targetRegion) const
    {
        getContextRenderer()->setColor(m_Color);

        Sml::Vec2<int32_t> translatedStart = start + targetRegion.pos;
        Sml::Vec2<int32_t> translatedEnd   = end + targetRegion.pos;

        Sml::renderLine(getContextRenderer(), translatedStart, translatedEnd);
    }

    void ColorFill::fillArea(const Sml::Rectangle<int32_t>& area,
                             const Sml::Rectangle<int32_t>& targetRegion) const
    {
        getContextRenderer()->setColor(m_Color);

        Sml::Rectangle<int32_t> translatedArea = area;
        translatedArea.pos += targetRegion.pos;

        Sml::renderFilledRect(getContextRenderer(), translatedArea);
    }

    void ColorFill::fillPoint(const Sml::Vec2<int32_t>& point,
                              const Sml::Rectangle<int32_t>& targetRegion) const
    {
        getContextRenderer()->setColor(m_Color);

        Sml::Vec2<int32_t> translatedPoint = point + targetRegion.pos;

        Sml::renderPoint(getContextRenderer(), translatedPoint);
    }
}
