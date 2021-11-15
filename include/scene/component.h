/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file component.h
 * @date 2021-11-05
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "sml/sml_graphics_wrapper.h"
#include "../events/gui_event_dispatching.h"

namespace Sgl
{
    class Parent;
    class Scene;

    class Component : public Sml::EventTarget
    {
    public:
        static const int32_t USE_COMPUTED_SIZE = INT32_MIN;
        static const int32_t UNLIMITED_SIZE    = INT32_MAX; 

    public:
        friend Scene;

        Component() = default;
        virtual ~Component() = default;

        virtual Component* getHitComponent(int32_t x, int32_t y) = 0;
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) = 0;
        virtual void layout() = 0;
        virtual void prerender() = 0;

        virtual Sml::EventDispatchChain* buildEventDispatchChain(Sml::EventDispatchChain* chain) override;

        bool isVisible() const;
        void setVisible(bool visible);

        bool isFocused() const;
        bool isHovered() const;

        virtual bool isResizable() const = 0;

        void requestFocus();
        void requestDrag();

        // void moveToFront();

        GuiEventDispatcher* getEventDispatcher();

        Scene* getScene();
        void setScene(Scene* scene);

        Parent* getParent();
        void setParent(Parent* parent);

        Sml::Rectangle<int32_t> getOriginBounds() const;
        const Sml::Rectangle<int32_t>& getLayoutBounds() const;
        int32_t getLayoutX() const;
        int32_t getLayoutY() const;
        int32_t getLayoutWidth() const;
        int32_t getLayoutHeight() const;

        void setLayoutX(int32_t x);
        void setLayoutY(int32_t y);
        void setLayoutWidth(int32_t width);
        void setLayoutHeight(int32_t height);

        Sml::Vec2<int32_t> computeScenePos();

        virtual int32_t computePrefWidth(int32_t height = -1) const = 0;
        virtual int32_t computePrefHeight(int32_t width = -1) const = 0;
        
        virtual int32_t computeMinWidth(int32_t height = -1) const = 0;
        virtual int32_t computeMinHeight(int32_t width = -1) const = 0;

        virtual int32_t computeMaxWidth(int32_t height = -1) const = 0;
        virtual int32_t computeMaxHeight(int32_t width = -1) const = 0;

    protected:
        bool                    m_Visible      = true;
        bool                    m_Focused      = false;
        bool                    m_Hovered      = false;
        GuiEventDispatcher      m_Dispatcher;

        Scene*                  m_Scene        = nullptr;
        Parent*                 m_Parent       = nullptr;
        Sml::Rectangle<int32_t> m_LayoutBounds = {{0, 0}, 0, 0};

        virtual void setSceneInSceneTree(Scene* scene);
    };
}

    // /* Setters */
    // void setRenderer(Renderer* renderer);
    // void setId(const id128_t& id);

    // void setParent(Component* parent);
    // void setEventDispatcher(GuiEventDispatcher* dispatcher);
    // void setVisible(bool visible);
    // void setFocused(bool focused); // TODO: !!!implement!!!

    // void setX(int32_t x);
    // void setY(int32_t y);

    // void setSize(int32_t width, int32_t height);
    // void setSize(const Vec2<int32_t>& size);

    // void setWidth(int32_t width);
    // void setHeight(int32_t height);
    
    // // void setMinSize(int32_t minWidth, int32_t minHeight);
    // // void setMinSize(const Vec2<int32_t>& minSize);
    // // void setMaxSize(int32_t maxWidth, int32_t maxHeight);
    // // void setMaxSize(const Vec2<int32_t>& maxSize);
    // // void setPrefSize(int32_t prefWidth, int32_t prefHeight);
    // // void setPrefSize(const Vec2<int32_t>& prefSize);
    // void setPrefWidth(int32_t width);
    // void setPrefHeight(int32_t height);
    // // void setMinSizeEnabled(bool enabled);
    // // void setMaxSizeEnabled(bool enabled);
    // void setPrefSizeEnabled(bool enabled);

    // void setForeground(Color foreground);
    // void setBackground(Color background);

    // /* Getters */
    // Renderer* getRenderer() const;
    // Texture* getTexture() const;
    // id128_t getId() const;

    // Component* getParent() const;
    // GuiEventDispatcher* getEventDispatcher();
    // bool isVisible() const;
    // bool isFocused() const;

    // int32_t getX() const;
    // int32_t getY() const;
    // Vec2<int32_t> getPos() const;
    // Rectangle<int32_t> getRegion() const;

    // int32_t getWidth() const;
    // int32_t getHeight() const;
    // const Vec2<int32_t>& getSize() const;

    // // const Vec2<int32_t>& getMinSize() const;
    // // const Vec2<int32_t>& getMaxSize() const;
    // virtual int32_t getPrefWidth() const;
    // virtual int32_t getPrefHeight() const;
    // // bool isMinSizeEnabled() const;
    // // bool isMaxSizeEnabled() const;
    // bool isPrefSizeEnabled() const;

    // Color getForeground() const;
    // Color getBackground() const;
