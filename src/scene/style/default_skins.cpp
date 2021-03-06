/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file default_skins.cpp
 * @date 2021-11-08
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/style/default_skins.h"
#include "scene/containers/box_container.h"
#include "scene/controls/button.h"
#include "scene/controls/slider.h"
#include "scene/controls/scroll_bar.h"
#include "scene/controls/scroll_pane.h"

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
    ButtonBaseSkin::StaticStyle::StaticStyle(const Insets& paddingLabelOnly,
                                             const Insets& paddingIconOnly,
                                             const Insets& paddingIconAndLabel,
                                             int32_t margin,
                                             const Border& border)
        : paddingLabelOnly(paddingLabelOnly),
          paddingIconOnly(paddingIconOnly),
          paddingIconAndLabel(paddingIconAndLabel),
          margin(margin),
          border(border) {}

    ButtonBaseSkin::StaticStyle::StaticStyle(const Insets& padding, int32_t margin, const Border& border)
        : StaticStyle(padding, padding, padding, margin, border) {}

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
        if (m_Button->getBackground() != nullptr)
        {
            Background::fillArea(m_Button->getBackground(), m_Button->getOriginBounds());
        }

        if (m_Button->getBorder() != nullptr)
        {
            Border::encloseArea(m_Button->getBorder(), m_Button->getOriginBounds());
        }
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
        Sml::Rectangle<int32_t> contentArea = m_Button->getContentArea();

        m_Text.setString(m_Button->getLabel());
        m_Icon.setImage(m_Button->getIcon());
        applyStaticStyle(); // FIXME: move somewhere else, no need to update this each layout pass (or not?..)

        m_Text.setLayoutWidth(m_Text.computePrefWidth());
        m_Text.setLayoutHeight(m_Text.computePrefHeight());

        int32_t iconHeight = std::min(m_Icon.computePrefHeight(), contentArea.height);
        int32_t iconWidth  = m_Icon.computePrefWidth(iconHeight);
        m_Icon.setLayoutWidth(iconWidth);
        m_Icon.setLayoutHeight(iconHeight);

        int32_t centerY = contentArea.pos.y + contentArea.height / 2;

        m_Icon.setLayoutX(contentArea.pos.x);
        m_Icon.setLayoutY(centerY - m_Icon.getLayoutHeight() / 2);

        m_Text.setLayoutX(m_Icon.getLayoutX() + m_Icon.getLayoutWidth());
        if (m_Icon.getLayoutWidth() != 0)
        {
            m_Text.setLayoutX(m_Text.getLayoutX() + getMargin());
        }

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
            bool isLabelSet = m_Button->getLabel() != nullptr;
            bool isIconSet  = m_Button->getIcon()  != nullptr;

            if (isLabelSet && !isIconSet)
            {
                m_Button->setPadding(m_StaticStyle->paddingLabelOnly);
            }
            else if (!isLabelSet && isIconSet)
            {
                m_Button->setPadding(m_StaticStyle->paddingIconOnly);
            }
            else if (isLabelSet && isIconSet)
            {
                m_Button->setPadding(m_StaticStyle->paddingIconAndLabel);
            }
            
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
    const Insets                           ButtonSkin::PADDING_LABEL_ONLY      = Insets{5, 10};
    const Insets                           ButtonSkin::PADDING_ICON_ONLY       = Insets{5};
    const Insets                           ButtonSkin::PADDING_LABEL_AND_ICON  = Insets{5, 10};
    const int32_t                          ButtonSkin::MARGIN                  = 5;
    const Border                           ButtonSkin::BORDER                  = Border{1, 0xE9'E9'E9'FF};
    const ButtonBaseSkin::StaticStyle      ButtonSkin::STATIC_STYLE            = {PADDING_LABEL_ONLY,
                                                                                  PADDING_ICON_ONLY,
                                                                                  PADDING_LABEL_AND_ICON,
                                                                                  MARGIN,
                                                                                  BORDER}; 

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
    // SliderSkin
    //------------------------------------------------------------------------------
    const ShadowSpecification SliderSkin::KNOB_SHADOW        = {{0, 0}, {1.1, 1.1}, 3, 0x00'00'00'88};
    const ColorFill           SliderSkin::NOT_SELECTED_FILL  = {0xE0'E0'E0'FF};
    const ColorFill           SliderSkin::SELECTED_FILL      = {0x32'73'F6'FF};
    const Sml::Color          SliderSkin::KNOB_COLOR         = 0x27'5B'E1'FF;
    const int32_t             SliderSkin::THICKNESS          = 6;
    const int32_t             SliderSkin::KNOB_SIZE_ALONG    = 6;
    const int32_t             SliderSkin::KNOB_SIZE_ACROSS   = 11;

    class SliderSkinDragListener : public DragListener<Slider>
    {
    public:
        SliderSkinDragListener(Slider* slider) : DragListener<Slider>(slider) {}

        void updateValue(const Sml::Vec2i& mousePos)
        {
            Slider&     slider = *getComponent();
            SliderSkin& skin   = dynamic_cast<SliderSkin&>(*slider.getSkin());

            Sml::Vec2i localPos = slider.computeSceneToLocalPos(mousePos);

            int32_t posAlong   = slider.getOrientation() == Orientation::HORIZONTAL ? localPos.x : localPos.y;
            int32_t lineLength = slider.getOrientation() == Orientation::HORIZONTAL ? skin.getLineRect().width :
                                                                                      skin.getLineRect().height;

            slider.setValue(slider.getRangeMin() +
                            (static_cast<float>(posAlong - SliderSkin::KNOB_SIZE_ALONG / 2) /
                             static_cast<float>(lineLength - SliderSkin::KNOB_SIZE_ALONG)) *
                            (slider.getRangeMax() - slider.getRangeMin()));
        }

        virtual void onDragStart(DragStartEvent* event)
        {
            LOG_LIB_INFO("SliderSkinDragListener::onDragStart() called");

            updateValue(Sml::Vec2i(event->getX(), event->getY()));
        }

        virtual void onDragMove(DragMoveEvent* event)
        {
            LOG_LIB_INFO("SliderSkinDragListener::onDragMove() called");

            updateValue(Sml::Vec2i(event->getX(), event->getY()));
        }
    };

    class SliderSkinMousePressListener : public Sgl::ComponentEventListener<Slider>
    {
    public:
        DEFINE_STATIC_LISTENED_EVENT_TYPES(Sml::MouseButtonPressedEvent::getStaticType())

    public:
        SliderSkinMousePressListener(Slider* slider) : Sgl::ComponentEventListener<Slider>(slider) {}

        virtual void onEvent(Sml::Event* event) override
        {
            LOG_LIB_INFO("SliderSkinMousePressListener called");
            getComponent()->requestDrag();
        }
    };
    
    SliderSkin::SliderSkin(const Fill* notSelectedFill,
                           const Fill* selectedFill,
                           Sml::Color knobColor,
                           int32_t thickness,
                           int32_t knobSizeAlong,
                           int32_t knobSizeAcross)
        : m_NotSelectedFill(notSelectedFill),
          m_SelectedFill(selectedFill),
          m_Thickness(thickness),
          m_KnobSizeAlong(knobSizeAlong),
          m_KnobSizeAcross(knobSizeAcross),
          m_KnobRect(new Sgl::Rectangle())
    {
        m_KnobRect->setFillColor(knobColor);
        m_KnobRect->setShadow(&KNOB_SHADOW);
    }

    SliderSkin::SliderSkin(Slider* slider) : SliderSkin(&NOT_SELECTED_FILL, &SELECTED_FILL)
    {
        assert(slider);
        attach(slider);
    }

    void SliderSkin::dispose()
    {
        m_Slider->removeChild(m_KnobRect);
        delete m_KnobRect;

        m_Slider->getEventDispatcher()->detachHandler(m_MousePressListener);
        delete m_MousePressListener;

        m_Slider->getEventDispatcher()->detachHandler(m_DragListener);
        delete m_DragListener;
    }

    void SliderSkin::attach(Slider* slider)
    {
        assert(slider);

        m_Slider = slider;
        m_Slider->addChild(m_KnobRect);

        m_MousePressListener = new SliderSkinMousePressListener(m_Slider);
        m_Slider->getEventDispatcher()->attachHandler(SliderSkinMousePressListener::EVENT_TYPES, m_MousePressListener);

        m_DragListener = new SliderSkinDragListener(m_Slider);
        m_Slider->getEventDispatcher()->attachHandler(SliderSkinDragListener::EVENT_TYPES, m_DragListener);
    }

    Component* SliderSkin::getHitComponent(int32_t x, int32_t y)
    {
        Sml::Rectangle<int32_t> translatedRect = getLineRect();
        translatedRect.pos += m_Slider->getLayoutPos();

        if (Sml::isPointInsideRectangle({x, y}, translatedRect))
        {
            return m_Slider;
        }

        translatedRect = getKnobRect();
        translatedRect.pos += m_Slider->getLayoutPos();
        if (Sml::isPointInsideRectangle({x, y}, translatedRect))
        {
            return m_Slider;
        }

        return nullptr;
    }

    void SliderSkin::prerenderControl()
    {
        Sml::Rectangle<int32_t> lineRect = getLineRect();

        if (m_NotSelectedFill != nullptr)
        {
            m_NotSelectedFill->fillArea(lineRect, m_Slider->getOriginBounds());
        }

        if (m_Slider->getOrientation() == Orientation::HORIZONTAL)
        {
            lineRect.width *= getPercentage();
        }
        else
        {
            lineRect.height *= getPercentage();
        }

        if (m_SelectedFill != nullptr)
        {
            m_SelectedFill->fillArea(lineRect, m_Slider->getOriginBounds());
        }
    }

    const Control* SliderSkin::getControl() const { return m_Slider; }
    Control* SliderSkin::getModifiableControl()   { return m_Slider; }

    int32_t SliderSkin::computePrefWidth(int32_t height) const
    {
        if (m_Slider->getOrientation() == Orientation::HORIZONTAL)
        {
            return m_KnobSizeAlong;
        }

        return m_KnobSizeAcross;
    }

    int32_t SliderSkin::computePrefHeight(int32_t width) const
    {
        if (m_Slider->getOrientation() == Orientation::VERTICAL)
        {
            return m_KnobSizeAlong;
        }

        return m_KnobSizeAcross;
    }

    void SliderSkin::layoutChildren()
    {
        Sml::Rectangle<int32_t> knobRect = getKnobRect();

        m_KnobRect->setLayoutWidth(knobRect.width);
        m_KnobRect->setLayoutHeight(knobRect.height);
        m_KnobRect->setLayoutX(knobRect.pos.x);
        m_KnobRect->setLayoutY(knobRect.pos.y);
    }

    int32_t SliderSkin::getThickness() const { return m_Thickness; }
    void SliderSkin::setThickness(int32_t thickness) { m_Thickness = thickness; }

    int32_t SliderSkin::getKnobSizeAlong() const { return m_KnobSizeAlong; }
    void SliderSkin::setKnobSizeAlong(int32_t knobSizeAlong) { m_KnobSizeAlong = knobSizeAlong; }

    int32_t SliderSkin::getKnobSizeAcross() const { return m_KnobSizeAcross; }
    void SliderSkin::setKnobSizeAcross(int32_t knobSizeAcross) { m_KnobSizeAcross = knobSizeAcross; }

    void SliderSkin::setKnobShadow(const ShadowSpecification* shadow) { m_KnobRect->setShadow(shadow); }

    Sml::Rectangle<int32_t> SliderSkin::getLineRect()
    {
        if (m_Slider->getOrientation() == Orientation::HORIZONTAL)
        {
            Sml::Rectangle<int32_t> originBounds = m_Slider->getOriginBounds();
            int32_t                 centerY      = originBounds.height / 2;

            return Sml::Rectangle<int32_t>{0, centerY - m_Thickness / 2, originBounds.width, m_Thickness};
        }
        else
        {
            Sml::Rectangle<int32_t> originBounds = m_Slider->getOriginBounds();
            int32_t                 centerX      = originBounds.width / 2;

            return Sml::Rectangle<int32_t>{centerX - m_Thickness / 2, 0, m_Thickness, originBounds.height};
        }
    }

    Sml::Rectangle<int32_t> SliderSkin::getKnobRect()
    {
        if (m_Slider->getOrientation() == Orientation::HORIZONTAL)
        {
            Sml::Rectangle<int32_t> originBounds = m_Slider->getOriginBounds();
            int32_t                 centerY      = originBounds.height / 2;
            int32_t                 knobCenterX  = m_KnobSizeAlong / 2 + getPercentage() * (originBounds.width - m_KnobSizeAlong);

            return Sml::Rectangle<int32_t>{knobCenterX - m_KnobSizeAlong / 2, centerY - m_KnobSizeAcross / 2,
                                           m_KnobSizeAlong, m_KnobSizeAcross};
        }
        else
        {
            Sml::Rectangle<int32_t> originBounds = m_Slider->getOriginBounds();
            int32_t                 centerX      = originBounds.width / 2;
            int32_t                 knobCenterY  = m_KnobSizeAlong / 2 + getPercentage() * (originBounds.height - m_KnobSizeAlong);

            return Sml::Rectangle<int32_t>{centerX - m_KnobSizeAcross / 2, knobCenterY - m_KnobSizeAlong / 2,
                                           m_KnobSizeAcross, m_KnobSizeAlong};
        }
    }

    float SliderSkin::getPercentage()
    {
        return (m_Slider->getValue() - m_Slider->getRangeMin()) / (m_Slider->getRangeMax() - m_Slider->getRangeMin());
    }

    //------------------------------------------------------------------------------
    // ScrollBarSkin
    //------------------------------------------------------------------------------
    const Sml::Color          ScrollBarSkin::KNOB_COLOR  = 0xF5'F5'F5'FF;
    // const ShadowSpecification ScrollBarSkin::KNOB_SHADOW = {{0, 0}, {1.07, 1.07}, 3, 0xCC'CC'CC'88};

    ScrollBarSkin::ScrollBarSkin(ScrollBar* scrollBar)
        : m_Box(new BoxContainer()),
          m_SliderSkin(new SliderSkin(&SliderSkin::NOT_SELECTED_FILL, nullptr, KNOB_COLOR)),
          m_Slider(new Slider(m_SliderSkin, 0, 1)),
          m_DecrementButton(new Button(new ButtonPlaneSkin())),
          m_IncrementButton(new Button(new ButtonPlaneSkin()))
    {
        assert(scrollBar);

        m_Box->addChildren(m_DecrementButton, m_Slider, m_IncrementButton);
        m_Box->setGrowPriority(m_Slider, BoxContainer::GrowPriority::ALWAYS);
        m_Box->setFillAcross(true);

        m_SliderSkin->setKnobShadow(nullptr);

        attach(scrollBar);
    }

    void ScrollBarSkin::dispose()
    {
        m_ScrollBar->removeChild(m_Box);
        delete m_Box;
    }

    void ScrollBarSkin::attach(ScrollBar* scrollBar)
    {
        assert(scrollBar);
        m_ScrollBar = scrollBar;

        if (m_ScrollBar->getOrientation() == Orientation::HORIZONTAL)
        {
            m_Box->setDirection(BoxContainer::Direction::LEFT_TO_RIGHT);

            m_DecrementButton->setIcon(new Image("res/sgl/arrow_left.png", ImageFormat::PNG));
            m_IncrementButton->setIcon(new Image("res/sgl/arrow_right.png", ImageFormat::PNG));
        }
        else
        {
            m_Box->setDirection(BoxContainer::Direction::TOP_TO_BOTTOM);

            m_DecrementButton->setIcon(new Image("res/sgl/arrow_up.png", ImageFormat::PNG));
            m_IncrementButton->setIcon(new Image("res/sgl/arrow_down.png", ImageFormat::PNG));
        }

        m_ScrollBar->addChild(m_Box);

        class ChangeValueButtonListener : public ActionListener<Button>
        {
        public:
            ChangeValueButtonListener(Button* button, ScrollBar* scrollBar, float multiplier)
                : ActionListener<Button>(button), m_ScrollBar(scrollBar), m_Multiplier(multiplier) {}

            virtual void onAction(ActionEvent* event) override
            {
                m_ScrollBar->setValue(m_ScrollBar->getValue() + m_ScrollBar->getIncrement() * m_Multiplier);
            }

        private:
            ScrollBar* m_ScrollBar  = nullptr;
            float      m_Multiplier = 0;
        };

        m_DecrementButton->setOnAction(new ChangeValueButtonListener(m_DecrementButton, m_ScrollBar, -1));
        m_IncrementButton->setOnAction(new ChangeValueButtonListener(m_IncrementButton, m_ScrollBar,  1));

        class SliderPropertyChangeListener : public Sml::PropertyChangeListener<float>
        {
        public:
            SliderPropertyChangeListener(ScrollBar* scrollBar) : m_ScrollBar(scrollBar) {}

            virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
            {
                m_ScrollBar->setValue(event->getNewValue());
            }

        private:
            ScrollBar* m_ScrollBar = nullptr;
        };

        m_Slider->addOnPropertyChange(new SliderPropertyChangeListener(m_ScrollBar));
    }

    const Control* ScrollBarSkin::getControl() const { return m_ScrollBar; }
    Control* ScrollBarSkin::getModifiableControl() { return m_ScrollBar; }

    void ScrollBarSkin::layoutChildren()
    {
        int32_t layoutWidth  = m_ScrollBar->getLayoutWidth();
        int32_t layoutHeight = m_ScrollBar->getLayoutHeight();

        m_Box->setLayoutX(0);
        m_Box->setLayoutY(0);
        m_Box->setLayoutWidth(layoutWidth);
        m_Box->setLayoutHeight(layoutHeight);

        m_Slider->setRangeMin(m_ScrollBar->getRangeMin());
        m_Slider->setRangeMax(m_ScrollBar->getRangeMax());
        m_Slider->setValue(m_ScrollBar->getValue());
        m_Slider->setOrientation(m_ScrollBar->getOrientation());

        if (m_ScrollBar->getOrientation() == Orientation::HORIZONTAL)
        {
            m_SliderSkin->setThickness(layoutHeight);
            m_SliderSkin->setKnobSizeAcross(layoutHeight - 4);
            m_SliderSkin->setKnobSizeAlong(m_Slider->getLayoutWidth() * m_ScrollBar->getVisibleRange() /
                                           (m_ScrollBar->getRangeMax() - m_ScrollBar->getRangeMin()));
        }
        else
        {
            m_SliderSkin->setThickness(layoutWidth);
            m_SliderSkin->setKnobSizeAcross(layoutWidth - 4);
            m_SliderSkin->setKnobSizeAlong(m_Slider->getLayoutHeight() * m_ScrollBar->getVisibleRange() /
                                           (m_ScrollBar->getRangeMax() - m_ScrollBar->getRangeMin()));
        }
    }

    //------------------------------------------------------------------------------
    // ScrollPaneSkin
    //------------------------------------------------------------------------------
    class HorizontalScrollListener : public Sml::PropertyChangeListener<float>
    {
    public:
        HorizontalScrollListener(ScrollPaneSkin* skin) : m_Skin(skin) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            ScrollPane* scrollPane = dynamic_cast<ScrollPane*>(m_Skin->getModifiableControl());

            if (scrollPane->getContent() != nullptr)
            {
                // int32_t delta = (scrollPane->getContent()->getLayoutWidth() - scrollPane->getViewportWidth()) *
                                // (event->getNewValue() - event->getOldValue());

                // scrollPane->setViewportWidth(scrollPane->getViewportWidth() + delta);
                scrollPane->setViewportX(event->getNewValue() * (scrollPane->getContent()->getLayoutWidth() - scrollPane->getViewportWidth()));
            }
        }

    private:
        ScrollPaneSkin* m_Skin = nullptr;
    };

    class VerticalScrollListener : public Sml::PropertyChangeListener<float>
    {
    public:
        VerticalScrollListener(ScrollPaneSkin* skin) : m_Skin(skin) {}

        virtual void onPropertyChange(Sml::PropertyChangeEvent<float>* event) override
        {
            ScrollPane* scrollPane = dynamic_cast<ScrollPane*>(m_Skin->getModifiableControl());

            if (scrollPane->getContent() != nullptr)
            {
                // int32_t delta = (scrollPane->getContent()->getLayoutHeight() - scrollPane->getViewportHeight()) *
                //                 (event->getNewValue() - event->getOldValue());

                // // scrollPane->setViewportHeight(scrollPane->getViewportHeight() + delta);
                // scrollPane->setViewportY(scrollPane->getViewportY() + delta);
                scrollPane->setViewportY(event->getNewValue() * (scrollPane->getContent()->getLayoutHeight() - scrollPane->getViewportHeight()));
            }
        }

    private:
        ScrollPaneSkin* m_Skin = nullptr;
    };

    ScrollPaneSkin::ScrollPaneSkin(ScrollPane* scrollPane)
        : m_HorizontalScrollBar(new Sgl::ScrollBar(Orientation::HORIZONTAL, 0, 1)),
          m_VerticalScrollBar(new Sgl::ScrollBar(Orientation::VERTICAL, 0, 1))
    {
        assert(scrollPane);

        m_HorizontalScrollBar->setIncrement(0.1);
        m_VerticalScrollBar->setIncrement(0.1);

        attach(scrollPane);
    }

    void ScrollPaneSkin::dispose()
    {
        m_ScrollPane->removeChild(m_ScrollPane->getContent());
    }

    void ScrollPaneSkin::attach(ScrollPane* scrollPane)
    {
        assert(scrollPane);
        m_ScrollPane = scrollPane;

        m_ScrollPane->addChildren(m_HorizontalScrollBar, m_VerticalScrollBar);

        m_HorizontalScrollBar->setOnPropertyChange(new HorizontalScrollListener(this));
        m_VerticalScrollBar->setOnPropertyChange(new VerticalScrollListener(this));
    }

    Component* ScrollPaneSkin::getHitComponent(int32_t x, int32_t y)
    {
        Sml::Vec2i posInPane = Sml::Vec2i(x, y) - m_ScrollPane->getLayoutPos();

        if (m_ScrollPane->getContent() != nullptr && Sml::isPointInsideRectangle(posInPane, computeContentRegion()))
        {
            return m_ScrollPane->getContent()->getHitComponent(x, y);
        }

        if (Sml::isPointInsideRectangle(posInPane, m_HorizontalScrollBar->getLayoutBounds()))
        {
            const Sml::Vec2i& layoutPos = m_HorizontalScrollBar->getLayoutPos();
            return m_HorizontalScrollBar->getHitComponent(layoutPos.x + x, layoutPos.y + y);
        }

        if (Sml::isPointInsideRectangle(posInPane, m_VerticalScrollBar->getLayoutBounds()))
        {
            const Sml::Vec2i& layoutPos = m_VerticalScrollBar->getLayoutPos();
            return m_VerticalScrollBar->getHitComponent(layoutPos.x + x, layoutPos.y + y);
        }

        if (Sml::isPointInsideRectangle(posInPane, m_ScrollPane->getLayoutBounds()))
        {
            return m_ScrollPane;
        }

        return nullptr;
    }

    void ScrollPaneSkin::prerenderControl()
    {
        if (m_ScrollPane->getContent() == nullptr)
        {
            return;
        }

        updateRenderedContentTexture();

        if (m_RenderedContentTexture != nullptr)
        {   
            Sml::Rectangle<int32_t> snapshotRegion = computeContentRegion();
            m_RenderedContentTexture->copyTo(m_ScrollPane->getSnapshot(), &snapshotRegion, &m_ScrollPane->getViewport());
        }
    }

    const Control* ScrollPaneSkin::getControl() const { return m_ScrollPane; }
    Control* ScrollPaneSkin::getModifiableControl() { return m_ScrollPane; }

    int32_t ScrollPaneSkin::computePrefWidth(int32_t height) const
    {
        return m_VerticalScrollBar->computePrefWidth(height) +
               m_ScrollPane->getContent() != nullptr ? m_ScrollPane->getContent()->computePrefWidth(height) : 0;
    }

    int32_t ScrollPaneSkin::computePrefHeight(int32_t width) const
    {
        return m_HorizontalScrollBar->computePrefHeight(width) +
               m_ScrollPane->getContent() != nullptr ? m_ScrollPane->getContent()->computePrefHeight(width) : 0;
    }

    void ScrollPaneSkin::layoutChildren()
    {
        Sml::Rectangle<int32_t> contentArea = m_ScrollPane->getContentArea();

        int32_t scrollWidth  = m_VerticalScrollBar->computePrefWidth();
        int32_t scrollHeight = m_HorizontalScrollBar->computePrefHeight();

        m_ScrollPane->getContent()->setLayoutWidth(m_ScrollPane->getContent()->computePrefWidth());
        m_ScrollPane->getContent()->setLayoutHeight(m_ScrollPane->getContent()->computePrefHeight());

        m_VerticalScrollBar->setLayoutWidth(scrollWidth);
        m_VerticalScrollBar->setLayoutHeight(contentArea.height - scrollHeight);
        m_VerticalScrollBar->setLayoutX(contentArea.pos.x + contentArea.width - m_VerticalScrollBar->getLayoutWidth());
        m_VerticalScrollBar->setLayoutY(contentArea.pos.y);

        m_HorizontalScrollBar->setLayoutWidth(contentArea.width - scrollWidth);
        m_HorizontalScrollBar->setLayoutHeight(scrollHeight);
        m_HorizontalScrollBar->setLayoutX(contentArea.pos.x);
        m_HorizontalScrollBar->setLayoutY(contentArea.pos.y + contentArea.height -
                                          m_HorizontalScrollBar->getLayoutHeight());
        

        if (m_ScrollPane->getContent() != nullptr)
        {
            m_VerticalScrollBar->setVisibleRange(static_cast<float>(m_ScrollPane->getViewportHeight()) /
                                                 m_ScrollPane->getContent()->getLayoutHeight());
            m_HorizontalScrollBar->setVisibleRange(static_cast<float>(m_ScrollPane->getViewportWidth()) /
                                                 m_ScrollPane->getContent()->getLayoutWidth());

            // m_ScrollPane->getContent()->setScene(m_ScrollPane->getScene());
            m_ScrollPane->getContent()->setInteractable(false);
            m_ScrollPane->getContent()->setVisible(false);
            // FIXME: COSTYL!!!
            m_ScrollPane->removeChildren();
            m_ScrollPane->addChild(m_ScrollPane->getContent());
            m_ScrollPane->addChild(m_HorizontalScrollBar);
            m_ScrollPane->addChild(m_VerticalScrollBar);
        }
        else
        {
            m_VerticalScrollBar->setVisibleRange(1);
            m_HorizontalScrollBar->setVisibleRange(1);
        }

        if (m_PrevContent != m_ScrollPane->getContent())
        {
            m_PrevContent = m_ScrollPane->getContent();

            if (m_PrevContent != nullptr)
            {
                m_ScrollPane->setViewport({0, 0, computeContentRegion().width, computeContentRegion().height});
            }
        }
    }

    Sml::Rectangle<int32_t> ScrollPaneSkin::computeContentRegion() const
    {
        Sml::Rectangle<int32_t> contentArea = m_ScrollPane->getContentArea();

        return {contentArea.pos, contentArea.width - m_VerticalScrollBar->getLayoutWidth(),
                                 contentArea.height - m_HorizontalScrollBar->getLayoutHeight()};
    }

    void ScrollPaneSkin::updateRenderedContentTexture()
    {
        Component* content = m_ScrollPane->getContent();
        int32_t contentWidth = content->getLayoutWidth();
        int32_t contentHeight = content->getLayoutHeight();

        if (content == nullptr || contentWidth == 0 || contentHeight == 0)
        {
            return;
        }

        if (m_RenderedContentTexture == nullptr ||
            m_RenderedContentTexture->getWidth() != contentWidth ||
            m_RenderedContentTexture->getHeight() != contentHeight)
        {
            if (m_RenderedContentTexture == nullptr)
            {
                delete m_RenderedContentTexture;
            }

            m_RenderedContentTexture = new Sml::Texture(contentWidth, contentHeight);
        }

        Sml::Renderer& renderer = Sml::Renderer::getInstance();
        renderer.pushSetTarget(m_RenderedContentTexture);

        content->render({0, 0, contentWidth, contentHeight});

        renderer.popTarget();
    }
}
}