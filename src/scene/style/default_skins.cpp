/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file default_skins.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/style/default_skins.h"
#include "scene/controls/button.h"
#include "core.h"

namespace Sgl
{
namespace DefaultSkins
{
    Sml::Font* g_DefaultFont = nullptr;

    #define ASSERT_CATEGORY(eventClass) \
        assert(event != nullptr);       \
        assert(event->isInCategory(eventClass::getStaticCategory()))

    #define ASSERT_EITHER_CATEGORY(eventClass1, eventClass2)           \
        assert(event != nullptr);                                      \
        assert(event->isInCategory(eventClass1::getStaticCategory() || \
                                   eventClass2::getStaticCategory()))

    #define INVALID_EVENT_TYPE(handler) LOG_ERROR("Invalid event type in " #handler)

    //------------------------------------------------------------------------------
    // ButtonSkin
    //------------------------------------------------------------------------------
    const Insets     ButtonSkin::PADDING                 = Insets{5, 10};
    const int32_t    ButtonSkin::MARGIN                  = 5;
    const Border     ButtonSkin::BORDER                  = Border{1, 0xE9'E9'E9'FF};

    /* Idle */
    const Sml::Color ButtonSkin::IDLE_FOREGROUND         = Sml::COLOR_BLACK;
    const ColorFill  ButtonSkin::IDLE_BACKGROUND_FILL    = {0xF5'F5'F5'FF};
    const Background ButtonSkin::IDLE_BACKGROUND         = {{&IDLE_BACKGROUND_FILL}, {}};
    
    /* Hovered */
    const Sml::Color ButtonSkin::HOVERED_FOREGROUND      = Sml::COLOR_WHITE;
    const ColorFill  ButtonSkin::HOVERED_BACKGROUND_FILL = {0x25'92'FF'FF};
    const Background ButtonSkin::HOVERED_BACKGROUND      = {{&HOVERED_BACKGROUND_FILL}, {}};

    const std::initializer_list<Sml::EventType> BUTTON_SKIN_LISTENER_EVENTS = {
            MouseEnteredEvent::getStaticType(),
            MouseExitedEvent::getStaticType(),
            Sml::MouseButtonPressedEvent::getStaticType()
        };

    class ButtonSkinEventListener : public Sml::Listener
    {
    public:
        Button& button;
        
        ButtonSkinEventListener(Button& button);

        virtual void onEvent(Sml::Event* event) override;
    };

    ButtonSkinEventListener::ButtonSkinEventListener(Button& button)
        : button(button) {}

    void ButtonSkinEventListener::onEvent(Sml::Event* event)
    {
        ASSERT_CATEGORY(Sml::MouseEvent);
        ButtonSkin& skin = static_cast<ButtonSkin&>(*button.getSkin());
        
        switch (event->getType())
        {
            case MouseEnteredEvent::getStaticType():
            {
                skin.setStyle(ButtonSkin::HOVERED_FOREGROUND, ButtonSkin::HOVERED_BACKGROUND);
                break;
            }

            case MouseExitedEvent::getStaticType():
            {
                skin.setStyle(ButtonSkin::IDLE_FOREGROUND, ButtonSkin::IDLE_BACKGROUND);
                break;
            }

            case Sml::MouseButtonPressedEvent::getStaticType():
            {
                if (button.getOnAction() != nullptr)
                {
                    // FIXME: Add event queue!
                    ActionEvent event{&button};
                    button.getOnAction()->onActionPerformed(&event);
                }

                break;
            }

            default:
            {
                INVALID_EVENT_TYPE(ButtonSkinEventListener);
                break;
            }
        }
    }

    ButtonSkin::ButtonSkin(Button* button)
        : m_Button(button),
          m_Text(*g_DefaultFont, nullptr, IDLE_FOREGROUND)
    {
        assert(m_Button);
    }

    void ButtonSkin::attach()
    {
        m_Handler = new ButtonSkinEventListener(*m_Button);
        m_Button->getEventDispatcher()->attachHandler(BUTTON_SKIN_LISTENER_EVENTS, m_Handler);

        m_Button->addChild(&m_Icon);
        m_Button->addChild(&m_Text);

        m_Button->setPadding(PADDING);
        m_Button->setBorder(&BORDER);

        setStyle(IDLE_FOREGROUND, IDLE_BACKGROUND);
    }

    void ButtonSkin::dispose()
    {
        delete m_Handler;

        m_Button->removeChild(&m_Icon);
        m_Button->removeChild(&m_Text);
    }

    void ButtonSkin::apply()
    {
        m_Text.setString(m_Button->getLabel());
        m_Icon.setImage(m_Button->getIcon());

        Sml::Texture* savedTarget = getContextRenderer()->getTarget();
        getContextRenderer()->setTarget(m_Button->getSnapshot());

        if (m_Button->getBackground() != nullptr)
        {
            Background::fillArea(*m_Button->getBackground(), m_Button->getOriginBounds());
        }

        if (m_Button->getBorder() != nullptr)
        {
            Border::encloseArea(*m_Button->getBorder(), m_Button->getOriginBounds());
        }

        getContextRenderer()->setTarget(savedTarget);
    }

    const Control* ButtonSkin::getControl() const { return m_Button; }
    Control* ButtonSkin::getModifiableControl() { return m_Button; }

    int32_t ButtonSkin::computePrefHeight(int32_t width) const
    {
        const Insets& padding = m_Button->getPadding();

        return padding.top + padding.bottom +
               std::max(m_Text.computePrefHeight(width), m_Icon.computePrefHeight(width));
    }

    int32_t ButtonSkin::computePrefWidth(int32_t height) const
    {
        const Insets& padding = m_Button->getPadding();

        int32_t textPrefWidth = m_Text.computePrefWidth(height);
        int32_t iconPrefWidth = m_Icon.computePrefWidth(height);

        return padding.left + padding.right +
               ((textPrefWidth != 0 && iconPrefWidth != 0) ? MARGIN : 0) +
               textPrefWidth + iconPrefWidth;
    }

    void ButtonSkin::layoutChildren()
    {
        m_Text.setLayoutWidth(m_Text.computePrefWidth());
        m_Text.setLayoutHeight(m_Text.computePrefHeight());

        m_Icon.setLayoutWidth(m_Icon.computePrefWidth());
        m_Icon.setLayoutHeight(m_Icon.computePrefHeight());

        const Insets& padding = m_Button->getPadding();
        Sml::Rectangle<int32_t> contentArea = m_Button->getOriginBounds();
        contentArea.pos    += {padding.left, padding.top};
        contentArea.width  -= padding.left + padding.right;
        contentArea.height -= padding.top + padding.bottom;

        int32_t centerY = contentArea.pos.y + contentArea.height / 2;

        m_Icon.setLayoutX(contentArea.pos.x);
        m_Icon.setLayoutY(centerY - m_Icon.getLayoutHeight() / 2);

        m_Text.setLayoutX(contentArea.pos.x + contentArea.width - m_Text.getLayoutWidth());
        m_Text.setLayoutY(centerY - m_Text.getLayoutHeight() / 2);
    }

    void ButtonSkin::setStyle(Sml::Color foreground, const Background& background)
    {
        m_Text.setColor(foreground);
        m_Button->setBackground(&background);
    }
}
}

// //------------------------------------------------------------------------------
// //! @author Nikita Mochalov (github.com/tralf-strues)
// //! @file default_skins.cpp
// //! @date 2021-11-01
// //! 
// //! @copyright Copyright (c) 2021
// //------------------------------------------------------------------------------

// #include "style/default_skins.h"
// #include "components/button.h"

// #define ASSERT_CATEGORY(eventClass) \
//     assert(event != nullptr);       \
//     assert(event->isInCategory(eventClass::getStaticCategory()))

// #define ASSERT_EITHER_CATEGORY(eventClass1, eventClass2)           \
//     assert(event != nullptr);                                      \
//     assert(event->isInCategory(eventClass1::getStaticCategory() || \
//                                eventClass2::getStaticCategory()))

// #define INVALID_EVENT_TYPE(listener) assert(! "Invalid" #listener "event type!") 

// void renderBorderedRect(Renderer* renderer, const Rectangle<int32_t>& region, const BorderedRect& style)
// {
//     assert(renderer);

//     if (style.backgroundEnabled)
//     {
//         renderer->setColor(style.background);
//         renderFilledRect(renderer, region);
//     }

//     if (style.borderEnabled)
//     {
//         renderer->setColor(style.borderColor);
//         renderRect(renderer, region, style.borderThickness);
//     }
// }

// //------------------------------------------------------------------------------
// // ButtonSkin
// //------------------------------------------------------------------------------
// const std::initializer_list<EventType> BUTTON_SKIN_LISTENER_EVENTS = {MouseEnteredEvent::getStaticType(),
//                                                                       MouseExitedEvent::getStaticType(),
//                                                                       MouseButtonPressedEvent::getStaticType()};

// ButtonSkinListener::ButtonSkinListener(Button* button) : button(button) {}

// void ButtonSkinListener::onEvent(Event* event)
// {
//     ASSERT_CATEGORY(MouseEvent);

//     BorderedRect newBackground = {};
//     Color        newForeground = 0;

//     switch (event->getType())
//     {
//         case MouseEnteredEvent::getStaticType():
//         {
//             newBackground = DEFAULT_BUTTON_BACKGROUND_HOVERED;
//             newForeground = DEFAULT_BUTTON_FOREGROUND_HOVERED;
//             break;
//         }

//         case MouseExitedEvent::getStaticType():
//         {
//             newBackground = DEFAULT_BUTTON_BACKGROUND;
//             newForeground = DEFAULT_BUTTON_FOREGROUND;
//             break;
//         }

//         case MouseButtonPressedEvent::getStaticType():
//         {
//             if (button->actionListener != nullptr)
//             {
//                 // FIXME: Add event queue!
//                 ActionEvent event{button};
//                 button->actionListener->onActionPerformed(&event);
//             }

//             return; // FIXME:
//         }

//         default:
//         {
//             INVALID_EVENT_TYPE(ButtonSkinHoverListener);
//             break;
//         }
//     }

//     button->setBackground(newBackground.background);
//     button->setForeground(newForeground);
//     button->updateGraphics();
// }

// ButtonSkin::ButtonSkin(Button* button) : Skin(button)
// {
//     assert(button);

//     m_Listener = new ButtonSkinListener(button);
//     button->getEventDispatcher()->attachHandler(BUTTON_SKIN_LISTENER_EVENTS, m_Listener);
// }

// void ButtonSkin::apply()
// {
//     Renderer* renderer = m_Component->getRenderer();
//     Texture* savedTarget = renderer->getTarget();
//     renderer->setTarget(m_Component->getTexture());

//     Rectangle<int32_t> componentRegion = m_Component->getRegion();
//     componentRegion.pos -= m_Component->getPos();

//     renderBorderedRect(m_Component->getRenderer(), componentRegion, m_Background);

//     Label* label = m_Component->getLabel();
//     if (label != nullptr)
//     {
//         label->render(m_Component->getTexture(), componentRegion);
//     }

//     renderer->setTarget(savedTarget);
// }

// void ButtonSkin::dispose()
// {
//     m_Component->getEventDispatcher()->detachHandler(m_Listener);
// }

// int32_t ButtonSkin::computePrefWidth() const
// {
//     Label* label = m_Component->getLabel();
//     if (!label->isEmpty())
//     {
//         return label->getPrefWidth() + 2 * DEFAULT_BUTTON_PREF_MARGINS.x;
//     }

//     return DEFAULT_BUTTON_PREF_SIZE.x;
// }

// int32_t ButtonSkin::computePrefHeight() const
// {
//     Label* label = m_Component->getLabel();
//     if (!label->isEmpty())
//     {
//         return label->getPrefHeight() + 2 * DEFAULT_BUTTON_PREF_MARGINS.y;
//     }

//     return DEFAULT_BUTTON_PREF_SIZE.y;
// }

// void ButtonSkin::layoutChildren() const
// {
//     Label* label = m_Component->getLabel();
//     if (!label->isEmpty())
//     {
//         label->setX((m_Component->getWidth()  - label->getWidth())  / 2);
//         label->setY((m_Component->getHeight() - label->getHeight()) / 2);
//     }
// }


// // void HoverListener::onEvent(Event* event)
// // {
// //     assert(event);
// //     assert(event->getCategory() == MouseEvent::getStaticCategory());

// //     const Skin* newSkin = nullptr;
// //     Color newForeground = 0;

// //     switch (event->getType())
// //     {
// //         case MouseEnteredEvent::getStaticType():
// //         {
// //             newSkin       = m_HoverStyle->skinHovered;
// //             newForeground = m_HoverStyle->foregroundHovered;
// //             break;
// //         }

// //         case MouseExitedEvent::getStaticType():
// //         {
// //             newSkin       = m_HoverStyle->skinNormal;
// //             newForeground = m_HoverStyle->foregroundNormal;
// //             break;
// //         }
    
// //         default:
// //         {
// //             assert(! "Invalid HoverListener event type!");
// //             break;
// //         }
// //     }

// //     m_Component->setSkin(newSkin);
// //     m_Component->setForeground(newForeground);
// //     m_Component->updateGraphics();
// // }