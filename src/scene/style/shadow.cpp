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
    Shadow::Shadow(const Sml::Vec2<int32_t>& offset, const Sml::Vec2<float>& scale,
                   int32_t blurRadius, Sml::Color color)
        : m_Offset(offset), m_Scale(scale), m_Color(color)
    {
        m_BlurKernel = Sml::createGaussianBlurKernel(blurRadius);
        assert(m_BlurKernel);
    }

    const Sml::Vec2<int32_t>& Shadow::getOffset() const { return m_Offset; }
    void Shadow::setOffset(const Sml::Vec2<int32_t>& offset) { m_Offset = offset; }

    const Sml::Vec2<float>& Shadow::getScale() const { return m_Scale; }
    void Shadow::setScale(const Sml::Vec2<float>& offset) { m_Scale = offset; }

    int32_t Shadow::getBlurRadius() const { return m_BlurKernel->getRadius(); }

    Sml::Color Shadow::getColor() const { return m_Color; }
    void Shadow::setColor(Sml::Color color) { m_Color = color; }

    void Shadow::renderRectangular(const Shadow* shadow, const Sml::Rectangle<int32_t>& object,
                                   const Sml::Rectangle<int32_t>& targetRegion)
    {
        assert(shadow);

        Sml::Rectangle<int32_t> rectangle{targetRegion.pos + object.pos + shadow->m_Offset,
                                          object.width, object.height};

        rectangle.width  *= shadow->getScale().x;
        rectangle.height *= shadow->getScale().y;
        rectangle.pos.x  -= (rectangle.width  - object.width)  / 2;
        rectangle.pos.y  -= (rectangle.height - object.height) / 2;

        Sml::Renderer::getInstance().setColor(shadow->m_Color);
        Sml::renderFilledRect(rectangle);

        rectangle.pos.x  -= shadow->getBlurRadius();
        rectangle.pos.y  -= shadow->getBlurRadius();
        rectangle.width  += 2 * shadow->getBlurRadius();
        rectangle.height += 2 * shadow->getBlurRadius();

        if (rectangle.pos.x < 0)
        {
            rectangle.width += rectangle.pos.x;
            rectangle.pos.x  = 0;
        }

        if (rectangle.pos.x >= targetRegion.width)
        {
            rectangle.width -= rectangle.pos.x - targetRegion.width + 1;
            rectangle.pos.x  = targetRegion.width - 1;
        }

        if (rectangle.pos.y < 0)
        {
            rectangle.height += rectangle.pos.y;
            rectangle.pos.y   = 0;
        }

        if (rectangle.pos.y >= targetRegion.height)
        {
            rectangle.height -= rectangle.pos.y - targetRegion.height + 1;
            rectangle.pos.y   = targetRegion.height - 1;
        }

        Sml::applyKernel(shadow->m_BlurKernel, rectangle);
    }
}