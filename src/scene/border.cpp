/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file border.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/graphics_wrapper/primitives.h"
#include "scene/border.h"

namespace Sgl
{
    Border::Border(int32_t thickness, Sml::Color color)
        : m_Thickness(thickness), m_Color(color) {}

    Border::Border(const Insets& thickness, Sml::Color color)
        : m_Thickness(thickness), m_Color(color) {}


    const Insets& Border::getThickness() const
    {
        return m_Thickness;
    }

    Sml::Color Border::getColor() const
    {
        return m_Color;
    }

    void Border::encloseArea(const Border* border, const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (border == nullptr) { return; }

        Sml::Rectangle<int32_t> topBorder{targetRegion.pos, targetRegion.width, border->getThickness().top};
        Sml::Rectangle<int32_t> leftBorder{targetRegion.pos, border->getThickness().left, targetRegion.height};

        Sml::Rectangle<int32_t> bottomBorder;
        bottomBorder.pos.x  = targetRegion.pos.x;
        bottomBorder.pos.y  = targetRegion.pos.y + targetRegion.height - border->getThickness().bottom;
        bottomBorder.width  = targetRegion.width;
        bottomBorder.height = border->getThickness().bottom;

        Sml::Rectangle<int32_t> rightBorder;
        rightBorder.pos.x  = targetRegion.pos.x + targetRegion.width - border->getThickness().right;
        rightBorder.pos.y  = targetRegion.pos.y;
        rightBorder.width  = border->getThickness().right;
        rightBorder.height = targetRegion.height;

        Sml::Renderer::getInstance().setColor(border->getColor());
     
        if (border->getThickness().top > 0)
        {
            Sml::renderFilledRect(topBorder);
        }

        if (border->getThickness().right > 0)
        {
            Sml::renderFilledRect(rightBorder);
        }

        if (border->getThickness().bottom > 0)
        {
            Sml::renderFilledRect(bottomBorder);
        }

        if (border->getThickness().left > 0)
        {
            Sml::renderFilledRect(leftBorder);
        }
    }
}