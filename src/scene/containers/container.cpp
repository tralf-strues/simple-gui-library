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

        if (Sml::isPointInsideRectangle({x, y}, getLayoutBounds()))
        {
            return this;
        }

        return nullptr;
    }

    void Container::prerenderSelf()
    {
        Sml::Texture* savedTarget = getContextRenderer()->getTarget();
        getContextRenderer()->setTarget(m_Snapshot);

        if (m_Background != nullptr)
        {
            Background::fillArea(*m_Background, getOriginBounds());
        }

        if (m_Border != nullptr)
        {
            Border::encloseArea(*m_Border, getOriginBounds());
        }

        getContextRenderer()->setTarget(savedTarget);
    }
}