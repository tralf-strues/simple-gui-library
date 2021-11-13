/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.cpp
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/scene.h"
#include "scene/containers/context_menu.h"
#include "scene/controls/button.h"

namespace Sgl
{
    MenuItem::MenuItem(const char* label)
        : Button(new DefaultSkins::MenuItemSkin(), label) {}

    const Border ContextMenu::DEFAULT_BORDER = Border{1, 0xCF'CF'CF'FF};

    ContextMenu::ContextMenu(Scene* scene, Component* sourceComponent)
        : m_Source(sourceComponent)
    {
        assert(scene);

        setBorder(&DEFAULT_BORDER);

        setScene(scene);
        scene->registerContextMenu(this);

        setFillAcross(true);
    }

    Component* ContextMenu::getSource() { return m_Source; }
    void ContextMenu::setSource(Component* source) { m_Source = source; }
}