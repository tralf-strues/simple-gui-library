/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file container.cpp
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 */

#include "core.h"
#include "scene/containers/container.h"

namespace Sgl
{
    const ColorFill  Container::DEFAULT_FILL       = ColorFill{Sml::COLOR_WHITE};
    const Background Container::DEFAULT_BACKGROUND = Background{{&Container::DEFAULT_FILL}, {}};

    Container::Container() : Parent()
    {
        m_Background = &DEFAULT_BACKGROUND;
    }

    Component* Container::getHitComponent(int32_t x, int32_t y)
    {
        for (Component* child : getChildren())
        {
            Component* hitted = child->getHitComponent(x + getLayoutX(), y + getLayoutY());

            if (hitted != nullptr)
            {
                return hitted;
            }
        }

        return nullptr;
    }

    void Container::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        renderBackground(targetRegion);
        renderBorder(targetRegion);

        for (Component* child : getChildren())
        {
            Sml::Rectangle<int32_t> translatedRegion = targetRegion;
            translatedRegion.pos += Sml::Vec2<int32_t>{getLayoutX(), getLayoutY()};

            child->render(translatedRegion);
        }
    }

    void Container::renderBackground(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (m_Background == nullptr) { return; }

        for (const Fill* fill : m_Background->getFills())
        {
            fill->fillArea(targetRegion, targetRegion);
        }

        for (const Image* image : m_Background->getImages())
        {
            renderImage(image, targetRegion);
        }
    }

    void Container::renderBorder(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (m_Border == nullptr || m_Border->getThickness() == 0) { return; }

        getContextRenderer()->setColor(m_Border->getColor());
        Sml::renderRect(getContextRenderer(), targetRegion, m_Border->getThickness());
    }

    void Container::addChild(Component* child)
    {
        assert(child);
        assert(child->getParent() == nullptr);

        m_Children.push_back(child);
    }
    
    void Container::removeChild(Component* child)
    {
        assert(child);
        assert(child->getParent() == this);

        m_Children.remove(child);
    }
}