/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shape.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../component.h"
#include "../paint/fill.h"

namespace Sgl
{
    class Shape : public Component
    {
    public:
        Shape(Fill* fill = nullptr);
        virtual ~Shape() override = default;

        virtual void layout() override final;
        virtual bool isResizable() const override final;

        const Fill* getFill() const;
        void setFill(Fill* fill);

        virtual int32_t computeMinWidth(int32_t height = -1) const override final;
        virtual int32_t computeMinHeight(int32_t width = -1) const override final;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override final;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override final;

    protected:
        Fill* m_Fill = nullptr;
        // TODO: border, etc.
    };
}
