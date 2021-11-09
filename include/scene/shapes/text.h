/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file text.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/graphics_wrapper/text.h"
#include "shape.h"

namespace Sgl
{
    class Text : public Shape
    {
    public:
        Text(const Sml::Font& font, const char* string, Sml::Color color = Sml::COLOR_BLACK);

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;
        virtual void prerender() override;

        const Sml::Font& getFont() const;
        Sml::Color getColor() const;
        const char* getStr() const;
        size_t getWidth() const;
        size_t getHeight() const;
        size_t getWrapWidth() const;

        void setFont(const Sml::Font& font);
        void setColor(Sml::Color color);
        void setString(const char* str);
        void setWrapWidth(size_t wrapWidth);

        virtual int32_t computePrefWidth(int32_t height = -1) const override final;
        virtual int32_t computePrefHeight(int32_t width = -1) const override final;

    private:
        Sml::Text m_Text;
    };
}
