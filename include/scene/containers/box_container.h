/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file box_container.h
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "container.h"

namespace Sgl
{
    class BoxContainer : public Container
    {
    public:
        enum class Direction
        {
            LEFT_TO_RIGHT, TOP_TO_BOTTOM
        };

    public:
        BoxContainer(Direction direction);
        virtual ~BoxContainer() override = default;

        Direction getDirection() const;
        void setDirection(Direction direction);

        bool getFillAcross() const;
        void setFillAcross(bool fillAcross);

        int32_t getSpacing() const;
        void setSpacing(int32_t spacing);

        void pushBackSpacer(uint32_t weight = 1);
        void pushFrontSpacer(uint32_t weight = 1);

    protected:
        struct Spacer
        {
            Spacer(Component* prevComponent = nullptr, uint32_t weight = 1)
                : prevComponent(prevComponent), weight(weight) {}

            Component* prevComponent = nullptr; ///< If nullptr than at the beginning.
            uint32_t   weight        = 1;
        };

    protected:
        Direction         m_Direction  = Direction::LEFT_TO_RIGHT;
        bool              m_FillAcross = false;

        int32_t           m_Spacing    = 0;

        std::list<Spacer> m_Spacers;

    private:
        virtual void layoutChildren() override;

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeCustomMinWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomMinHeight(int32_t width = -1) const override;

        enum class SizeDimension { WIDTH, HEIGHT };

        int32_t computeTotalPrefSizeWithSpacing(SizeDimension sizeDimension) const;
        int32_t computeMaximumPrefSize(SizeDimension sizeDimension) const;

        uint32_t computeTotalSpacersWeight() const;
        int32_t computeSpacerSize(Component* prev, uint32_t totalWeight, int32_t totalSpacersSize) const;
        void mergeSpacers(Component* prev);
    };
}