/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file rectangle.cpp
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/shapes/rectangle.h"

using namespace Sgl;

Rectangle::Rectangle(Sml::Color fillColor) : Shape(fillColor) {}
Rectangle::Rectangle(const Background* background) : Shape(background) {}

int32_t Rectangle::getWidth() const { return m_Width; }
void Rectangle::setWidth(int32_t width) { m_Width = width; }

int32_t Rectangle::getHeight() const { return m_Height; }
void Rectangle::setHeight(int32_t height) { m_Height = height; }

void Rectangle::render(const Sml::Rectangle<int32_t>& targetRegion)
{
    renderShadow(targetRegion);

    Sml::Rectangle<int32_t> region = getLayoutBounds();
    region.pos += targetRegion.pos;

    Sml::Renderer::getInstance().setColor(m_FillColor);
    Sml::renderFilledRect(region);

    if (getBackground() != nullptr)
    {
        Sgl::Background::fillArea(getBackground(), region);
    }

    if (getBorder() != nullptr)
    {
        Border::encloseArea(getBorder(), targetRegion);
    }
}

int32_t Rectangle::computePrefWidth(int32_t height) const
{
    return m_Width;
}

int32_t Rectangle::computePrefHeight(int32_t width) const
{
    return m_Height;
}