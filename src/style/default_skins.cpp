//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file default_skins.cpp
//! @date 2021-11-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "style/default_skins.h"
#include "component.h"

void HoverListener::onEvent(Event* event)
{
    assert(event);
    assert(event->getCategory() == MouseEvent::getStaticCategory());

    const Skin* newSkin = nullptr;
    Color newForeground = 0;

    switch (event->getType())
    {
        case MouseEnteredEvent::getStaticType():
        {
            newSkin       = m_HoverStyle->skinHovered;
            newForeground = m_HoverStyle->foregroundHovered;
            break;
        }

        case MouseExitedEvent::getStaticType():
        {
            newSkin       = m_HoverStyle->skinNormal;
            newForeground = m_HoverStyle->foregroundNormal;
            break;
        }
    
        default:
        {
            assert(! "Invalid HoverListener event type!");
            break;
        }
    }

    m_Component->setSkin(newSkin);
    m_Component->setForeground(newForeground);
    m_Component->updateGraphics();
}