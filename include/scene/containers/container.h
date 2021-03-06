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
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;

    protected:
        virtual void prerenderSelf() override;
    };
    
    class BlankContainer : public Container
    {
    public:
        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
    };
}