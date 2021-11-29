/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file default_skins.h
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/events/listener_notifier.h"
#include "../shapes/text.h"
#include "../media/image_view.h"
#include "../background.h"
#include "../border.h"
#include "../insets.h"
#include "skin.h"
#include "shadow.h"

namespace Sgl
{
    class Button;
    class Slider;

namespace DefaultSkins
{
    extern Sml::Font* g_DefaultFont;

    //------------------------------------------------------------------------------
    // ButtonBaseSkin
    //------------------------------------------------------------------------------
    class ButtonBaseSkinEventListener;

    class ButtonBaseSkin : public Sgl::BaseSkin<Button>
    {
    public:
        struct StaticStyle
        {
            Insets  padding{0};
            int32_t margin = 0;
            Border  border{0, Sml::COLOR_BLACK};
        };

        struct InteractionStyle
        {
            enum class Type
            {
                IDLE,
                HOVERED,
                PRESSED
            };

            Sml::Color        foreground;
            const Background* background;
        };

    public:
        ButtonBaseSkin(Sgl::Button* button,
                       const StaticStyle* staticStyle,
                       const InteractionStyle* idleStyle,
                       const InteractionStyle* hoveredStyle,
                       const InteractionStyle* pressedStyle);

        ButtonBaseSkin(const StaticStyle* staticStyle,
                       const InteractionStyle* idleStyle,
                       const InteractionStyle* hoveredStyle,
                       const InteractionStyle* pressedStyle);

        virtual void dispose() override;
        virtual void attach(Button* button) override;

        virtual void prerenderControl() override;

        virtual const Control* getControl() const override;
        virtual Control* getModifiableControl() override;

        virtual int32_t computePrefHeight(int32_t width = -1) const override;
        virtual int32_t computePrefWidth(int32_t height = -1) const override;

        virtual void layoutChildren() override;

        void applyInteractionStyle(InteractionStyle::Type type);

    protected:
        const StaticStyle*           m_StaticStyle  = nullptr;

        const InteractionStyle*      m_IdleStyle    = nullptr;
        const InteractionStyle*      m_HoveredStyle = nullptr;
        const InteractionStyle*      m_PressedStyle = nullptr;

        Sgl::Button*                 m_Button       = nullptr;
        ButtonBaseSkinEventListener* m_Handler      = nullptr;

        Sgl::Text                    m_Text;
        Sgl::ImageView               m_Icon;

        int32_t getMargin() const;

        void applyStaticStyle();
    };

    //------------------------------------------------------------------------------
    // ButtonPlaneSkin
    //------------------------------------------------------------------------------
    class ButtonPlaneSkin : public ButtonBaseSkin
    {
    public:
        ButtonPlaneSkin();
        ButtonPlaneSkin(Sgl::Button* button);
    };

    //------------------------------------------------------------------------------
    // ButtonSkin
    //------------------------------------------------------------------------------
    class ButtonSkin : public ButtonBaseSkin
    {
    public:
        static const Insets           PADDING;
        static const int32_t          MARGIN;
        static const Border           BORDER;
        static const StaticStyle      STATIC_STYLE; 

        static const Sml::Color       IDLE_FOREGROUND;
        static const ColorFill        IDLE_BACKGROUND_FILL;
        static const Background       IDLE_BACKGROUND;
        static const InteractionStyle IDLE_STYLE;

        static const Sml::Color       HOVERED_FOREGROUND;
        static const ColorFill        HOVERED_BACKGROUND_FILL;
        static const Background       HOVERED_BACKGROUND;
        static const InteractionStyle HOVERED_STYLE;
    
    public:
        ButtonSkin();
        ButtonSkin(Sgl::Button* button);
    };

    //------------------------------------------------------------------------------
    // MenuItemSkin
    //------------------------------------------------------------------------------
    class MenuItemSkin : public ButtonBaseSkin
    {
    public:
        static const Insets           PADDING;
        static const int32_t          MARGIN;
        static const Border           BORDER;
        static const StaticStyle      STATIC_STYLE; 

        static const Sml::Color       IDLE_FOREGROUND;
        static const ColorFill        IDLE_BACKGROUND_FILL;
        static const Background       IDLE_BACKGROUND;
        static const InteractionStyle IDLE_STYLE;

        static const Sml::Color       HOVERED_FOREGROUND;
        static const ColorFill        HOVERED_BACKGROUND_FILL;
        static const Background       HOVERED_BACKGROUND;
        static const InteractionStyle HOVERED_STYLE;
    
    public:
        MenuItemSkin();
        MenuItemSkin(Sgl::Button* button);
    };

    //------------------------------------------------------------------------------
    // SliderSkin
    //------------------------------------------------------------------------------
    class SliderSkinDragListener;
    class SliderSkinMousePressListener;

    class SliderSkin : public Sgl::BaseSkin<Slider>
    {
    public:
        static const Shadow     SHADOW;
        static const Sml::Color NOT_SELECTED_COLOR;
        static const Sml::Color SELECTED_COLOR;
        static const Sml::Color KNOB_COLOR;
        static const int32_t    THICKNESS;
        static const int32_t    KNOB_WIDTH;
        static const int32_t    KNOB_HEIGHT;

    public:
        SliderSkin(Slider* slider);

        virtual void dispose() override;
        virtual void attach(Slider* slider) override;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void prerenderControl() override;

        virtual const Control* getControl() const override;
        virtual Control* getModifiableControl() override;

        virtual int32_t computePrefHeight(int32_t width = -1) const override;

        virtual void layoutChildren() override;

        Sml::Rectangle<int32_t> getLineRect();
        Sml::Rectangle<int32_t> getKnobRect();
        float                   getPercentage();

    private:
        Slider*                       m_Slider             = nullptr;
        SliderSkinDragListener*       m_DragListener       = nullptr;
        SliderSkinMousePressListener* m_MousePressListener = nullptr;
    };
}
}