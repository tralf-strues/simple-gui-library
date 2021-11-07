/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file insets.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/insets.h"

namespace Sgl
{
    const Insets Insets::EMPTY = Insets{0, 0, 0, 0};

    Insets::Insets(int32_t top, int32_t right, int32_t bottom, int32_t left)
        : top(top), right(right), bottom(bottom), left(left) {}

    Insets::Insets(int32_t topBottom, int32_t rightLeft)
        : Insets(topBottom, rightLeft, topBottom, rightLeft) {}

    Insets::Insets(int32_t inset)
        : Insets(inset, inset) {}

    Insets& Insets::operator+=(const Insets& second)
    {
        top    += second.top;
        right  += second.right;
        bottom += second.bottom;
        left   += second.left;

        return *this;
    }

    Insets& Insets::operator-=(const Insets& second)
    {
        top    -= second.top;
        right  -= second.right;
        bottom -= second.bottom;
        left   -= second.left;

        return *this;
    }

    Insets operator+(const Insets& first, const Insets& second)
    {
        Insets sum{first};
        sum += second;

        return sum;
    }

    Insets operator-(const Insets& first, const Insets& second)
    {
        Insets dif{first};
        dif -= second;

        return dif;
    }
}