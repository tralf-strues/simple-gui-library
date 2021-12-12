/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "paint/background.h"
#include "paint/fill.h"
#include "media/image.h"

namespace Sgl
{
    Background::Background(const std::initializer_list<const Fill*>& fills,
                           const std::initializer_list<const Image*>& images)
    {
        m_Fills.insert(m_Fills.end(), fills);
        m_Images.insert(m_Images.end(), images);
    }

    Background::Background(const std::initializer_list<const Fill*>& fills)
    {
        m_Fills.insert(m_Fills.end(), fills);
    }

    Background::Background(const Fill* fill) : Background({fill}) {}

    void Background::addFill(const Fill* fill) { assert(fill); m_Fills.push_back(fill); }
    void Background::addImage(const Image* image) { assert(image); m_Images.push_back(image); }

    void Background::clearFills() { m_Fills.clear(); }
    void Background::clearImages() { m_Images.clear(); }

    const std::vector<const Fill*>& Background::getFills() const { return m_Fills; }
    const std::vector<const Image*> Background::getImages() const { return m_Images; }

    void Background::fillArea(const Background* background, const Sml::Rectangle<int32_t>& targetRegion)
    {
        for (const Fill* fill : background->getFills())
        {
            fill->fillArea({0, 0, targetRegion.width, targetRegion.height}, targetRegion);
        }

        for (const Image* image : background->getImages())
        {
            renderImage(image, targetRegion);
        }
    }
}