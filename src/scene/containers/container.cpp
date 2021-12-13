/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file container.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/containers/container.h"

namespace Sgl
{
    const ColorFill  Container::DEFAULT_FILL       = ColorFill{Sml::COLOR_WHITE};
    const Background Container::DEFAULT_BACKGROUND = Background{&Container::DEFAULT_FILL};

    Container::Container() : Parent()
    {
        m_Background = &DEFAULT_BACKGROUND;
    }

    Component* Container::getHitComponent(int32_t x, int32_t y)
    {
        if (!isInteractable()) { return nullptr; }

        Component* hitted = Parent::getHitComponent(x, y);

        if (hitted != nullptr)
        {
            return hitted;
        }

        if (Sml::isPointInsideRectangle({x, y}, getLayoutBounds()))
        {
            return this;
        }

        return nullptr;
    }

    void Container::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (!isVisible())
        {
            return;
        }

        renderShadow(targetRegion);

        Sml::Rectangle<int32_t> translatedTargetRegion = getLayoutBounds();
        translatedTargetRegion.pos += targetRegion.pos;

        if (m_Snapshot != nullptr)
        {
            Sml::Rectangle<int32_t> originBounds = getOriginBounds();
            renderTexture(*m_Snapshot, &translatedTargetRegion, &originBounds);
        }

        for (Component* child : getChildren())
        {
            child->render(translatedTargetRegion);
        }
    }

    void Container::prerenderSelf()
    {
        if (m_Background != nullptr)
        {
            Background::fillArea(m_Background, getOriginBounds());
        }

        if (m_Border != nullptr)
        {
            Border::encloseArea(m_Border, getOriginBounds());
        }
    }

    int32_t BlankContainer::computeCustomPrefWidth(int32_t height) const { return 0; }
    int32_t BlankContainer::computeCustomPrefHeight(int32_t width) const { return 0; }
}