/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.h
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "box_container.h"

namespace Sgl
{
    class Scene;

    class ContextMenu : public VBox
    {
    public:
        ContextMenu(Scene* scene, Component* sourceComponent = nullptr);

        Component* getSource();
        void setSource(Component* source);

    private:
        Component* m_Source = nullptr;
    };
}