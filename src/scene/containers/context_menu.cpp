/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.cpp
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#include "core.h"
#include "scene/scene.h"
#include "scene/containers/context_menu.h"
#include "scene/controls/button.h"

namespace Sgl
{
    MenuItem::MenuItem(const char* label)
        : Button(new DefaultSkins::MenuItemSkin(), label) {}

    const Border     ContextMenu::DEFAULT_BORDER          = {1, 0xCF'CF'CF'FF};
    const ColorFill  ContextMenu::DEFAULT_BACKGROUND_FILL = {0xE8'E6'EB'FF};
    const Background ContextMenu::DEFAULT_BACKGROUND      = {&DEFAULT_BACKGROUND_FILL};

    class ContextMenuFocusListener : public FocusListener<ContextMenu>
    {
    public:
        ContextMenuFocusListener(ContextMenu* menu) : FocusListener(menu) {}

        virtual void onFocusLost(FocusEvent* event) override
        {
            LOG_INFO("Hide ContextMenu!");
            getComponent()->hide();
        }
    };

    ContextMenu::ContextMenu(Scene* scene, Component* sourceComponent)
        : m_Source(sourceComponent)
    {
        assert(scene);

        setBorder(&DEFAULT_BORDER);
        setBackground(&DEFAULT_BACKGROUND);

        setFillAcross(true);

        setScene(scene);
        scene->registerContextMenu(this);
        getEventDispatcher()->attachHandler(FocusListener<ContextMenu>::EVENT_TYPES,
                                            new ContextMenuFocusListener(this));
    }

    Component* ContextMenu::getSource() { return m_Source; }
    void ContextMenu::setSource(Component* source) { m_Source = source; }

    void ContextMenu::show()
    {
        setVisible(true);
        requestFocus();
    }

    void ContextMenu::hide()
    {
        setVisible(false);
    }
}