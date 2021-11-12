/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scene.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "core.h"
#include "scene/scene.h"
#include "scene/parent.h"

namespace Sgl
{
    Scene::Scene(int32_t width, int32_t height) : m_Width(width), m_Height(height) {}

    void Scene::update()
    {
        if (m_Root == nullptr) { return; }

        m_Root->setLayoutX(0);
        m_Root->setLayoutY(0);
        m_Root->setLayoutWidth(m_Width);
        m_Root->setLayoutHeight(m_Height);

        m_Root->layout();
        m_Root->prerender();

        for (ContextMenu* contextMenu : m_ContextMenus)
        {
            contextMenu->setLayoutWidth(contextMenu->computePrefWidth());
            contextMenu->setLayoutHeight(contextMenu->computePrefHeight());

            Component* source = contextMenu->getSource();
            if (source == nullptr)
            {
                Sml::Rectangle<int32_t> centeredPos = Sml::centerRegion(getLayoutRegion(),
                                                                        contextMenu->getOriginBounds());

                contextMenu->setLayoutX(centeredPos.pos.x);
                contextMenu->setLayoutY(centeredPos.pos.y);
            }
            else
            {
                Sml::Vec2<int32_t> bottomLeftCorner = source->computeScenePos();
                bottomLeftCorner.y += source->getLayoutHeight();

                contextMenu->setLayoutX(bottomLeftCorner.x);
                contextMenu->setLayoutY(bottomLeftCorner.y);
            }

            contextMenu->layout();
            contextMenu->prerender();
        }
    }

    void Scene::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        // getContextRenderer()->setColor(Sml::COLOR_WHITE);
        // getContextRenderer()->clear();

        if (m_Root != nullptr)
        {
            m_Root->render(targetRegion);
        }

        for (ContextMenu* contextMenu : m_ContextMenus)
        {
            contextMenu->render(targetRegion);
        }
    }

    void Scene::proccessEvent(Sml::Event* event)
    {
        assert(event);

        if (event->isInCategory(Sml::MouseEvent::getStaticCategory()))
        {
            proccessMouseEvent(static_cast<Sml::MouseEvent*>(event));
        }
        else if (event->isInCategory(Sml::KeyEvent::getStaticCategory()))
        {
            proccessKeyboardEvent(static_cast<Sml::KeyEvent*>(event));
        }
    }

    Sml::EventDispatchChain* Scene::buildEventDispatchChain(Sml::EventDispatchChain* chain)
    {
        assert(chain);

        chain->prependDispatcher(&m_Dispatcher);
        return chain;
    }

    GuiEventDispatcher* Scene::getEventDispatcher() { return &m_Dispatcher; }

    Sml::Rectangle<int32_t> Scene::getLayoutRegion() const { return {{0, 0}, m_Width, m_Height}; }

    int32_t Scene::getWidth() const { return m_Width; }
    void Scene::setWidth(int32_t width) { m_Width = width; }

    int32_t Scene::getHeight() const { return m_Height; }
    void Scene::setHeight(int32_t height) { m_Height = height; }

    Parent* Scene::getRoot() { return m_Root; }
    void Scene::setRoot(Parent* root) { m_Root = root; m_Root->setScene(this); }

    Component* Scene::getFocusOwner() { return m_FocusOwner; }

    // TODO: Generate UnfocusedEvent and FocusedEvent
    void Scene::requestFocus(Component* component)
    {
        assert(component);

        component->m_Focused = true;

        if (m_FocusOwner != nullptr)
        {
            m_FocusOwner->m_Focused = false;
        }

        m_FocusOwner = component;
    }

    void Scene::registerContextMenu(ContextMenu* contextMenu)
    {
        assert(contextMenu);
        m_ContextMenus.push_back(contextMenu);
    }

    void Scene::detachContextMenu(ContextMenu* contextMenu)
    {
        assert(contextMenu);
        m_ContextMenus.remove(contextMenu);
    }

    Component* Scene::findHitComponent(int32_t x, int32_t y)
    {
        for (ContextMenu* contextMenu : m_ContextMenus)
        {
            Component* hitComponent = contextMenu->getHitComponent(x, y);
            if (hitComponent != nullptr)
            {
                return hitComponent;
            }
        }

        if (m_Root != nullptr)
        {
            return m_Root->getHitComponent(x, y);
        }

        return nullptr;
    }

    void Scene::updateHoverOwner(Component* newHoverOwner, int32_t mouseX, int32_t mouseY)
    {
        if (m_HoverOwner != newHoverOwner)
        {
            if (m_HoverOwner != nullptr)
            {
                MouseExitedEvent mouseExitedEvent{mouseX, mouseY};
                mouseExitedEvent.setTarget(m_HoverOwner);
                fireEvent(&mouseExitedEvent);
            }

            if (newHoverOwner != nullptr)
            {
                MouseEnteredEvent mouseEnteredEvent{mouseX, mouseY};
                mouseEnteredEvent.setTarget(newHoverOwner);
                fireEvent(&mouseEnteredEvent);
            }
        }

        m_HoverOwner = newHoverOwner;
    }

    void Scene::proccessKeyboardEvent(Sml::KeyEvent* keyEvent)
    {
        assert(keyEvent);

        // TODO:
    }

    void Scene::proccessMouseEvent(Sml::MouseEvent* mouseEvent)
    {
        assert(mouseEvent);

        if (m_Root == nullptr)
        {
            return;
        }

        Component* newHoverOwner = findHitComponent(mouseEvent->getX(), mouseEvent->getY());
        if (newHoverOwner != nullptr)
        {
            mouseEvent->setTarget(newHoverOwner);
            fireEvent(mouseEvent);
        }

        updateHoverOwner(newHoverOwner, mouseEvent->getX(), mouseEvent->getY());
    }
}