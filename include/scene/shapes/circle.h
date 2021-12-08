/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file circle.h
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "shape.h"

namespace Sgl
{
    class Circle : public Shape
    {
    public:
        Circle() = default;
        Circle(int32_t radius, const Border* border);

        int32_t getRadius() const;
        void setRadius(int32_t radius);

        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override final;
        virtual int32_t computePrefHeight(int32_t width = -1) const override final;

    private:
        int32_t m_Radius = 0;
    };
};