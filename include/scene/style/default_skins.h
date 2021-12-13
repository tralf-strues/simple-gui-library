/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file default_skins.h
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/events/listener_notifier.h"
#include "../image_view.h"
#include "../shapes/text.h"
#include "../shapes/rectangle.h"
#include "../../paint/background.h"
#include "../../paint/border.h"
#include "../../paint/insets.h"
#include "../../paint/shadow.h"
#include "skin.h"

namespace Sgl
{
    class Button;
    class Slider;
    class ScrollBar;
    class BoxContainer;
    class ScrollPane;

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
            Insets  paddingLabelOnly{0};
            Insets  paddingIconOnly{0};
            Insets  paddingIconAndLabel{0};
            int32_t margin = 0;
            Border  border{0, Sml::COLOR_BLACK};

            StaticStyle(const Insets& padding, int32_t margin, const Border& border);

            StaticStyle(const Insets& paddingLabelOnly,
                        const Insets& paddingIconOnly,
                        const Insets& paddingIconAndLabel,
                        int32_t margin,
                        const Border& border);
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
        static const Insets           PADDING_LABEL_ONLY;
        static const Insets           PADDING_ICON_ONLY;
        static const Insets           PADDING_LABEL_AND_ICON;
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
        static const ShadowSpecification KNOB_SHADOW;
        static const ColorFill           NOT_SELECTED_FILL;
        static const ColorFill           SELECTED_FILL;
        static const Sml::Color          KNOB_COLOR;
        static const int32_t             THICKNESS;
        static const int32_t             KNOB_SIZE_ALONG;
        static const int32_t             KNOB_SIZE_ACROSS;

    public:
        SliderSkin(const Fill* notSelectedFill,
                   const Fill* selectedFill,
                   Sml::Color knobColor = KNOB_COLOR,
                   int32_t thickness = THICKNESS,
                   int32_t knobSizeAlong = KNOB_SIZE_ALONG,
                   int32_t knobSizeAcross = KNOB_SIZE_ACROSS);

        SliderSkin(Slider* slider);

        virtual void dispose() override;
        virtual void attach(Slider* slider) override;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;
        virtual void prerenderControl() override;

        virtual const Control* getControl() const override;
        virtual Control* getModifiableControl() override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override;
        virtual int32_t computePrefHeight(int32_t width = -1) const override;

        virtual void layoutChildren() override;

        int32_t getThickness() const;
        void setThickness(int32_t thickness);

        int32_t getKnobSizeAlong() const;
        void setKnobSizeAlong(int32_t knobWidth);

        int32_t getKnobSizeAcross() const;
        void setKnobSizeAcross(int32_t knobSizeAcross);

        void setKnobShadow(const ShadowSpecification* shadow);

        Sml::Rectangle<int32_t> getLineRect();
        Sml::Rectangle<int32_t> getKnobRect();
        float                   getPercentage();

    private:
        Slider*                       m_Slider             = nullptr;
        SliderSkinDragListener*       m_DragListener       = nullptr;
        SliderSkinMousePressListener* m_MousePressListener = nullptr;

        const Fill*                   m_NotSelectedFill    = nullptr;
        const Fill*                   m_SelectedFill       = nullptr;
        
        int32_t                       m_Thickness          = 0;
        int32_t                       m_KnobSizeAlong      = 0;
        int32_t                       m_KnobSizeAcross     = 0;

        Rectangle*                    m_KnobRect           = nullptr;
    };

    //------------------------------------------------------------------------------
    // ScrollBarSkin
    //------------------------------------------------------------------------------
    class ScrollBarSkin : public Sgl::BaseSkin<ScrollBar>
    {
    public:
        static const Sml::Color          KNOB_COLOR;
        static const ShadowSpecification KNOB_SHADOW;

    public:
        ScrollBarSkin(ScrollBar* scrollBar);

        virtual void dispose() override;
        virtual void attach(ScrollBar* scrollBar) override;

        virtual const Control* getControl() const override;
        virtual Control* getModifiableControl() override;

        virtual void layoutChildren() override;

    private:
        ScrollBar*    m_ScrollBar       = nullptr;

        BoxContainer* m_Box             = nullptr;
        SliderSkin*   m_SliderSkin      = nullptr;
        Slider*       m_Slider          = nullptr;
        Button*       m_DecrementButton = nullptr;
        Button*       m_IncrementButton = nullptr;
    };

    //------------------------------------------------------------------------------
    // ScrollPaneSkin
    //------------------------------------------------------------------------------
    class ScrollPaneSkin : public Sgl::BaseSkin<ScrollPane>
    {
    public:
        ScrollPaneSkin(ScrollPane* scrollPane);

        virtual void dispose() override;
        virtual void attach(ScrollPane* scrollPane) override;

        virtual Component* getHitComponent(int32_t x, int32_t y) override;

        virtual void prerenderControl() override;

        virtual const Control* getControl() const override;
        virtual Control* getModifiableControl() override;

        virtual int32_t computePrefWidth(int32_t height = -1) const override;
        virtual int32_t computePrefHeight(int32_t width = -1) const override;

        virtual void layoutChildren() override;
        
    private:
        ScrollPane*   m_ScrollPane             = nullptr;
        Component*    m_PrevContent            = nullptr;

        ScrollBar*    m_HorizontalScrollBar    = nullptr;
        ScrollBar*    m_VerticalScrollBar      = nullptr;

        Sml::Texture* m_RenderedContentTexture = nullptr;

        // Sml::PropertyChangeListener<float>* m_HorizontalScrollListener = nullptr;
        // Sml::PropertyChangeListener<float>* m_VerticalScrollListener   = nullptr;

        Sml::Rectangle<int32_t> computeContentRegion() const;
        void updateRenderedContentTexture();
    };
}
}