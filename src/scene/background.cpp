/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/background.h"

namespace Sgl
{
    Background::Background(const std::initializer_list<const Fill*>& fills,
                           const std::initializer_list<const Image*>& images)
    {
        m_Fills.insert(m_Fills.end(), fills);
        m_Images.insert(m_Images.end(), images);
    }

    Background::Background(const Fill* fill)
        : Background({fill}, {}) {}

    const std::vector<const Fill*>& Background::getFills() const
    {
        return m_Fills;
    }

    const std::vector<const Image*> Background::getImages() const
    {
        return m_Images;
    }

    void Background::fillArea(const Background* background,
                              const Sml::Rectangle<int32_t>& targetRegion)
    {
        for (const Fill* fill : background->getFills())
        {
            fill->fillArea(targetRegion, targetRegion);
        }

        for (const Image* image : background->getImages())
        {
            renderImage(image, targetRegion);
        }
    }
}