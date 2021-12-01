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

namespace Sgl
{
    class ShadowSpecification
    {
    public:
        friend class Shadow;

    public:
        ShadowSpecification(const Sml::Vec2<int32_t>& offset, const Sml::Vec2<float>& scale,
                            int32_t blurRadius, Sml::Color color = Sml::COLOR_BLACK);

        ~ShadowSpecification();

        const Sml::Vec2<int32_t>& getOffset() const;
        const Sml::Vec2<float>& getScale() const;
        int32_t getBlurRadius() const;
        Sml::Color getColor() const;

    private:
        Sml::Vec2<int32_t> m_Offset     = {0, 0};
        Sml::Vec2<float>   m_Scale      = {1, 1};
        Sml::Kernel*       m_BlurKernel = nullptr; 
        Sml::Color         m_Color      = Sml::COLOR_BLACK;
    };

    class Shadow
    {
    public:
        Shadow(const ShadowSpecification* specification = nullptr);

    public:
        ~Shadow();

        void update(const Sml::Texture* srcTexture, const Sml::Rectangle<int32_t>& objectRegion);
        void render(const Sml::Rectangle<int32_t>& object, const Sml::Rectangle<int32_t>& targetRegion);

        Sml::Texture* getTexture() const;

        const ShadowSpecification* getSpecification() const;
        void setSpecification(const ShadowSpecification* specification);

    private:
        const ShadowSpecification* m_Specification = nullptr;
        Sml::Texture*              m_Texture       = nullptr;
    };
}