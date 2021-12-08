/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shape.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../component.h"
#include "../../paint/fill.h"
#include "../../paint/border.h"

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
        void setFill(const Fill* fill);

        const Border* getBorder() const;
        void setBorder(const Border* border);

        virtual int32_t computeMinWidth(int32_t height = -1) const override final;
        virtual int32_t computeMinHeight(int32_t width = -1) const override final;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override final;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override final;

    protected:
        const Fill*   m_Fill   = nullptr;
        const Border* m_Border = nullptr;
        // TODO: border, etc.
    };
}
