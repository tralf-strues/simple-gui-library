/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file box_container.h
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include <unordered_map>
#include "container.h"
#include "sys/syscall.h"

namespace Sgl
{
    /**
     * @brief Basic list-like container which lays out children in a line one after another.
     * 
     * Direction specifies how to lay out children - vertically vs. horizontally and
     * normal vs. reverse order (still-in-progress).
     * 
     * Spacing specifies how much space to put between children (@note: t)
     */
    class BoxContainer : public Container
    {
    public:
        enum class Direction
        {
            LEFT_TO_RIGHT, TOP_TO_BOTTOM
        };

        enum class GrowPriority
        {
            NEVER, ALWAYS
        };

    public:
        BoxContainer() = default;
        BoxContainer(Direction direction);
        virtual ~BoxContainer() override = default;

        Direction getDirection() const;
        void setDirection(Direction direction);

        bool getFillAcross() const;
        void setFillAcross(bool fillAcross);

        int32_t getSpacing() const;
        void setSpacing(int32_t spacing);

        void setGrowPriority(Component* component, GrowPriority priority);
        GrowPriority getGrowPriority(Component* component) const;

        void pushBackSpacer(uint32_t weight = 1);
        void pushFrontSpacer(uint32_t weight = 1);

    private:
        struct Spacer
        {
            Spacer(Component* prevComponent = nullptr, uint32_t weight = 1)
                : prevComponent(prevComponent), weight(weight) {}

            Component* prevComponent = nullptr; ///< If nullptr than at the beginning.
            uint32_t   weight        = 1;
        };

    private:
        Direction         m_Direction  = Direction::LEFT_TO_RIGHT;
        bool              m_FillAcross = false;
        int32_t           m_Spacing    = 0;

        std::list<Spacer> m_Spacers;

        /**
         * By default, only components with GrowPriority::ALWAYS will share free space.
         * Neither components with GrowPriority::NEVER, nor components with no GrowPriority
         * set will get any free space.
         */
        std::unordered_map<Component*, GrowPriority> m_GrowPriorities;

    private:
        virtual void layoutChildren() override;

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeCustomMinWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomMinHeight(int32_t width = -1) const override;

        enum class SizeDimension { WIDTH, HEIGHT };

        int32_t computeTotalPrefSizeWithSpacing(SizeDimension sizeDimension) const;
        int32_t computeMaximumPrefSize(SizeDimension sizeDimension) const;

        void splitFreeSpace(int32_t freeSpace, size_t countSpacers, size_t countGrowingComponents,
                            int32_t* totalSpacersSpace, int32_t* totalGrowingComponentsSpace) const;

        uint32_t computeTotalSpacersWeight() const;
        int32_t computeSpacerSize(Component* prev, uint32_t totalWeight, int32_t totalSpacersSize) const;
        void mergeSpacers(Component* prev);

        size_t computeGrowingComponentsCount(GrowPriority priority) const;
    };

    class HBox : public BoxContainer
    {
    public:
        HBox();
        virtual ~HBox() override = default;
    };

    class VBox : public BoxContainer
    {
    public:
        VBox();
        virtual ~VBox() override = default;
    };
}