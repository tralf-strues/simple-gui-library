/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file container.h
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../parent.h"

namespace Sgl
{
    class Container : public Parent
    {
    public:
        static const ColorFill  DEFAULT_FILL;
        static const Background DEFAULT_BACKGROUND;

    public:
        Container();
        virtual ~Container() override = default;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;

    protected:
        virtual void prerenderSelf() override;
    };
}