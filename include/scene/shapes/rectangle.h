/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file rectangle.h
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/math/rectangle.h"
#include "shape.h"

namespace Sgl
{
    class Rectangle : public Shape
    {
    public:
        Rectangle() = default;
        Rectangle(const Fill* fill);

        int32_t getWidth() const;
        void setWidth(int32_t width);

        int32_t getHeight() const;
        void setHeight(int32_t height);

        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override final;
        virtual int32_t computePrefHeight(int32_t width = -1) const override final;

    private:
        int32_t m_Width  = 0;
        int32_t m_Height = 0;
    };
};