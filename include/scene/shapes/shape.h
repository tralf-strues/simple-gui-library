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

        const Fill* getFill() const;
        void setFill(Fill* fill);

    protected:
        Fill* m_Fill = nullptr;
        // TODO: border, etc.
    };
}
