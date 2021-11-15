/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file component.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/parent.h"
#include "scene/scene.h"

namespace Sgl
{
    Sml::EventDispatchChain* Component::buildEventDispatchChain(Sml::EventDispatchChain* chain)
    {
        assert(chain);

        chain->prependDispatcher(&m_Dispatcher);

        if (m_Parent == nullptr)
        {
            return chain;
        }
        
        return m_Parent->buildEventDispatchChain(chain);
    }

    bool Component::isVisible() const { return m_Visible; }
    void Component::setVisible(bool visible) { m_Visible = visible; }

    bool Component::isFocused() const { return m_Focused; }
    bool Component::isHovered() const { return m_Hovered; }

    void Component::requestFocus()
    {
        assert(getScene());

        getScene()->requestFocus(this);
    }

    void Component::requestDrag()
    {
        assert(getScene());

        getScene()->requestDrag(this);
    }

    // void Component::moveToFront()
    // {
    //     if (getParent() == nullptr) { return; }

    //     getParent()->getChildren().remove(this);
    //     getParent()->getChildren().push_back(this);

    //     getParent()->moveToFront();
    // }

    GuiEventDispatcher* Component::getEventDispatcher()
    {
        return &m_Dispatcher;
    }

    Scene* Component::getScene()
    {
        return m_Scene;
    }

    void Component::setScene(Scene* scene)
    {
        setSceneInSceneTree(scene);
    }

    Parent* Component::getParent()
    {
        return m_Parent;
    }

    void Component::setParent(Parent* parent)
    {
        m_Parent = parent;
        m_Scene  = m_Parent->getScene();
    }

    Sml::Rectangle<int32_t> Component::getOriginBounds() const
    {
        return Sml::Rectangle<int32_t>{{0, 0}, getLayoutWidth(), getLayoutHeight()};
    }

    const Sml::Rectangle<int32_t>& Component::getLayoutBounds() const
    {
        return m_LayoutBounds;
    }

    int32_t Component::getLayoutX() const
    {
        return m_LayoutBounds.pos.x;
    }

    int32_t Component::getLayoutY() const
    {
        return m_LayoutBounds.pos.y;
    }

    int32_t Component::getLayoutWidth() const
    {
        return m_LayoutBounds.width;
    }

    int32_t Component::getLayoutHeight() const
    {
        return m_LayoutBounds.height;
    }

    void Component::setLayoutX(int32_t x)
    {
        m_LayoutBounds.pos.x = x;
    }

    void Component::setLayoutY(int32_t y)
    {
        m_LayoutBounds.pos.y = y;
    }

    void Component::setLayoutWidth(int32_t width)
    {
        m_LayoutBounds.width = width;
    }

    void Component::setLayoutHeight(int32_t height)
    {
        m_LayoutBounds.height = height;
    }

    Sml::Vec2<int32_t> Component::computeScenePos()
    {
        Sml::Vec2<int32_t> scenePos = getLayoutBounds().pos;

        Component* parent = getParent();
        while (parent != nullptr)
        {
            scenePos += parent->getLayoutBounds().pos;
            parent = parent->getParent();
        }

        return scenePos;
    }

    void Component::setSceneInSceneTree(Scene* scene)
    {
        m_Scene = scene;
    }
}