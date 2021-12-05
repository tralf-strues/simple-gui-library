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
    ShadowSpecification::ShadowSpecification(const Sml::Vec2i& offset, const Sml::Vec2f& scale,
                                             int32_t blurRadius, Sml::Color color)
        : m_Offset(offset), m_Scale(scale), m_Color(color)
    {
        m_BlurKernel = Sml::createGaussianBlurKernel(blurRadius);
        assert(m_BlurKernel);
    }

    ShadowSpecification::~ShadowSpecification()
    {
        if (m_BlurKernel != nullptr)
        {
            delete m_BlurKernel;
        }        
    }

    const Sml::Vec2i& ShadowSpecification::getOffset() const { return m_Offset; }
    const Sml::Vec2f& ShadowSpecification::getScale() const { return m_Scale; }
    int32_t ShadowSpecification::getBlurRadius() const { return m_BlurKernel->getRadius(); }
    Sml::Color ShadowSpecification::getColor() const { return m_Color; }

    Shadow::~Shadow()
    {
        if (m_Texture != nullptr)
        {
            delete m_Texture;
        }
    }

    Sml::Texture* Shadow::getTexture() const { return m_Texture; }

    const ShadowSpecification* Shadow::getSpecification() const { return m_Specification; }
    void Shadow::setSpecification(const ShadowSpecification* specification) { m_Specification = specification; }

    Shadow::Shadow(const ShadowSpecification* specification) : m_Specification(specification) {}

    void Shadow::update(const Sml::Texture* srcTexture, const Sml::Rectangle<int32_t>& objectRegion)
    {
        assert(srcTexture);

        if (m_Specification == nullptr) { return; }

        if (m_Texture != nullptr)
        {
            delete m_Texture;
        }

        int32_t blurRadius   = m_Specification->getBlurRadius();

        int32_t shadowWidth  = objectRegion.width  + 2 * blurRadius;
        int32_t shadowHeight = objectRegion.height + 2 * blurRadius;

        Sml::Color* srcPixels = srcTexture->readPixels(&objectRegion);
        Sml::Color* dstPixels = new Sml::Color[shadowWidth * shadowHeight];
        memset(dstPixels, 0, shadowWidth * shadowHeight * sizeof(Sml::Color));

        float shadowColorAlpha = static_cast<float>(Sml::colorGetA(m_Specification->getColor())) / (255.f * 255.f);

        for (int32_t y = 0; y < objectRegion.height; ++y)
        {
            for (int32_t x = 0; x < objectRegion.width; ++x)
            {
                Sml::Color srcColor = srcPixels[x + y * objectRegion.width];

                float curColorAlphaFloat = shadowColorAlpha * static_cast<float>(Sml::colorGetA(srcColor));
                uint8_t curColorAlpha = 255 * curColorAlphaFloat;

                Sml::Color curColor = Sml::colorSetA(m_Specification->getColor(), curColorAlpha);
                dstPixels[(x + blurRadius) + (y + blurRadius) * shadowWidth] = curColor;
            }
        }

        Sml::Color* blurredPixels = new Sml::Color[shadowWidth * shadowHeight];

        Sml::applyKernel(m_Specification->m_BlurKernel,
                         dstPixels,
                         blurredPixels,
                         shadowWidth,
                         shadowHeight);

        m_Texture = new Sml::Texture(shadowWidth, shadowHeight);
        m_Texture->updatePixels(blurredPixels);

        delete[] srcPixels;
        delete[] dstPixels;
        delete[] blurredPixels;
    }

    void Shadow::render(const Sml::Rectangle<int32_t>& object, const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (m_Texture == nullptr)
        {
            return;
        }

        Sml::Rectangle<int32_t> shadowRegion{0,
                                             0,
                                             static_cast<int32_t>(m_Texture->getWidth()),
                                             static_cast<int32_t>(m_Texture->getHeight())};

        shadowRegion = Sml::centerRegion(object, shadowRegion);
        shadowRegion.pos += m_Specification->getOffset() + targetRegion.pos;

        int32_t scaledWidth  = static_cast<int32_t>(m_Texture->getWidth());
        int32_t scaledHeight = static_cast<int32_t>(m_Texture->getHeight());

        shadowRegion.pos.x -= (scaledWidth  - shadowRegion.width)  / 2;
        shadowRegion.pos.y -= (scaledHeight - shadowRegion.height) / 2;
        shadowRegion.width  = scaledWidth;
        shadowRegion.height = scaledHeight;

        Sml::renderTexture(*m_Texture, &shadowRegion, nullptr);
    }

    // void Shadow::renderRectangular(const Shadow* shadow, const Sml::Rectangle<int32_t>& object,
    //                                const Sml::Rectangle<int32_t>& targetRegion)
    // {
    //     assert(shadow);

    //     Sml::Rectangle<int32_t> rectangle{targetRegion.pos + object.pos + shadow->m_Offset,
    //                                       object.width, object.height};

    //     rectangle.width  *= shadow->getScale().x;
    //     rectangle.height *= shadow->getScale().y;
    //     rectangle.pos.x  -= (rectangle.width  - object.width)  / 2;
    //     rectangle.pos.y  -= (rectangle.height - object.height) / 2;

    //     Sml::Renderer::getInstance().setColor(shadow->m_Color);
    //     Sml::renderFilledRect(rectangle);

    //     rectangle.pos.x  -= shadow->getBlurRadius();
    //     rectangle.pos.y  -= shadow->getBlurRadius();
    //     rectangle.width  += 2 * shadow->getBlurRadius();
    //     rectangle.height += 2 * shadow->getBlurRadius();

    //     if (rectangle.pos.x >= targetRegion.width || rectangle.pos.y >= targetRegion.height)
    //     {
    //         return;
    //     }

    //     if (rectangle.pos.x < 0)
    //     {
    //         rectangle.width += rectangle.pos.x;
    //         rectangle.pos.x  = 0;
    //     }

    //     if (rectangle.pos.y < 0)
    //     {
    //         rectangle.height += rectangle.pos.y;
    //         rectangle.pos.y   = 0;
    //     }

    //     Sml::applyKernel(shadow->m_BlurKernel, rectangle);
    // }
}