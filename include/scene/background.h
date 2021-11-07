/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file background.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <vector>
#include "paint/fill.h"
#include "media/image.h"

namespace Sgl
{
    class Background
    {
    public:
        Background(const std::initializer_list<const Fill*>& fills,
                   const std::initializer_list<const Image*>& images);

        const std::vector<const Fill*>& getFills()  const;
        const std::vector<const Image*> getImages() const;

    private:
        std::vector<const Fill*>  m_Fills;
        std::vector<const Image*> m_Images;
    };
}