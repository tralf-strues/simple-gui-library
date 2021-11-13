/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scene.h
 * @date 2021-11-06
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../events/gui_event_dispatching.h"
#include "containers/menu_bar.h"

namespace Sgl
{
    class Component;
    class Parent;

    class Scene : public Sml::EventTarget
    {
    public:
        Scene(int32_t width, int32_t height);
        Scene() = default;
        ~Scene() = default;

        void update();
        void render(const Sml::Rectangle<int32_t>& targetRegion);
        void proccessEvent(Sml::Event* event);

        virtual Sml::EventDispatchChain* buildEventDispatchChain(Sml::EventDispatchChain* chain) override;
        GuiEventDispatcher* getEventDispatcher();

        Sml::Rectangle<int32_t> getLayoutRegion() const;

        int32_t getWidth() const;
        void setWidth(int32_t width);

        int32_t getHeight() const;
        void setHeight(int32_t height);

        Parent* getRoot();
        void setRoot(Parent* root);

        Component* getFocusOwner();
        void requestFocus(Component* component);

        void registerContextMenu(ContextMenu* contextMenu);
        void detachContextMenu(ContextMenu* contextMenu);

    private:
        GuiEventDispatcher      m_Dispatcher;

        int32_t                 m_Width      = 0;
        int32_t                 m_Height     = 0;

        Parent*                 m_Root       = nullptr;
        Component*              m_FocusOwner = nullptr;
        Component*              m_HoverOwner = nullptr;

        std::list<ContextMenu*> m_ContextMenus;

        Component* findHitComponentInContextMenus(int32_t x, int32_t y, ContextMenu** menu);
        Component* findHitComponentInRoot(int32_t x, int32_t y);
        void updateHoverOwner(Component* newHoverOwner, int32_t mouseX, int32_t mouseY);
        void updateFocusOwner(Component* newFocusOwner);
        void proccessKeyboardEvent(Sml::KeyEvent* keyEvent);
        void proccessMouseEvent(Sml::MouseEvent* mouseEvent);
    };
}
