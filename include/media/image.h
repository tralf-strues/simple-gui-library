/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file image.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/texture.h"

namespace Sgl
{
    enum class ImageFormat
    {
        INVALID,
        BMP,
        PNG
    };

    class Image
    {
    public:
        Image(const char* filename, ImageFormat format);
        Image(const Sml::Texture* texture);
        ~Image() = default;

        int32_t             getWidth()   const;
        int32_t             getHeight()  const;
        ImageFormat         getFormat()  const;
        const Sml::Texture* getTexture() const;

    private:
        ImageFormat   m_Format  = ImageFormat::INVALID;
        Sml::Texture* m_Texture = nullptr;
    };

    void renderImage(const Image* image, const Sml::Rectangle<int32_t>& targetRegion);
    void renderImage(const Image* image, const Sml::Rectangle<int32_t>& targetRegion,
                     int32_t scaledWidth, int32_t scaledHeight);
}