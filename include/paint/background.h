/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <vector>
#include "sml/math/rectangle.h"
#include "fill.h"
#include "../media/image.h"

namespace Sgl
{
    class Fill;
    class Image;

    class Background
    {
    public:
        static void fillArea(const Background* background,
                             const Sml::Rectangle<int32_t>& targetRegion);

    public:
        Background() = default;

        Background(const std::initializer_list<const Fill*>& fills,
                   const std::initializer_list<const Image*>& images);

        Background(const std::initializer_list<const Fill*>& fills);

        Background(const Fill* fill);

        void addFill(const Fill* fill);
        void addImage(const Image* image);

        void clearFills();
        void clearImages();

        const std::vector<const Fill*>& getFills()  const;
        const std::vector<const Image*> getImages() const;

    private:
        std::vector<const Fill*>  m_Fills;
        std::vector<const Image*> m_Images;
    };
}