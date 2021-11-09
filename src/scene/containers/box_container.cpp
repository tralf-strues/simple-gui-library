/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file box_container.cpp
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/containers/box_container.h"

namespace Sgl
{
    BoxContainer::BoxContainer(Direction direction)
        : m_Direction(direction) {}

    BoxContainer::Direction BoxContainer::getDirection() const { return m_Direction; }
    void BoxContainer::setDirection(Direction direction) { m_Direction = direction; }

    bool BoxContainer::getFillAcross() const { return m_FillAcross; }
    void BoxContainer::setFillAcross(bool fillAcross) { m_FillAcross = fillAcross; }

    int32_t BoxContainer::getSpacing() const { return m_Spacing; }
    void BoxContainer::setSpacing(int32_t spacing) { m_Spacing = spacing; }

    void BoxContainer::layoutChildren()
    {
        if (m_Direction == Direction::LEFT_TO_RIGHT)
        {
            int32_t curX    = getInsets().left;
            int32_t centerY = getInsets().top +
                              (getLayoutHeight() - getInsets().top - getInsets().bottom) / 2;

            for (Component* child : m_Children)
            {
                child->setLayoutX(curX);
                child->setLayoutWidth(child->computePrefWidth());
                curX += getSpacing();

                child->setLayoutHeight(m_FillAcross ? getLayoutHeight() : child->computePrefHeight());
                child->setLayoutY(centerY - child->getLayoutHeight() / 2);
            }
        }
    }

    int32_t BoxContainer::computeTotalPrefSizeWithSpacing(SizeDimension sizeDimension) const
    {
        int32_t totalPrefSize = getSpacing() * (m_Children.size() - 1);

        for (Component* child : m_Children)
        {
            if (sizeDimension == SizeDimension::WIDTH)
            {
                totalPrefSize += child->computePrefWidth();
            }
            else
            {
                totalPrefSize += child->computePrefHeight();
            }
        }

        return totalPrefSize;
    }

    int32_t BoxContainer::computeMaximumPrefSize(SizeDimension sizeDimension) const
    {
        int32_t maxPrefSize = 0;

        for (Component* child : m_Children)
        {
            int32_t prefSize = 0;

            if (sizeDimension == SizeDimension::WIDTH)
            {
                prefSize = child->computePrefWidth();
            }
            else
            {
                prefSize = child->computePrefHeight();
            }

            if (prefSize > maxPrefSize)
            {
                maxPrefSize = prefSize;
            }
        }

        return maxPrefSize;
    }

    int32_t BoxContainer::computeCustomPrefWidth(int32_t height) const
    {
        int32_t prefWidth = getInsets().left + getInsets().right;
        
        if (m_Children.empty())
        { 
            return prefWidth;
        }

        if (m_Direction == Direction::LEFT_TO_RIGHT)
        {
            prefWidth += computeTotalPrefSizeWithSpacing(SizeDimension::WIDTH);
        }
        else if (m_Direction == Direction::TOP_TO_BOTTOM)
        {
            prefWidth += computeMaximumPrefSize(SizeDimension::WIDTH);
        }

        return prefWidth;
    }

    int32_t BoxContainer::computeCustomPrefHeight(int32_t width) const
    {
        int32_t prefHeight = getInsets().left + getInsets().right;
        
        if (m_Children.empty())
        { 
            return prefHeight;
        }

        if (m_Direction == Direction::LEFT_TO_RIGHT)
        {
            prefHeight += computeMaximumPrefSize(SizeDimension::HEIGHT);
        }
        else if (m_Direction == Direction::TOP_TO_BOTTOM)
        {
            prefHeight += computeTotalPrefSizeWithSpacing(SizeDimension::HEIGHT);
        }

        return prefHeight;
    }
    
    // FIXME: implement
    int32_t BoxContainer::computeCustomMinWidth(int32_t height) const
    {
        return 0;
    }

    // FIXME: implement
    int32_t BoxContainer::computeCustomMinHeight(int32_t width) const
    {
        return 0;
    }
}