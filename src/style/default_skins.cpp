//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file default_skins.cpp
//! @date 2021-11-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "style/default_skins.h"
#include "components/button.h"

#define ASSERT_CATEGORY(eventClass) \
    assert(event != nullptr);       \
    assert(event->isInCategory(eventClass::getStaticCategory()))

#define ASSERT_EITHER_CATEGORY(eventClass1, eventClass2)           \
    assert(event != nullptr);                                      \
    assert(event->isInCategory(eventClass1::getStaticCategory() || \
                               eventClass2::getStaticCategory()))

#define INVALID_EVENT_TYPE(listener) assert(! "Invalid" #listener "event type!") 

void renderBorderedRect(Renderer* renderer, const Rectangle<int32_t>& region, const BorderedRect& style)
{
    assert(renderer);

    if (style.backgroundEnabled)
    {
        renderer->setColor(style.background);
        renderFilledRect(renderer, region);
    }

    if (style.borderEnabled)
    {
        renderer->setColor(style.borderColor);
        renderRect(renderer, region, style.borderThickness);
    }
}

//------------------------------------------------------------------------------
// ButtonSkin
//------------------------------------------------------------------------------
const std::initializer_list<EventType> BUTTON_SKIN_LISTENER_EVENTS = {MouseEnteredEvent::getStaticType(),
                                                                      MouseExitedEvent::getStaticType(),
                                                                      MouseButtonPressedEvent::getStaticType()};

ButtonSkinListener::ButtonSkinListener(Button* button) : button(button) {}

void ButtonSkinListener::onEvent(Event* event)
{
    ASSERT_CATEGORY(MouseEvent);

    BorderedRect newBackground = {};
    Color        newForeground = 0;

    switch (event->getType())
    {
        case MouseEnteredEvent::getStaticType():
        {
            newBackground = DEFAULT_BUTTON_BACKGROUND_HOVERED;
            newForeground = DEFAULT_BUTTON_FOREGROUND_HOVERED;
            break;
        }

        case MouseExitedEvent::getStaticType():
        {
            newBackground = DEFAULT_BUTTON_BACKGROUND;
            newForeground = DEFAULT_BUTTON_FOREGROUND;
            break;
        }

        case MouseButtonPressedEvent::getStaticType():
        {
            if (button->actionListener != nullptr)
            {
                // FIXME: Add event queue!
                ActionEvent event{button};
                button->actionListener->onActionPerformed(&event);
            }

            return; // FIXME:
        }

        default:
        {
            INVALID_EVENT_TYPE(ButtonSkinHoverListener);
            break;
        }
    }

    button->setBackground(newBackground.background);
    button->setForeground(newForeground);
    button->updateGraphics();
}

ButtonSkin::ButtonSkin(Button* button) : Skin(button)
{
    assert(button);

    m_Listener = new ButtonSkinListener(button);
    button->getEventDispatcher()->attachHandler(BUTTON_SKIN_LISTENER_EVENTS, m_Listener);
}

void ButtonSkin::apply()
{
    Renderer* renderer = m_Component->getRenderer();
    Texture* savedTarget = renderer->getTarget();
    renderer->setTarget(m_Component->getTexture());

    Rectangle<int32_t> componentRegion = m_Component->getRegion();
    componentRegion.pos -= m_Component->getPos();

    renderBorderedRect(m_Component->getRenderer(), componentRegion, m_Background);

    Label* label = m_Component->getLabel();
    if (label != nullptr)
    {
        label->render(m_Component->getTexture(), componentRegion);
    }

    renderer->setTarget(savedTarget);
}

void ButtonSkin::dispose()
{
    m_Component->getEventDispatcher()->detachHandler(m_Listener);
}

int32_t ButtonSkin::computePrefWidth() const
{
    Label* label = m_Component->getLabel();
    if (!label->isEmpty())
    {
        return label->getPrefWidth() + 2 * DEFAULT_BUTTON_PREF_MARGINS.x;
    }

    return DEFAULT_BUTTON_PREF_SIZE.x;
}

int32_t ButtonSkin::computePrefHeight() const
{
    Label* label = m_Component->getLabel();
    if (!label->isEmpty())
    {
        return label->getPrefHeight() + 2 * DEFAULT_BUTTON_PREF_MARGINS.y;
    }

    return DEFAULT_BUTTON_PREF_SIZE.y;
}

void ButtonSkin::layoutChildren() const
{
    Label* label = m_Component->getLabel();
    if (!label->isEmpty())
    {
        label->setX((m_Component->getWidth()  - label->getWidth())  / 2);
        label->setY((m_Component->getHeight() - label->getHeight()) / 2);
    }
}


// void HoverListener::onEvent(Event* event)
// {
//     assert(event);
//     assert(event->getCategory() == MouseEvent::getStaticCategory());

//     const Skin* newSkin = nullptr;
//     Color newForeground = 0;

//     switch (event->getType())
//     {
//         case MouseEnteredEvent::getStaticType():
//         {
//             newSkin       = m_HoverStyle->skinHovered;
//             newForeground = m_HoverStyle->foregroundHovered;
//             break;
//         }

//         case MouseExitedEvent::getStaticType():
//         {
//             newSkin       = m_HoverStyle->skinNormal;
//             newForeground = m_HoverStyle->foregroundNormal;
//             break;
//         }
    
//         default:
//         {
//             assert(! "Invalid HoverListener event type!");
//             break;
//         }
//     }

//     m_Component->setSkin(newSkin);
//     m_Component->setForeground(newForeground);
//     m_Component->updateGraphics();
// }