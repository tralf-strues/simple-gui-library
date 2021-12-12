/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file shape.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../component.h"
#include "../../paint/background.h"
#include "../../paint/border.h"

namespace Sgl
{
    class Shape : public Component
    {
    public:
        Shape(Sml::Color fillColor = Sml::COLOR_TRANSPARENT);
        Shape(const Background* background);
        virtual ~Shape() override = default;

        virtual void layout() override final;
        virtual bool isResizable() const override final;

        // const std::vector<const Fill*>& getFills() const;
        // void addFill(const Fill* fill);

        // void setFill(const Fill* fill);

        Sml::Color getFillColor() const;
        void setFillColor(Sml::Color color);

        const Background* getBackground() const;
        void setBackground(const Background* background);

        const Border* getBorder() const;
        void setBorder(const Border* border);

        virtual int32_t computeMinWidth(int32_t height = -1) const override final;
        virtual int32_t computeMinHeight(int32_t width = -1) const override final;

        virtual int32_t computeMaxWidth(int32_t height = -1) const override final;
        virtual int32_t computeMaxHeight(int32_t width = -1) const override final;

    protected:
        Sml::Color        m_FillColor  = Sml::COLOR_TRANSPARENT;
        const Background* m_Background = nullptr;
        const Border*     m_Border     = nullptr;
        // std::vector<const Fill*> m_Fills;
        // TODO: border, etc.
    };
}
