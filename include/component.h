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
#include "events/gui_event_dispatching.h"

struct Dimension
{
    Dimension(int32_t width = 0, int32_t height = 0) : width(width), height(height) {}

    int32_t width;
    int32_t height;
};

class Component : public EventTarget
{
public:
    Component() = default;

    Component(Renderer* renderer, int32_t width = 32, int32_t height = 32)
        : m_Renderer(renderer)
    {
        setSize(width, height);
    }

    virtual Component* getHitComponent(int32_t x, int32_t y);
    virtual void updateGraphics() = 0;
    void render(Texture* target);

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
    
    void setMinSize(int32_t minWidth, int32_t minHeight);
    void setMaxSize(int32_t maxWidth, int32_t maxHeight);
    void setPrefSize(int32_t prefWidth, int32_t prefHeight);
    void setMinSizeEnabled(bool enabled);
    void setMaxSizeEnabled(bool enabled);
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

    int32_t getWidth() const;
    int32_t getHeight() const;
    const Dimension& getSize() const;

    const Dimension& getMinSize() const;
    const Dimension& getMaxSize() const;
    const Dimension& getPrefSize() const;
    bool isMinSizeSet() const;
    bool isMaxSizeSet() const;
    bool isPrefSizeSet() const;

    Color getForeground() const;
    Color getBackground() const;

protected:
    /* Context */
    Renderer*          m_Renderer    = nullptr;
    Texture*           m_Texture     = nullptr;
    id128_t            m_Id          = {};

    /* Scene */
    Component*         m_Parent      = nullptr;
    GuiEventDispatcher m_Dispatcher;
    bool               m_IsVisible   = true;
    bool               m_IsFocused   = false;

    /* Layout (in the parent's coordinate system) */
    int32_t            m_X           = 0;
    int32_t            m_Y           = 0;

    Dimension          m_Size        = 0;
    
    bool               m_MinSizeSet  = false;
    Dimension          m_MinSize     = {0, 0};
    bool               m_MaxSizeSet  = false;
    Dimension          m_MaxSize     = {0, 0};
    bool               m_PrefSizeSet = false;
    Dimension          m_PrefSize    = {0, 0};

    /* Visuals */
    Color              m_Foreground  = COLOR_BLACK;
    Color              m_Background  = COLOR_WHITE;

    void updateTextureSize();
};

#endif // COMPONENT_H