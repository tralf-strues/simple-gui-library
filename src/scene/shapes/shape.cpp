/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shape.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/shapes/shape.h"

namespace Sgl
{
    Shape::Shape(Sml::Color fillColor) : m_FillColor(fillColor) {}
    Shape::Shape(const Background* background) : m_Background(background) {}

    void Shape::layout() { updateShadow(); }
    bool Shape::isResizable() const { return false; }

    // const std::vector<const Fill*>& Shape::getFills() const { return m_Fills; }
    // void Shape::addFill(const Fill* fill) { assert(fill); m_Fills.push_back(fill); }

    // void Shape::setFill(const Fill* fill)
    // {
    //     assert(fill);
    //     m_Fills.clear();
    //     m_Fills.push_back(fill);
    // }

    Sml::Color Shape::getFillColor() const { return m_FillColor; }
    void Shape::setFillColor(Sml::Color color) { m_FillColor = color; }

    const Background* Shape::getBackground() const { return m_Background; }
    void Shape::setBackground(const Background* background) { m_Background = background; }

    const Border* Shape::getBorder() const { return m_Border; }
    void Shape::setBorder(const Border* border) { m_Border = border; }
    
    int32_t Shape::computeMinWidth(int32_t height) const { return getLayoutWidth();  }
    int32_t Shape::computeMinHeight(int32_t width) const { return getLayoutHeight(); }

    int32_t Shape::computeMaxWidth(int32_t height) const { return getLayoutWidth();  }
    int32_t Shape::computeMaxHeight(int32_t width) const { return getLayoutHeight(); }
}