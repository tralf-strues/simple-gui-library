/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file insets.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <stdint.h>

namespace Sgl
{
    struct Insets
    {
        static const Insets EMPTY;

        Insets() = default;

        Insets(int32_t top, int32_t right, int32_t bottom, int32_t left);
        Insets(int32_t topBottom, int32_t rightLeft);
        Insets(int32_t inset);

        int32_t top    = 0;
        int32_t right  = 0;
        int32_t bottom = 0;
        int32_t left   = 0;

        Insets& operator+=(const Insets& second);
        Insets& operator-=(const Insets& second);
    };

    Insets operator+(const Insets& first, const Insets& second);
    Insets operator-(const Insets& first, const Insets& second);
}
