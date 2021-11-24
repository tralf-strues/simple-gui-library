/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shadow.cpp
 * @date 2021-11-22
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/style/shadow.h"

namespace Sgl
{
    //------------------------------------------------------------------------------
    // Shadow
    //------------------------------------------------------------------------------
    Shadow::Shadow(const Sml::Vec2<int32_t>& offset, int32_t blurRadius, Sml::Color color)
        : m_Offset(offset), m_Color(color)
    {
        m_BlurKernel = Sml::createGaussianBlurKernel(blurRadius);
        assert(m_BlurKernel);
    }

    const Sml::Vec2<int32_t>& Shadow::getOffset() const { return m_Offset; }
    void Shadow::setOffset(const Sml::Vec2<int32_t>& offset) { m_Offset = offset; }

    int32_t Shadow::getBlurRadius() const { return m_BlurKernel->getRadius(); }

    Sml::Color Shadow::getColor() const { return m_Color; }
    void Shadow::setColor(Sml::Color color) { m_Color = color; }

    void Shadow::renderRectangular(const Shadow* shadow, const Sml::Rectangle<int32_t>& object,
                                   const Sml::Rectangle<int32_t>& targetRegion)
    {
        assert(shadow);

        Sml::Rectangle<int32_t> rectangle{targetRegion.pos + object.pos + shadow->m_Offset,
                                          object.width, object.height};

        getContextRenderer()->setColor(shadow->m_Color);
        Sml::renderFilledRect(getContextRenderer(), rectangle);

        rectangle.pos.x  -= shadow->getBlurRadius();
        rectangle.pos.y  -= shadow->getBlurRadius();
        rectangle.width  += 2 * shadow->getBlurRadius();
        rectangle.height += 2 * shadow->getBlurRadius();

        Sml::applyKernel(shadow->m_BlurKernel, getContextRenderer(), rectangle);
    }
}