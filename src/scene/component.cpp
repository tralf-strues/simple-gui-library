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
    Component* Component::getHitComponent(int32_t x, int32_t y)
    {
        if (Sml::isPointInsideRectangle({x, y}, getLayoutBounds()))
        {
            return this;
        }

        return nullptr;
    }

    void Component::layout() {}
    void Component::prerender() {}

    bool Component::containsComponent(Component* component)
    {
        assert(component);
        return this == component;
    }

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

    const Shadow& Component::getShadow() const
    {
        return m_Shadow;
    }

    void Component::setShadow(const ShadowSpecification* specification)
    {
        m_Shadow.setSpecification(specification);
    }

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

    const Parent* Component::getParent() const
    {
        return m_Parent;
    }

    void Component::setParent(Parent* parent)
    {
        m_Parent = parent;
        m_Scene  = m_Parent->getScene();
    }

    Sml::Vec2i Component::computeLocalToScenePos(const Sml::Vec2i& localPos) const
    {
        if (getParent() == nullptr) { return localPos; }
        return getParent()->computeLocalToScenePos(localPos + getLayoutPos());
    }

    Sml::Vec2i Component::computeSceneToLocalPos(const Sml::Vec2i& scenePos) const
    {
        if (getParent() == nullptr) { return scenePos - getLayoutPos(); }
        return getParent()->computeSceneToLocalPos(scenePos - getLayoutPos());
    }

    Sml::Rectangle<int32_t> Component::getOriginBounds() const
    {
        return Sml::Rectangle<int32_t>{{0, 0}, getLayoutWidth(), getLayoutHeight()};
    }

    const Sml::Rectangle<int32_t>& Component::getLayoutBounds() const
    {
        return m_LayoutBounds;
    }

    const Sml::Vec2i& Component::getLayoutPos() const
    {
        return m_LayoutBounds.pos;
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

    Sml::Vec2i Component::computeScenePos()
    {
        Sml::Vec2i scenePos = getLayoutBounds().pos;

        const Component* parent = getParent();
        while (parent != nullptr)
        {
            scenePos += parent->getLayoutBounds().pos;
            parent = parent->getParent();
        }

        return scenePos;
    }

    void Component::updateShadow()
    {
        if (getLayoutWidth() == 0 || getLayoutHeight() == 0 || m_Shadow.getSpecification() == nullptr || !isVisible())
        {
            return;
        }

        Sml::Renderer& renderer = Sml::Renderer::getInstance();

        Sml::Texture* renderedComponent = new Sml::Texture(m_LayoutBounds.width, m_LayoutBounds.height);
        
        renderer.pushTarget();

        renderer.setTarget(renderedComponent);

        Sml::Vec2i savedLayoutPos = m_LayoutBounds.pos;
        m_LayoutBounds.pos = {0, 0};
        render(getOriginBounds());
        m_LayoutBounds.pos = savedLayoutPos;

        renderer.popTarget();

        m_Shadow.update(renderedComponent, getLayoutBounds());

        delete renderedComponent;
    }

    void Component::renderShadow(const Sml::Rectangle<int32_t>& targetRegion)
    {
        if (isVisible())
        {   
            m_Shadow.render(getLayoutBounds(), targetRegion);
        }
    }

    void Component::setSceneInSceneTree(Scene* scene)
    {
        m_Scene = scene;
    }
}