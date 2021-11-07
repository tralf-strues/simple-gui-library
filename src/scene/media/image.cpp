/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file image.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "core.h"
#include "scene/media/image.h"

namespace Sgl
{
    Image::Image(const char* filename, ImageFormat format)
        : m_Format(format), m_Texture(getContextRenderer())
    {
        m_Texture.loadFromImage(filename);
    }

    int32_t             Image::getWidth()   const { return m_Texture.getWidth();  }
    int32_t             Image::getHeight()  const { return m_Texture.getHeight(); }
    ImageFormat         Image::getFormat()  const { return m_Format;              }
    const Sml::Texture* Image::getTexture() const { return &m_Texture;            }

    void renderImage(const Image* image, const Sml::Rectangle<int32_t>& targetRegion)
    {
        assert(image);
        assert(image->getTexture());

        Sml::Renderer* contextRenderer = getContextRenderer();

        image->getTexture()->copyTo(contextRenderer->getTarget(), &targetRegion, nullptr);
    }
}