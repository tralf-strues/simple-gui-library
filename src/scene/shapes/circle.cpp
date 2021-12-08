/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file circle.cpp
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/shapes/circle.h"

using namespace Sgl;

Circle::Circle(int32_t radius, const Border* border) : m_Radius(radius)
{
    setBorder(border);
}

int32_t Circle::getRadius() const { return m_Radius; }
void Circle::setRadius(int32_t radius) { m_Radius = radius; }

void Circle::render(const Sml::Rectangle<int32_t>& targetRegion)
{
    if (getBorder() != nullptr)
    {
        Sml::Renderer::getInstance().setColor(getBorder()->getColor());
        Sml::renderCircle(Sml::Circle(targetRegion.pos + getLayoutPos(), m_Radius));
    }
}

int32_t Circle::computePrefWidth(int32_t height) const { return 2 * m_Radius + 1; }
int32_t Circle::computePrefHeight(int32_t width) const { return 2 * m_Radius + 1; }