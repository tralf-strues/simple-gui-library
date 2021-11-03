//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file component.h
//! @date 2021-10-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef COMPONENT_H
#define COMPONENT_H

#include "sml/sml_graphics_wrapper.h"
#include "sml/utils/id.h"
#include "../events/gui_event_dispatching.h"
#include "../style/skin.h"

class Component : public EventTarget
{
public:
    Component() {}

    Component(Renderer* renderer, int32_t width = 1, int32_t height = 1)
        : m_Renderer(renderer)
    {
        setSize(width, height);
    }

    virtual ~Component() = default;

    virtual Component* getHitComponent(int32_t x, int32_t y);
    virtual void updateGraphics();
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion);

    /* EventTarget */
    virtual EventDispatchChain* buildEventDispatchChain(EventDispatchChain* chain) override;

    /* Setters */
    void setRenderer(Renderer* renderer);
    void setId(const id128_t& id);

    void setParent(Component* parent);
    void setEventDispatcher(GuiEventDispatcher* dispatcher);
    void setVisible(bool visible);
    void setFocused(bool focused); // TODO: !!!implement!!!

    void setX(int32_t x);
    void setY(int32_t y);

    void setSize(int32_t width, int32_t height);
    void setSize(const Vec2<int32_t>& size);

    void setWidth(int32_t width);
    void setHeight(int32_t height);
    
    // void setMinSize(int32_t minWidth, int32_t minHeight);
    // void setMinSize(const Vec2<int32_t>& minSize);
    // void setMaxSize(int32_t maxWidth, int32_t maxHeight);
    // void setMaxSize(const Vec2<int32_t>& maxSize);
    // void setPrefSize(int32_t prefWidth, int32_t prefHeight);
    // void setPrefSize(const Vec2<int32_t>& prefSize);
    void setPrefWidth(int32_t width);
    void setPrefHeight(int32_t height);
    // void setMinSizeEnabled(bool enabled);
    // void setMaxSizeEnabled(bool enabled);
    void setPrefSizeEnabled(bool enabled);

    void setForeground(Color foreground);
    void setBackground(Color background);

    /* Getters */
    Renderer* getRenderer() const;
    Texture* getTexture() const;
    id128_t getId() const;

    Component* getParent() const;
    GuiEventDispatcher* getEventDispatcher();
    bool isVisible() const;
    bool isFocused() const;

    int32_t getX() const;
    int32_t getY() const;
    Vec2<int32_t> getPos() const;
    Rectangle<int32_t> getRegion() const;

    int32_t getWidth() const;
    int32_t getHeight() const;
    const Vec2<int32_t>& getSize() const;

    // const Vec2<int32_t>& getMinSize() const;
    // const Vec2<int32_t>& getMaxSize() const;
    virtual int32_t getPrefWidth() const;
    virtual int32_t getPrefHeight() const;
    // bool isMinSizeEnabled() const;
    // bool isMaxSizeEnabled() const;
    bool isPrefSizeEnabled() const;

    Color getForeground() const;
    Color getBackground() const;

protected:
    /* Context */
    Renderer*          m_Renderer        = nullptr;
    Texture*           m_Texture         = nullptr;
    id128_t            m_Id              = {};

    /* Scene */
    Component*         m_Parent          = nullptr;
    bool               m_IsVisible       = true;
    bool               m_IsFocused       = false;
    GuiEventDispatcher m_Dispatcher;

    /* Layout (in the parent's coordinate system) */
    int32_t            m_X               = 0;
    int32_t            m_Y               = 0;
    Vec2<int32_t>      m_Size            = {0, 0};
    
    // bool               m_MinSizeEnabled  = false;
    // Vec2<int32_t>      m_MinSize     = {0, 0};
    // bool               m_MaxSizeEnabled  = false;
    // Vec2<int32_t>      m_MaxSize     = {0, 0};
    bool               m_PrefSizeEnabled = false;
    Vec2<int32_t>      m_PrefSize        = {0, 0};

    /* Visuals */
    Color              m_Foreground      = COLOR_BLACK;
    Color              m_Background      = 0xFF'FF'FF'00;

    void updateTextureSize();
};

#endif // COMPONENT_H