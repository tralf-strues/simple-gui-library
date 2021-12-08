/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file fill.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <vector>
#include "sml/math/vec2.h"
#include "sml/math/rectangle.h"
#include "sml/graphics_wrapper/texture.h"

namespace Sgl
{
    class Fill
    {
    public:
        virtual void fillLine(const Sml::Vec2i& start,
                              const Sml::Vec2i& end,
                              const Sml::Rectangle<int32_t>& targetRegion) const = 0;

        virtual void fillArea(const Sml::Rectangle<int32_t>& area,
                              const Sml::Rectangle<int32_t>& targetRegion) const = 0;

        virtual void fillPoint(const Sml::Vec2i& point,
                               const Sml::Rectangle<int32_t>& targetRegion) const = 0;
    };

    //------------------------------------------------------------------------------
    // ColorFill
    //------------------------------------------------------------------------------
    class ColorFill : public Fill
    {
    public:
        ColorFill(Sml::Color color = Sml::COLOR_TRANSPARENT);

        virtual void fillLine(const Sml::Vec2i& start,
                              const Sml::Vec2i& end,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillArea(const Sml::Rectangle<int32_t>& area,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillPoint(const Sml::Vec2i& point,
                               const Sml::Rectangle<int32_t>& targetRegion) const override;

        Sml::Color getColor() const;
        void setColor(Sml::Color color);

    protected:
        Sml::Color m_Color;
    };

    //------------------------------------------------------------------------------
    // LinearGradientFill
    //------------------------------------------------------------------------------
    // TODO:
    class LinearGradientFill : public Fill
    {
    public:
        class Stop
        {
        public:
            Stop(float offset, Sml::Color color);

            float getOffset() const;
            const Sml::Vec4f& getColorVector() const;
            Sml::Color getColor() const;

            bool operator<(const Stop& other) const;

        private:
            float      m_Offset = 0.0; ///< Number ranging from 0 to 1
            Sml::Vec4f m_Color  = Sml::COLOR_TRANSPARENT;
        };

    public:
        LinearGradientFill() = default;
        LinearGradientFill(const std::initializer_list<Stop>& stops);

        void addStop(const Stop& stop);

        void setVertical();
        void setHorizontal();

        virtual void fillLine(const Sml::Vec2i& start,
                              const Sml::Vec2i& end,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillArea(const Sml::Rectangle<int32_t>& area,
                              const Sml::Rectangle<int32_t>& targetRegion) const override;

        virtual void fillPoint(const Sml::Vec2i& point,
                               const Sml::Rectangle<int32_t>& targetRegion) const override;

    private:
        std::vector<Stop> m_Stops;
        bool              m_IsVertical = false;
    };
}
