/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file default_skins.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/style/default_skins.h"
#include "scene/controls/button.h"

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

    #define INVALID_EVENT_TYPE(handler) LOG_LIB_ERROR("Invalid event type in " #handler)

    //------------------------------------------------------------------------------
    // ButtonBaseSkin
    //------------------------------------------------------------------------------
    class ButtonBaseSkinEventListener : public ComponentEventListener<Button>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(MouseEnteredEvent::getStaticType(),
                                           MouseExitedEvent::getStaticType(),
                                           Sml::MouseButtonPressedEvent::getStaticType())

    public:
        ButtonBaseSkinEventListener(Button* button) : ComponentEventListener<Button>(button) {}

        virtual void onEvent(Sml::Event* event) override
        {
            ASSERT_CATEGORY(Sml::MouseEvent);
            ButtonBaseSkin& skin = dynamic_cast<ButtonBaseSkin&>(*getComponent()->getSkin());
            
            switch (event->getType())
            {
                case MouseEnteredEvent::getStaticType():
                {
                    skin.applyInteractionStyle(ButtonBaseSkin::InteractionStyle::Type::HOVERED);
                    break;
                }

                case MouseExitedEvent::getStaticType():
                {
                    skin.applyInteractionStyle(ButtonBaseSkin::InteractionStyle::Type::IDLE);
                    break;
                }

                case Sml::MouseButtonPressedEvent::getStaticType():
                {
                    if (getComponent()->getOnAction() != nullptr)
                    {
                        // FIXME: Add event queue!
                        ActionEvent event{getComponent()};
                        getComponent()->getOnAction()->onAction(&event);
                    }

                    break;
                }

                default:
                {
                    INVALID_EVENT_TYPE(ButtonBaseSkinEventListener);
                    break;
                }
            }

            event->consume();
        }
    };

    ButtonBaseSkin::ButtonBaseSkin(const StaticStyle* staticStyle,
                                   const InteractionStyle* idleStyle,
                                   const InteractionStyle* hoveredStyle,
                                   const InteractionStyle* pressedStyle)
        : m_StaticStyle(staticStyle),
          m_IdleStyle(idleStyle),
          m_HoveredStyle(hoveredStyle),
          m_PressedStyle(pressedStyle),
          m_Text(*g_DefaultFont, nullptr) {}

    ButtonBaseSkin::ButtonBaseSkin(Button* button,
                                   const StaticStyle* staticStyle,
                                   const InteractionStyle* idleStyle,
                                   const InteractionStyle* hoveredStyle,
                                   const InteractionStyle* pressedStyle)
        : ButtonBaseSkin(staticStyle, idleStyle, hoveredStyle, pressedStyle)
    {
        assert(button);
        attach(button);
    }

    void ButtonBaseSkin::attach(Button* button)
    {
        m_Button = button;

        m_Handler = new ButtonBaseSkinEventListener(m_Button);
        m_Button->getEventDispatcher()->attachHandler(ButtonBaseSkinEventListener::EVENT_TYPES, m_Handler);

        m_Button->addChild(&m_Icon);
        m_Button->addChild(&m_Text);

        applyStaticStyle();
        applyInteractionStyle(InteractionStyle::Type::IDLE);
    }

    void ButtonBaseSkin::dispose()
    {
        m_Button->getEventDispatcher()->detachHandler(m_Handler);
        delete m_Handler;

        m_Button->removeChild(&m_Icon);
        m_Button->removeChild(&m_Text);
    }

    void ButtonBaseSkin::prerenderControl()
    {
        Sml::Renderer::getInstance().pushTarget();
        Sml::Renderer::getInstance().setTarget(m_Button->getSnapshot());

        if (m_Button->getBackground() != nullptr)
        {
            Background::fillArea(m_Button->getBackground(), m_Button->getOriginBounds());
        }

        if (m_Button->getBorder() != nullptr)
        {
            Border::encloseArea(m_Button->getBorder(), m_Button->getOriginBounds());
        }

        Sml::Renderer::getInstance().popTarget();
    }

    const Control* ButtonBaseSkin::getControl() const { return m_Button; }
    Control* ButtonBaseSkin::getModifiableControl() { return m_Button; }

    int32_t ButtonBaseSkin::computePrefHeight(int32_t width) const
    {
        Insets insets = m_Button->getInsets();

        return insets.top + insets.bottom +
               std::max(m_Text.computePrefHeight(width), m_Icon.computePrefHeight(width));
    }

    int32_t ButtonBaseSkin::computePrefWidth(int32_t height) const
    {
        Insets insets = m_Button->getInsets();

        int32_t textPrefWidth = m_Text.computePrefWidth(height);
        int32_t iconPrefWidth = m_Icon.computePrefWidth(height);

        return insets.left + insets.right +
               ((textPrefWidth != 0 && iconPrefWidth != 0) ? getMargin() : 0) +
               textPrefWidth + iconPrefWidth;
    }

    void ButtonBaseSkin::layoutChildren()
    {
        m_Text.setString(m_Button->getLabel());
        m_Icon.setImage(m_Button->getIcon());

        m_Text.setLayoutWidth(m_Text.computePrefWidth());
        m_Text.setLayoutHeight(m_Text.computePrefHeight());

        m_Icon.setLayoutWidth(m_Icon.computePrefWidth());
        m_Icon.setLayoutHeight(m_Icon.computePrefHeight());

        // const Insets& padding = m_Button->getPadding();
        Sml::Rectangle<int32_t> contentArea = m_Button->getContentArea();
        // contentArea.pos    += {padding.left, padding.top};
        // contentArea.width  -= padding.left + padding.right;
        // contentArea.height -= padding.top + padding.bottom;

        // Insets insets = m_Button->getInsets();

        int32_t centerY = contentArea.pos.y + contentArea.height / 2;

        m_Icon.setLayoutX(contentArea.pos.x);
        m_Icon.setLayoutY(centerY - m_Icon.getLayoutHeight() / 2);

        m_Text.setLayoutX(contentArea.pos.x + contentArea.width - m_Text.getLayoutWidth());
        m_Text.setLayoutY(centerY - m_Text.getLayoutHeight() / 2);
    }

    int32_t ButtonBaseSkin::getMargin() const
    {
        return m_StaticStyle == nullptr ? 0 : m_StaticStyle->margin;
    }

    void ButtonBaseSkin::applyStaticStyle()
    {
        if (m_StaticStyle != nullptr)
        {
            m_Button->setPadding(m_StaticStyle->padding);
            m_Button->setBorder(&m_StaticStyle->border);
        }
        else
        {
            m_Button->setPadding(Insets::EMPTY);
            m_Button->setBorder(nullptr);
        }
    }

    void ButtonBaseSkin::applyInteractionStyle(InteractionStyle::Type type)
    {
        const InteractionStyle* style = nullptr;
        switch (type)
        {
            case InteractionStyle::Type::IDLE:    { style = m_IdleStyle;    break; }
            case InteractionStyle::Type::HOVERED: { style = m_HoveredStyle; break; }
            case InteractionStyle::Type::PRESSED: { style = m_PressedStyle; break; }
        }

        if (style != nullptr)
        {
            m_Text.setColor(style->foreground);
            m_Button->setBackground(style->background);
        }
        else
        {
            m_Text.setColor(Sml::COLOR_BLACK);
            m_Button->setBackground(nullptr);
        }
    }

    //------------------------------------------------------------------------------
    // ButtonPlaneSkin
    //------------------------------------------------------------------------------
    ButtonPlaneSkin::ButtonPlaneSkin()
        : ButtonBaseSkin(nullptr, nullptr, nullptr, nullptr) {}

    ButtonPlaneSkin::ButtonPlaneSkin(Sgl::Button* button)
        : ButtonBaseSkin(button, nullptr, nullptr, nullptr, nullptr) {}

    //------------------------------------------------------------------------------
    // ButtonSkin
    //------------------------------------------------------------------------------
    const Insets                           ButtonSkin::PADDING                 = Insets{5, 10};
    const int32_t                          ButtonSkin::MARGIN                  = 5;
    const Border                           ButtonSkin::BORDER                  = Border{1, 0xE9'E9'E9'FF};
    const ButtonBaseSkin::StaticStyle      ButtonSkin::STATIC_STYLE            = {PADDING, MARGIN, BORDER}; 

    /* Idle */
    const Sml::Color                       ButtonSkin::IDLE_FOREGROUND         = Sml::COLOR_BLACK;
    const ColorFill                        ButtonSkin::IDLE_BACKGROUND_FILL    = {0xF5'F5'F5'FF};
    const Background                       ButtonSkin::IDLE_BACKGROUND         = {&IDLE_BACKGROUND_FILL};
    const ButtonBaseSkin::InteractionStyle ButtonSkin::IDLE_STYLE              = {IDLE_FOREGROUND,
                                                                                  &IDLE_BACKGROUND};

    /* Hovered */
    const Sml::Color                       ButtonSkin::HOVERED_FOREGROUND      = Sml::COLOR_WHITE;
    const ColorFill                        ButtonSkin::HOVERED_BACKGROUND_FILL = {0x25'92'FF'FF};
    const Background                       ButtonSkin::HOVERED_BACKGROUND      = {&HOVERED_BACKGROUND_FILL};
    const ButtonBaseSkin::InteractionStyle ButtonSkin::HOVERED_STYLE           = {HOVERED_FOREGROUND,
                                                                                  &HOVERED_BACKGROUND};

    ButtonSkin::ButtonSkin()
        : ButtonBaseSkin(&STATIC_STYLE, &IDLE_STYLE, &HOVERED_STYLE, nullptr) {}

    ButtonSkin::ButtonSkin(Sgl::Button* button)
        : ButtonBaseSkin(button, &STATIC_STYLE, &IDLE_STYLE, &HOVERED_STYLE, nullptr) {}

    //------------------------------------------------------------------------------
    // MenuItemSkin
    //------------------------------------------------------------------------------
    const Insets                           MenuItemSkin::PADDING                 = Insets{5, 10};
    const int32_t                          MenuItemSkin::MARGIN                  = 5;
    const Border                           MenuItemSkin::BORDER                  = Border{0, 0};
    const ButtonBaseSkin::StaticStyle      MenuItemSkin::STATIC_STYLE            = {PADDING, MARGIN, BORDER}; 

    /* Idle */
    const Sml::Color                       MenuItemSkin::IDLE_FOREGROUND         = Sml::COLOR_BLACK;
    const ColorFill                        MenuItemSkin::IDLE_BACKGROUND_FILL    = {Sml::COLOR_TRANSPARENT};
    const Background                       MenuItemSkin::IDLE_BACKGROUND         = {&IDLE_BACKGROUND_FILL};
    const ButtonBaseSkin::InteractionStyle MenuItemSkin::IDLE_STYLE              = {IDLE_FOREGROUND,
                                                                                    nullptr};

    /* Hovered */
    const Sml::Color                       MenuItemSkin::HOVERED_FOREGROUND      = 0xEF'F8'FF'FF;
    const ColorFill                        MenuItemSkin::HOVERED_BACKGROUND_FILL = {0x5A'B9'FF'FF};
    const Background                       MenuItemSkin::HOVERED_BACKGROUND      = {&HOVERED_BACKGROUND_FILL};
    const ButtonBaseSkin::InteractionStyle MenuItemSkin::HOVERED_STYLE           = {HOVERED_FOREGROUND,
                                                                                    &HOVERED_BACKGROUND};

    MenuItemSkin::MenuItemSkin()
        : ButtonBaseSkin(&STATIC_STYLE, &IDLE_STYLE, &HOVERED_STYLE, nullptr) {}

    MenuItemSkin::MenuItemSkin(Sgl::Button* button)
        : ButtonBaseSkin(button, &STATIC_STYLE, &IDLE_STYLE, &HOVERED_STYLE, nullptr) {}

    //------------------------------------------------------------------------------
    // ButtonIconSkin
    //------------------------------------------------------------------------------
}
}