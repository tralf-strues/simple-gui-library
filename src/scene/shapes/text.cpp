/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file text.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/math/rectangle.h"
#include "scene/shapes/text.h"

namespace Sgl
{
    Text::Text(const Sml::Font& font, const char* string, Sml::Color color)
        : m_Text(font, string, color) {}

    Component* Text::getHitComponent(int32_t x, int32_t y)
    {
        if (Sml::isPointInsideRectangle(Sml::Vec2<int32_t>{x, y}, getLayoutBounds()))
        {
            return this;
        }

        return nullptr;
    }

    void Text::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        m_Text.render(targetRegion.pos + m_LayoutBounds.pos);
    }

    void Text::prerender()
    {
        if (getStr() != nullptr)
        {
            m_Text.load();
        }
    }

    const Sml::Font& Text::getFont()      const { return m_Text.getFont();      }
    Sml::Color       Text::getColor()     const { return m_Text.getColor();     }
    const char*      Text::getStr()       const { return m_Text.getStr();       }
    size_t           Text::getWidth()     const { return m_Text.getWidth();     }
    size_t           Text::getHeight()    const { return m_Text.getHeight();    }
    size_t           Text::getWrapWidth() const { return m_Text.getWrapWidth(); }

    void Text::setFont(const Sml::Font& font) { m_Text.setFont(font);           }
    void Text::setColor(Sml::Color color)     { m_Text.setColor(color);         }
    void Text::setString(const char* str)     { m_Text.setString(str);          }
    void Text::setWrapWidth(size_t wrapWidth) { m_Text.setWrapWidth(wrapWidth); }

    int32_t Text::computePrefWidth(int32_t height) const
    {
        if (getStr() == nullptr) { return 0; }

        return m_Text.estimateTextDimensions(getStr()).x;
    }

    int32_t Text::computePrefHeight(int32_t width) const
    {
        if (getStr() == nullptr) { return 0; }

        return m_Text.estimateTextDimensions(getStr()).y;
    }
}