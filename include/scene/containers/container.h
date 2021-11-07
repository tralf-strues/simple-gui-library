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
        virtual void render(const Sml::Rectangle<int32_t> &targetRegion) override;

        void addChild(Component* child);
        void removeChild(Component* child);

    protected:
        void renderBackground(const Sml::Rectangle<int32_t>& targetRegion);
        void renderBorder(const Sml::Rectangle<int32_t>& targetRegion);
    };
}