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
#include "../paint/shadow.h"

namespace Sgl
{
    class Parent;
    class Scene;

    enum class Orientation
    {
        HORIZONTAL,
        VERTICAL
    };

    class Component : public Sml::EventTarget
    {
    public:
        /**
         * @brief Used to determine if Component should be rendered and targeted by events.
         * 
         * @note The least significant bit specifies interactability, and the following one
         *       specifies visibility.
         */
        enum class Visibility
        {
            INVISIBLE_DISABLED     = 0b00,
            INVISIBLE_INTERACTABLE = 0b01,
            VISIBLE_DISABLED       = 0b10,
            VISIBLE_INTERACTABLE   = 0b11
        };

    public:
        static const int32_t USE_COMPUTED_SIZE = INT32_MIN;
        static const int32_t UNLIMITED_SIZE    = INT32_MAX; 

    public:
        friend Scene;

        Component() = default;
        virtual ~Component() = default;

        virtual Component* getHitComponent(int32_t x, int32_t y);
        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) = 0;
        virtual void layout();
        virtual void prerender();
        virtual bool containsComponent(Component* component);

        virtual Sml::EventDispatchChain* buildEventDispatchChain(Sml::EventDispatchChain* chain) override;

        bool isVisible() const;
        void setVisible(bool visible);

        bool isInteractable() const;
        void setInteractable(bool interactable);

        Visibility getVisibility() const;
        void setVisibility(Visibility visibility);

        bool isFocused() const;
        bool isHovered() const;

        virtual bool isResizable() const = 0;

        void requestFocus();
        void requestDrag();

        const Shadow& getShadow() const;
        void setShadow(const ShadowSpecification* specification);

        GuiEventDispatcher* getEventDispatcher();

        Scene* getScene();
        void setScene(Scene* scene);

        const Parent* getParent() const;
        Parent* getModifiableParent();
        void setParent(Parent* parent);

        Sml::Vec2i computeLocalToScenePos(const Sml::Vec2i& localPos) const;
        Sml::Vec2i computeSceneToLocalPos(const Sml::Vec2i& scenePos) const;

        Sml::Rectangle<int32_t> getOriginBounds() const;
        const Sml::Rectangle<int32_t>& getLayoutBounds() const;
        const Sml::Vec2i& getLayoutPos() const;
        int32_t getLayoutX() const;
        int32_t getLayoutY() const;
        int32_t getLayoutWidth() const;
        int32_t getLayoutHeight() const;

        void setLayoutX(int32_t x);
        void setLayoutY(int32_t y);
        void setLayoutWidth(int32_t width);
        void setLayoutHeight(int32_t height);

        Sml::Vec2i computeScenePos();

        virtual int32_t computePrefWidth(int32_t height = -1) const = 0;
        virtual int32_t computePrefHeight(int32_t width = -1) const = 0;
        
        virtual int32_t computeMinWidth(int32_t height = -1) const = 0;
        virtual int32_t computeMinHeight(int32_t width = -1) const = 0;

        virtual int32_t computeMaxWidth(int32_t height = -1) const = 0;
        virtual int32_t computeMaxHeight(int32_t width = -1) const = 0;

    protected:
        Visibility              m_Visibility   = Visibility::VISIBLE_INTERACTABLE;
        bool                    m_Focused      = false;
        bool                    m_Hovered      = false;
        Shadow                  m_Shadow;
        
        GuiEventDispatcher      m_Dispatcher;

        Scene*                  m_Scene        = nullptr;
        Parent*                 m_Parent       = nullptr;
        Sml::Rectangle<int32_t> m_LayoutBounds = {{0, 0}, 0, 0};

        void updateShadow();
        void renderShadow(const Sml::Rectangle<int32_t>& targetRegion);
        virtual void setSceneInSceneTree(Scene* scene);
    };
}