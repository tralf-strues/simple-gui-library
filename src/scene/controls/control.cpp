/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file control.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/controls/control.h"

namespace Sgl
{
    Control::~Control()
    {
        if (m_DefaultSkin != nullptr)
        {
            delete m_DefaultSkin;
        }
    }

    Component* Control::getHitComponent(int32_t x, int32_t y)
    {
        for (Component* child : m_Children)
        {
            if (child->getHitComponent(x + getLayoutX(), y + getLayoutY()))
            {
                return child;
            }
        }

        if (m_Skin != nullptr)
        {
            return m_Skin->getHitComponent(x, y);
        }

        return nullptr;
    }

    void Control::layoutChildren()
    {
        if (m_Skin != nullptr)
        {
            m_Skin->layoutChildren();
        }
        else
        {
            Parent::layoutChildren();
        }
    }

    void Control::prerenderSelf()
    {
        if (m_Skin != nullptr)
        {
            m_Skin->apply();
        }
    }

    int32_t Control::computePrefWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computePrefWidth(height);
        }

        return 0;
    }

    int32_t Control::computePrefHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computePrefHeight(width);
        }

        return 0;
    }

    int32_t Control::computeMinWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMinWidth(height);
        }

        return 0;
    }

    int32_t Control::computeMinHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMinHeight(width);
        }

        return 0;
    }

    int32_t Control::computeMaxWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMaxWidth(height);
        }

        return 0;
    }

    int32_t Control::computeMaxHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMaxHeight(width);
        }

        return 0;
    }
}