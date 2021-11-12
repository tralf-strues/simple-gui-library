/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.cpp
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/scene.h"
#include "scene/containers/context_menu.h"

namespace Sgl
{
    ContextMenu::ContextMenu(Scene* scene, Component* sourceComponent)
        : BoxContainer(BoxContainer::Direction::TOP_TO_BOTTOM),
          m_Source(sourceComponent)
    {
        assert(scene);

        setScene(scene);
        scene->registerContextMenu(this);

        setFillAcross(true);
    }

    Component* ContextMenu::getSource() { return m_Source; }
    void ContextMenu::setSource(Component* source) { m_Source = source; }
}