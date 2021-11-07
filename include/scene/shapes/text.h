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
        Text(const Sml::Font& font, const char* string, Sml::Color = Sml::COLOR_BLACK);

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void render(Sml::Texture* target, const Sml::Rectangle<int32_t>& targetRegion) override;

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

    private:
        Sml::Text m_Text;

        void update();
    };
}
