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
#include "style/shadow.h"

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

        virtual Component* getHitComponent(int32_t x, int32_t y);
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

        const Shadow& getShadow() const;
        void setShadow(const ShadowSpecification* specification);

        GuiEventDispatcher* getEventDispatcher();

        Scene* getScene();
        void setScene(Scene* scene);

        Parent* getParent();
        void setParent(Parent* parent);

        Sml::Vec2<int32_t> computeLocalToScenePos(const Sml::Vec2<int32_t>& localPos);
        Sml::Vec2<int32_t> computeSceneToLocalPos(const Sml::Vec2<int32_t>& scenePos);

        Sml::Rectangle<int32_t> getOriginBounds() const;
        const Sml::Rectangle<int32_t>& getLayoutBounds() const;
        const Sml::Vec2<int32_t>& getLayoutPos() const;
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