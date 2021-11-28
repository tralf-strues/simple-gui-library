/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shadow.h
 * @date 2021-11-22
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/math/blur.h"
#include "sml/graphics_wrapper/texture.h"
#include "../component.h"

namespace Sgl
{
    class Shadow
    {
    public:
        static void renderRectangular(const Shadow* shadow, const Sml::Rectangle<int32_t>& object,
                                      const Sml::Rectangle<int32_t>& targetRegion);

    public:
        Shadow(const Sml::Vec2<int32_t>& offset, const Sml::Vec2<float>& scale,
               int32_t blurRadius, Sml::Color color = Sml::COLOR_BLACK);

        const Sml::Vec2<int32_t>& getOffset() const;
        void setOffset(const Sml::Vec2<int32_t>& offset);

        const Sml::Vec2<float>& getScale() const;
        void setScale(const Sml::Vec2<float>& scale);

        int32_t getBlurRadius() const;

        Sml::Color getColor() const;
        void setColor(Sml::Color color);

    protected:
        Sml::Vec2<int32_t> m_Offset     = {0, 0};
        Sml::Vec2<float>   m_Scale      = {1, 1};
        Sml::Kernel*       m_BlurKernel = nullptr; 
        Sml::Color         m_Color      = Sml::COLOR_BLACK;
    };
}