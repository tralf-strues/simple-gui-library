/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file parent.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/parent.h"

namespace Sgl
{
    // TODO: layout only if NEEDS_LAYOUT or DIRTY_BRANCH
    void Parent::layout()
    {
        layoutChildren();

        for (Component* child : m_Children)
        {
            child->layout();
        }

        m_NeedLayoutPass = false;
    }

    void Parent::layoutChildren()
    {
        for (Component* component : m_Children)
        {
            component->setLayoutWidth(component->computePrefWidth());
            component->setLayoutHeight(component->computePrefHeight());
        }
    }

    bool Parent::isResizable() const { return true; }

    std::list<Component*> Parent::getChildren() { return m_Children; }

    bool Parent::needLayoutPass() const
    {
        return m_NeedLayoutPass;
    }

    void Parent::requestLayoutPass() { m_NeedLayoutPass = true; }

    const Background* Parent::getBackground() const { return m_Background; }
    void Parent::setBackground(const Background* background) { m_Background = background; }

    const Border* Parent::getBorder() const { return m_Border; }
    void Parent::setBorder(const Border* border) { m_Border = border; }

    Insets Parent::getInsets() const
    {
        return (m_Border == nullptr ? Insets::EMPTY : m_Border->getInsets()) + getPadding();
    }
    
    Insets Parent::getPadding() const { return m_Padding; }
    void Parent::setPadding(const Insets& padding) { m_Padding = padding; }

    void Parent::setPrefWidth (int32_t width)  { m_PrefWidth  = width;  }
    void Parent::setPrefHeight(int32_t height) { m_PrefHeight = height; }

    void Parent::setMinWidth (int32_t width)  { m_MinWidth  = width;  }
    void Parent::setMinHeight(int32_t height) { m_MinHeight = height; }

    void Parent::setMaxWidth (int32_t width)  { m_MaxWidth  = width;  }
    void Parent::setMaxHeight(int32_t height) { m_MaxHeight = height; }

    #define CHECK_ENABLED_SIZE(size) if (size != USE_COMPUTED_SIZE) { return size; }

    int32_t Parent::computePrefWidth(int32_t height)
    {
        CHECK_ENABLED_SIZE(m_PrefWidth);

        int32_t minX = INT32_MAX;
        int32_t maxX = 0;
        for (Component* child : m_Children)
        {
            int32_t childX     = child->getLayoutX();
            int32_t childWidth = child->computePrefWidth();

            if (childWidth == UNLIMITED_SIZE)
            {
                return UNLIMITED_SIZE;
            }

            minX = std::min(minX, childX);
            maxX = std::max(maxX, childX + childWidth);
        }

        if (minX != INT32_MAX)
        {
            return maxX - minX;
        }

        return 0;
    }

    int32_t Parent::computePrefHeight(int32_t width)
    {
        CHECK_ENABLED_SIZE(m_PrefHeight);

        int32_t minY = INT32_MAX;
        int32_t maxY = 0;
        for (Component* child : m_Children)
        {
            int32_t childY      = child->getLayoutY();
            int32_t childHeight = child->computePrefHeight();

            if (childHeight == UNLIMITED_SIZE)
            {
                return UNLIMITED_SIZE;
            }

            minY = std::min(minY, childY);
            maxY = std::max(maxY, childY + childHeight);
        }

        if (minY != INT32_MAX)
        {
            return maxY - minY;
        }

        return 0;
    } 
    
    int32_t Parent::computeMinWidth(int32_t height)
    {
        CHECK_ENABLED_SIZE(m_MinWidth);
        return computePrefWidth();
    }

    int32_t Parent::computeMinHeight(int32_t width)
    {
        CHECK_ENABLED_SIZE(m_MinHeight);
        return computePrefHeight();
    }

    int32_t Parent::computeMaxWidth(int32_t height)
    { 
        CHECK_ENABLED_SIZE(m_MaxWidth);
        return UNLIMITED_SIZE;
    }

    int32_t Parent::computeMaxHeight(int32_t width)
    {
        CHECK_ENABLED_SIZE(m_MaxHeight);
        return UNLIMITED_SIZE;
    }
}