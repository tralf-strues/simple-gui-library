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
    Shape::Shape(Fill* fill) : m_Fill(fill) {}

    void Shape::layout() {}
    bool Shape::isResizable() const { return false; }

    const Fill* Shape::getFill() const { return m_Fill; }
    void Shape::setFill(const Fill* fill) { m_Fill = fill; }

    const Border* Shape::getBorder() const { return m_Border; }
    void Shape::setBorder(const Border* border) { m_Border = border; }

    
    int32_t Shape::computeMinWidth(int32_t height) const { return getLayoutWidth();  }
    int32_t Shape::computeMinHeight(int32_t width) const { return getLayoutHeight(); }

    int32_t Shape::computeMaxWidth(int32_t height) const { return getLayoutWidth();  }
    int32_t Shape::computeMaxHeight(int32_t width) const { return getLayoutHeight(); }
}