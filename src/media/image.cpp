/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file image.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/graphics_wrapper/renderer.h"
#include "media/image.h"

namespace Sgl
{
    Image::Image(const char* filename, ImageFormat format)
        : m_Format(format), m_Texture(new Sml::Texture())
    {
        m_Texture->loadFromImage(filename);
        assert(m_Texture->getWidth()  > 0);
        assert(m_Texture->getHeight() > 0);
    }

    Image::Image(const Sml::Texture* texture)
        : m_Format(ImageFormat::INVALID), m_Texture(texture->copy()) {}

    int32_t             Image::getWidth()   const { return m_Texture->getWidth();  }
    int32_t             Image::getHeight()  const { return m_Texture->getHeight(); }
    ImageFormat         Image::getFormat()  const { return m_Format;               }
    const Sml::Texture* Image::getTexture() const { return m_Texture;              }

    void renderImage(const Image* image, const Sml::Rectangle<int32_t>& targetRegion)
    {
        assert(image);
        assert(image->getTexture());

        image->getTexture()->copyTo(Sml::Renderer::getInstance().getTarget(), &targetRegion, nullptr);
    }

    void renderImage(const Image* image, const Sml::Rectangle<int32_t>& targetRegion,
                     int32_t scaledWidth, int32_t scaledHeight)
    {
        assert(image);
        assert(image->getTexture());

        if (scaledWidth <= 0 || scaledHeight <= 0) { return; }

        Sml::Rectangle<int32_t> scaledRegion{targetRegion.pos, scaledWidth, scaledHeight};
        image->getTexture()->copyTo(Sml::Renderer::getInstance().getTarget(), &scaledRegion, nullptr);
    }
}