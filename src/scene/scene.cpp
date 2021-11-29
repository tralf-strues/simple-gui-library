/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scene.cpp
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/events/system_events.h"
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
            proccessMouseEvent(dynamic_cast<Sml::MouseEvent*>(event));
        }
        else if (event->isInCategory(Sml::KeyEvent::getStaticCategory()))
        {
            proccessKeyboardEvent(dynamic_cast<Sml::KeyEvent*>(event));
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

    void Scene::requestFocus(Component* component)
    {
        if (m_FocusOwner != component)
        {
            if (m_FocusOwner != nullptr)
            {
                FocusEvent focusEvent{FocusEvent::Type::FOCUS_LOST, m_FocusOwner};
                m_FocusOwner->m_Focused = false;
                fireEvent(&focusEvent);
            }

            if (component != nullptr)
            {
                FocusEvent focusEvent{FocusEvent::Type::FOCUS_GOT, component};
                component->m_Focused = true;
                fireEvent(&focusEvent);
            }
        }

        m_FocusOwner = component;
    }

    Component* Scene::getDragOwner() { return m_DragOwner; }

    void Scene::requestDrag(Component* component)
    {
        assert(component);

        finishDrag();

        m_DragOwner = component;

        DragStartEvent dragStartEvent{Sml::MouseState::getMouseState().x,
                                      Sml::MouseState::getMouseState().y,
                                      m_DragOwner};
        fireEvent(&dragStartEvent);
    }

    void Scene::finishDrag()
    {
        if (m_DragOwner != nullptr)
        {
            Sml::Vec2<int32_t> pos{Sml::MouseState::getMouseState().x, Sml::MouseState::getMouseState().y};
            pos = m_DragOwner->computeSceneToLocalPos(pos);

            DragEndEvent dragEndEvent{Sml::MouseState::getMouseState().x,
                                      Sml::MouseState::getMouseState().y,
                                      m_DragOwner};
            fireEvent(&dragEndEvent);

            m_DragOwner = nullptr;
        }
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

    Component* Scene::findHitComponentInContextMenus(int32_t x, int32_t y, ContextMenu** menu)
    {
        Component* hitComponent = nullptr;
        ContextMenu* hitMenu = nullptr;

        for (ContextMenu* contextMenu : m_ContextMenus)
        {
            hitComponent = contextMenu->getHitComponent(x, y);
            if (hitComponent != nullptr)
            {
                hitMenu = contextMenu;
                break;
            }
        }

        if (menu != nullptr)
        {
            *menu = hitMenu;
        }

        return hitComponent;
    }

    Component* Scene::findHitComponentInRoot(int32_t x, int32_t y)
    {
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
                MouseExitedEvent mouseExitedEvent{mouseX, mouseY, m_HoverOwner};
                m_HoverOwner->m_Hovered = false;
                fireEvent(&mouseExitedEvent);
            }

            if (newHoverOwner != nullptr)
            {
                MouseEnteredEvent mouseEnteredEvent{mouseX, mouseY, newHoverOwner};
                newHoverOwner->m_Hovered = true;
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

        ContextMenu* hitMenu = nullptr;
        Component* newHoverOwner = findHitComponentInContextMenus(mouseEvent->getX(), mouseEvent->getY(),
                                                                  &hitMenu);

        if (newHoverOwner == nullptr)
        {
            newHoverOwner = findHitComponentInRoot(mouseEvent->getX(), mouseEvent->getY());
        }

        updateHoverOwner(newHoverOwner, mouseEvent->getX(), mouseEvent->getY());

        if (mouseEvent->isInCategory(Sml::SystemEventCategory::EVENT_CATEGORY_MOUSE_BUTTON))
        {
            finishDrag();
        }

        if (mouseEvent->getType() == Sml::MouseButtonPressedEvent::getStaticType())
        {
            requestFocus(newHoverOwner);
        }

        if (mouseEvent->getType() == Sml::MouseMovedEvent::getStaticType() && m_DragOwner != nullptr)
        {
            Sml::MouseMovedEvent* mouseMovedEvent = dynamic_cast<Sml::MouseMovedEvent*>(mouseEvent);
            DragMoveEvent dragMoveEvent{mouseMovedEvent->getX(), mouseMovedEvent->getY(),
                                        mouseMovedEvent->getDeltaX(), mouseMovedEvent->getDeltaY(),
                                        m_DragOwner};

            fireEvent(&dragMoveEvent);
        }

        if (newHoverOwner != nullptr)
        {
            mouseEvent->setTarget(newHoverOwner);
            fireEvent(mouseEvent);
        }
    }
}