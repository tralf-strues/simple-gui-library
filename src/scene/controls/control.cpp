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

        if (m_Skin != m_DefaultSkin && m_Skin != nullptr)
        {
            delete m_Skin;
        }
    }

    Component* Control::getHitComponent(int32_t x, int32_t y)
    {
        if (!isInteractable()) { return nullptr; }

        Component* hitted = Parent::getHitComponent(x, y);

        if (hitted != nullptr)
        {
            return hitted;
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
            Sml::Renderer::getInstance().pushTarget();
            Sml::Renderer::getInstance().setTarget(getSnapshot());
            
            m_Skin->prerenderControl();

            Sml::Renderer::getInstance().popTarget();
        }
    }

    int32_t Control::computeCustomPrefWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computePrefWidth(height);
        }

        return 0;
    }

    int32_t Control::computeCustomPrefHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computePrefHeight(width);
        }

        return 0;
    }

    int32_t Control::computeCustomMinWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMinWidth(height);
        }

        return 0;
    }

    int32_t Control::computeCustomMinHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMinHeight(width);
        }

        return 0;
    }

    int32_t Control::computeCustomMaxWidth(int32_t height) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMaxWidth(height);
        }

        return 0;
    }

    int32_t Control::computeCustomMaxHeight(int32_t width) const
    {
        if (m_Skin != nullptr)
        {
            return m_Skin->computeMaxHeight(width);
        }

        return 0;
    }
}
