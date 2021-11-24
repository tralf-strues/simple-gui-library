/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.cpp
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#include "sml/math/blur.h" // FIXME:
#include "core.h"
#include "scene/scene.h"
#include "scene/containers/context_menu.h"
#include "scene/controls/button.h"

namespace Sgl
{
    MenuItem::MenuItem(const char* label)
        : Button(new DefaultSkins::MenuItemSkin(), label) {}

    const Border     ContextMenu::DEFAULT_BORDER          = {1, 0xCF'CF'CF'FF};
    const ColorFill  ContextMenu::DEFAULT_BACKGROUND_FILL = {0xEE'EE'EE'FF};
    const Background ContextMenu::DEFAULT_BACKGROUND      = {&DEFAULT_BACKGROUND_FILL};
    const Shadow     ContextMenu::DEFAULT_SHADOW          = {{3, 3}, 3, 0x00'00'00'FF};

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
        // setShadow(&DEFAULT_SHADOW);

        setFillAcross(true);

        setScene(scene);
        scene->registerContextMenu(this);
        getEventDispatcher()->attachHandler(FocusListener<ContextMenu>::EVENT_TYPES,
                                            new ContextMenuFocusListener(this));
    }

    void ContextMenu::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        Container::render(targetRegion);

        // Sml::applyGaussianBlur(getContextRenderer(), getLayoutBounds(), 5);
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