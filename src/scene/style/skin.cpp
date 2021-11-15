/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file skin.cpp
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/graphics_wrapper/primitives.h"
#include "scene/style/skin.h"
#include "scene/controls/control.h"
#include "scene/utils.h"

namespace Sgl
{
    Component* Skin::getHitComponent(int32_t x, int32_t y)
    {
        if (Sml::isPointInsideRectangle({x, y}, getControl()->getLayoutBounds()))
        {
            return getModifiableControl();
        }

        return nullptr;
    }

    #define CHECK_ENABLED_SIZE(size) if (size != USE_COMPUTED_SIZE) { return size; }

    int32_t Skin::computePrefHeight(int32_t width) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Pref, Height, width, Y, control->getChildrenReadonly(),
                                    control->getPrefHeight(),
                                    control->getInsets().top + control->getInsets().bottom);
    }

    int32_t Skin::computePrefWidth(int32_t height) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Pref, Width, height, X, control->getChildrenReadonly(),
                                    control->getPrefWidth(),
                                    control->getInsets().left + control->getInsets().right);
    }

    int32_t Skin::computeMinHeight(int32_t width) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Min, Height, width, Y, control->getChildrenReadonly(),
                                    control->getMinHeight(),
                                    control->getInsets().top + control->getInsets().bottom);
    }

    int32_t Skin::computeMinWidth(int32_t height) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Min, Width, height, X, control->getChildrenReadonly(),
                                    control->getMinWidth(),
                                    control->getInsets().left + control->getInsets().right);
    }

    int32_t Skin::computeMaxHeight(int32_t width) const { return Component::UNLIMITED_SIZE; }
    int32_t Skin::computeMaxWidth(int32_t height) const { return Component::UNLIMITED_SIZE; }

    void Skin::layoutChildren()
    {
        Control* control = getModifiableControl();
        assert(control);

        for (Component* child : control->getChildren())
        {
            child->setLayoutWidth(child->computePrefWidth());
            child->setLayoutHeight(child->computePrefHeight());
        }
    }

    Skin* Skinnable::getSkin()
    {
        return m_Skin;
    }

    void Skinnable::setSkin(Skin* skin)
    {
        if (m_Skin != nullptr)
        {
            m_Skin->dispose();
        }

        m_Skin = skin;
    }
}