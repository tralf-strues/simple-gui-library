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
                                    control->getPrefHeight());
    }

    int32_t Skin::computePrefWidth(int32_t height) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Pref, Width, height, X, control->getChildrenReadonly(),
                                    control->getPrefWidth());
    }

    int32_t Skin::computeMinHeight(int32_t width) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Min, Height, width, Y, control->getChildrenReadonly(),
                                    control->getMinHeight());
    }

    int32_t Skin::computeMinWidth(int32_t height) const
    {
        const Control* control = getControl();
        assert(control);

        COMPONENT_COMPUTE_DIMENSION(Min, Width, height, X, control->getChildrenReadonly(),
                                    control->getMinWidth());
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

    void Skinnable::applySkin()
    {
        if (m_Skin != nullptr)
        {
            m_Skin->apply();
        }
    }
}


// const Skin* Skinable::getSkin() const
// {
//     return m_Skin;
// }

// void Skinable::setSkin(const Skin* skin)
// {
//     m_Skin = skin;
// }

// void IconSkin::apply(Renderer* renderer, Texture* target, const Rectangle<int32_t>* targetRegion) const
// {
//     assert(renderer);
//     renderTexture(renderer, *m_Icon, targetRegion, nullptr);
// }

// BasicSkin::BasicSkin(bool backgroundEnabled, Color background,
//                      bool borderEnabled, Color borderColor, int32_t borderThickness)
//     : m_BackgroundEnabled(backgroundEnabled), m_BackgroundColor(background),
//       m_BorderEnabled(borderEnabled), m_BorderColor(borderColor), m_BorderThickness(borderThickness)
// {
// }

// void BasicSkin::apply(Renderer* renderer, Texture* target, const Rectangle<int32_t>* targetRegion) const
// {
//     assert(renderer);

//     Rectangle<int32_t> region{{0, 0}, static_cast<int32_t>(target->getWidth()), 
//                                       static_cast<int32_t>(target->getHeight())};
//     if (targetRegion != nullptr)
//     {
//         region = *targetRegion;
//     }

//     Texture* savedTarget = renderer->getTarget();
//     renderer->setTarget(target);

//     renderBackground(renderer, target, region);
//     renderBorder(renderer, target, region);

//     renderer->setTarget(savedTarget);
// }

// void BasicSkin::renderBackground(Renderer* renderer, Texture* target, const Rectangle<int32_t>& region) const
// {
//     assert(renderer);
//     assert(target);

//     if (!m_BackgroundEnabled)
//     {
//         return;
//     }

//     renderer->setColor(m_BackgroundColor);
//     renderFilledRect(renderer, region);
// }

// void BasicSkin::renderBorder(Renderer* renderer, Texture* target, const Rectangle<int32_t>& region) const
// {
//     assert(renderer);
//     assert(target);

//     if (!m_BorderEnabled)
//     {
//         return;
//     }

//     renderer->setColor(m_BorderColor);
    
//     Rectangle<int32_t> curBorder = region;
//     for (int32_t step = 0; step < m_BorderThickness; ++step)
//     {
//         renderRect(renderer, curBorder);
//         curBorder.pos += Vec2{1, 1};

//         curBorder.width  -= 2;
//         curBorder.height -= 2;
//     }
// }

// bool BasicSkin::isBorderEnabled() const
// {
//     return m_BorderEnabled;
// }

// Color BasicSkin::getBorderColor() const
// {
//     return m_BorderColor;
// }

// int32_t BasicSkin::getBorderThickness() const
// {
//     return m_BorderThickness;
// }

// void BasicSkin::setBorderEnabled(bool enabled)
// {
//     m_BorderEnabled = enabled;
// }

// void BasicSkin::setBorderColor(Color color)
// {
//     m_BorderColor = color;
// }

// void BasicSkin::setBorderThickness(int32_t thickness)
// {
//     m_BorderThickness = thickness;
// }

// bool BasicSkin::isBackgroundEnabled() const
// {
//     return m_BackgroundEnabled;
// }

// Color BasicSkin::getBackgroundColor() const
// {
//     return m_BackgroundColor;
// }

// void BasicSkin::setBackgroundEnabled(bool enabled)
// {
//     m_BackgroundEnabled = enabled;
// }

// void BasicSkin::setBackgroundColor(Color color)
// {
//     m_BackgroundColor = color;
// }
