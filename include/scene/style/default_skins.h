// //------------------------------------------------------------------------------
// //! @author Nikita Mochalov (github.com/tralf-strues)
// //! @file default_skins.h
// //! @date 2021-10-30
// //! 
// //! @copyright Copyright (c) 2021
// //------------------------------------------------------------------------------

// #ifndef DEFAULT_SKIN_H
// #define DEFAULT_SKIN_H

// #include "sml/events/system_events.h"
// #include "sml/events/event_dispatching.h"
// #include "sml/events/listener_notifier.h"
// #include "skin.h"

// struct BorderedRect
// {
//     bool   backgroundEnabled;
//     Color  background;

//     bool   borderEnabled;
//     Color  borderColor;
//     int8_t borderThickness;
// };

// //------------------------------------------------------------------------------
// // ButtonSkin
// //------------------------------------------------------------------------------
// static const BorderedRect  DEFAULT_BUTTON_BACKGROUND         = BorderedRect{true, 0xF5'F5'F5'FF, true, 0xE9'E9'E9'FF, 1};
// static const Color         DEFAULT_BUTTON_FOREGROUND         = COLOR_BLACK;
// static const BorderedRect  DEFAULT_BUTTON_BACKGROUND_HOVERED = BorderedRect{true, 0x25'92'FF'FF, true, 0xE9'E9'E9'FF, 1};
// static const Color         DEFAULT_BUTTON_FOREGROUND_HOVERED = COLOR_WHITE;
// static const Vec2<int32_t> DEFAULT_BUTTON_PREF_MARGINS       = {8, 0};
// static const Vec2<int32_t> DEFAULT_BUTTON_PREF_SIZE          = {64, 16};

// class Button;

// class ButtonSkinListener : public Listener
// {
// public:
//     Button* button;

//     ButtonSkinListener(Button* button);
//     virtual void onEvent(Event* event) override;
// };

// class ButtonSkin : public Skin<Button>
// {
// public:
//     ButtonSkin(Button* button);

//     virtual void apply()   override;
//     virtual void dispose() override;

//     virtual int32_t computePrefWidth()  const override;
//     virtual int32_t computePrefHeight() const override;
//     virtual void    layoutChildren()    const override;

// private:
//     ButtonSkinListener* m_Listener;

//     BorderedRect        m_Background = DEFAULT_BUTTON_BACKGROUND;
//     Color               m_Foreground = DEFAULT_BUTTON_FOREGROUND;
// };

// // struct HoverStyle
// // {
// //     const Skin* skinNormal        = nullptr;
// //     const Skin* skinHovered       = nullptr;
// //     Color       foregroundNormal  = COLOR_BLACK;
// //     Color       foregroundHovered = COLOR_BLACK;

// //     HoverStyle(const Skin* skinNormal, const Skin* skinHovered,
// //                Color foregroundNormal, Color foregroundHovered)
// //         : skinNormal(skinNormal),
// //           skinHovered(skinHovered),
// //           foregroundNormal(foregroundNormal),
// //           foregroundHovered(foregroundHovered)
// //     {
// //     }
// // };

// // static const BasicSkin  BLANK_BASIC_SKIN                  = BasicSkin{false, 0, false, 0, 0};

// // static const BasicSkin  DEFAULT_SKIN_BUTTON               = BasicSkin{true, 0xF5'F5'F5'FF, true, 0xE9'E9'E9'FF, 1};
// // static const BasicSkin  DEFAULT_SKIN_BUTTON_HOVERED       = BasicSkin{true, 0x25'92'FF'FF, true, 0xE9'E9'E9'FF, 1};
// // static const HoverStyle DEFAULT_HOVER_STYLE_BUTTON        = {&DEFAULT_SKIN_BUTTON, &DEFAULT_SKIN_BUTTON_HOVERED,
// //                                                              COLOR_BLACK, COLOR_WHITE};

// // static const BasicSkin  DEFAULT_SKIN_MENUBAR              = BasicSkin{true, 0xF5'F5'F5'FF, true, 0xE9'E9'E9'FF, 1};

// // static const BasicSkin  DEFAULT_SKIN_LIST_MENU            = DEFAULT_SKIN_MENUBAR;

// // static const BasicSkin  DEFAULT_SKIN_MENU_ITEM            = BasicSkin{true, COLOR_TRANSPARENT, false, 0, 0};
// // static const BasicSkin  DEFAULT_SKIN_MENU_ITEM_HOVERED    = BasicSkin{true, 0x59'B9'FF'FF, false, 0, 0};
// // static const HoverStyle DEFAULT_HOVER_STYLE_MENU_ITEM     = {&DEFAULT_SKIN_MENU_ITEM, &DEFAULT_SKIN_MENU_ITEM_HOVERED,
// //                                                              COLOR_BLACK, COLOR_WHITE};

// static const int32_t    DEFAULT_LIST_MENU_MIN_WIDTH       = 128;
// static const int32_t    DEFAULT_LIST_MENU_INDENT          = 4;
// static const int32_t    DEFAULT_LIST_MENU_SPACING         = 2;

// // class Component;

// // class HoverListener : public Listener
// // {
// // public:
// //     HoverListener(Component* component, const HoverStyle* hoverStyle)
// //         : m_Component(component), m_HoverStyle(hoverStyle) {}

// //     virtual void onEvent(Event* event) override;

// // private:
// //     Component*        m_Component  = nullptr;
// //     const HoverStyle* m_HoverStyle = nullptr;
// // };

// #endif // DEFAULT_SKIN_H