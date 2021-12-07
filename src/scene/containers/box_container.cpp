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

    void BoxContainer::setGrowPriority(Component* component, GrowPriority priority)
    {
        m_GrowPriorities[component] = priority;
    }

    BoxContainer::GrowPriority BoxContainer::getGrowPriority(Component* component) const
    {
        auto priority = m_GrowPriorities.find(component);
        return priority == m_GrowPriorities.end() ? GrowPriority::NEVER : (*priority).second;
    }

    void BoxContainer::pushBackSpacer(uint32_t weight)
    {
        Component* prevComponent = m_Children.empty() ? nullptr : m_Children.back();
        m_Spacers.emplace_back(prevComponent, weight);

        mergeSpacers(prevComponent);
    }

    void BoxContainer::pushFrontSpacer(uint32_t weight)
    {
        m_Spacers.emplace_back(nullptr, weight);

        mergeSpacers(nullptr);
    }

    void BoxContainer::layoutChildren()
    {
        Sml::Rectangle<int32_t> contentArea = getContentArea();

        size_t   countSpacers           = m_Spacers.size();
        size_t   countGrowingComponents = computeGrowingComponentsCount(GrowPriority::ALWAYS);
        uint32_t totalSpacersWeight     = computeTotalSpacersWeight();

        if (m_Direction == Direction::LEFT_TO_RIGHT)
        {
            int32_t freeWidth                 = getLayoutWidth() - computeCustomPrefWidth();
            int32_t widthForSpacers           = (countGrowingComponents == 0) ? freeWidth : 0;
            int32_t widthForGrowingComponents = freeWidth - widthForSpacers;
            int32_t widthForGrowingComponent  = widthForGrowingComponents / countGrowingComponents;

            int32_t curX = contentArea.pos.x + computeSpacerSize(nullptr, totalSpacersWeight,
                                                                 widthForSpacers);
            int32_t centerY = contentArea.pos.y + contentArea.height / 2;

            for (Component* child : m_Children)
            {
                child->setLayoutX(curX);
                child->setLayoutWidth(child->computePrefWidth() + widthForGrowingComponent);
                curX += child->getLayoutWidth() + getSpacing() +
                        computeSpacerSize(child, totalSpacersWeight, widthForSpacers);

                child->setLayoutHeight(m_FillAcross ? contentArea.height : child->computePrefHeight());
                child->setLayoutY(centerY - child->getLayoutHeight() / 2);
            }
        }
        else if (m_Direction == Direction::TOP_TO_BOTTOM)
        {
            int32_t freeHeight                = getLayoutHeight() - computeCustomPrefHeight();
            int32_t heightForSpacers          = (countGrowingComponents == 0) ? freeHeight : 0;
            int32_t widthForGrowingComponents = freeHeight - heightForSpacers;
            int32_t widthForGrowingComponent  = widthForGrowingComponents / countGrowingComponents;

            int32_t curY = contentArea.pos.y + computeSpacerSize(nullptr, totalSpacersWeight,
                                                                 heightForSpacers);

            for (Component* child : m_Children)
            {
                child->setLayoutX(contentArea.pos.x);
                child->setLayoutWidth(m_FillAcross ? contentArea.width : child->computePrefWidth());

                child->setLayoutY(curY);
                child->setLayoutHeight(child->computePrefHeight() + widthForGrowingComponent);
                curY += child->getLayoutHeight() + getSpacing() +
                        computeSpacerSize(child, totalSpacersWeight, heightForSpacers);
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
        int32_t prefHeight = getInsets().top + getInsets().bottom;
        
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
    
    // TODO: implement
    int32_t BoxContainer::computeCustomMinWidth(int32_t height) const
    {
        return 0;
    }

    // TODO: implement
    int32_t BoxContainer::computeCustomMinHeight(int32_t width) const
    {
        return 0;
    }

    uint32_t BoxContainer::computeTotalSpacersWeight() const
    {
        uint32_t totalWeight = 0;
        for (const Spacer& spacer : m_Spacers)
        {
            totalWeight += spacer.weight;
        }

        return totalWeight;
    }

    int32_t BoxContainer::computeSpacerSize(Component* prev, uint32_t totalWeight, 
                                            int32_t totalSpacersSize) const
    {
        if (totalWeight == 0) { return 0; }

        for (const Spacer& spacer : m_Spacers)
        {
            if (spacer.prevComponent == prev)
            {
                return totalSpacersSize * (static_cast<float>(spacer.weight) / 
                                           static_cast<float>(totalWeight));
            }
        }

        return 0;
    }

    void BoxContainer::mergeSpacers(Component* prev)
    {
        static std::vector<std::list<Spacer>::iterator> spacersToErase;
        spacersToErase.clear();

        Spacer newSpacer{prev, 0};
        for (auto it = m_Spacers.begin(); it != m_Spacers.end(); ++it)
        {
            if (it->prevComponent == prev)
            {
                newSpacer.weight += it->weight;
                spacersToErase.push_back(it);
            }
        }

        for (auto spacerToErase : spacersToErase)
        {
            m_Spacers.erase(spacerToErase);
        }

        if (newSpacer.weight > 0)
        {
            m_Spacers.push_back(newSpacer);
        }
    }

    size_t BoxContainer::computeGrowingComponentsCount(GrowPriority priority) const
    {
        size_t count = 0;

        for (auto pair : m_GrowPriorities)
        {
            if (pair.second == priority)
            {
                ++count;
            }
        }

        return count;
    }

    HBox::HBox() : BoxContainer(BoxContainer::Direction::LEFT_TO_RIGHT) {}

    VBox::VBox() : BoxContainer(BoxContainer::Direction::TOP_TO_BOTTOM) {}
}