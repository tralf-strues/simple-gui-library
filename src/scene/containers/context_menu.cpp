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
    MenuItem::MenuItem(const char* label) : Button(new DefaultSkins::MenuItemSkin(), label) {}

    const Insets              ContextMenu::DEFAULT_PADDING         = {2};
    const Border              ContextMenu::DEFAULT_BORDER          = {1, 0xC9'C9'C9'66};
    const ColorFill           ContextMenu::DEFAULT_BACKGROUND_FILL = {0xEE'EE'EE'FF};
    const Background          ContextMenu::DEFAULT_BACKGROUND      = {&DEFAULT_BACKGROUND_FILL};
    const ShadowSpecification ContextMenu::DEFAULT_SHADOW          = {{0, 3}, {1.05, 0.95}, 7, 0x22'22'22'18};

    void MenuItem::setOnAction(ActionListener<MenuItem>* listener)
    {
        assert(listener);

        Button::setOnAction(reinterpret_cast<ActionListener<Button>*>(listener));
    }

    class ContextMenuFocusListener : public FocusListener<ContextMenu>
    {
    public:
        ContextMenuFocusListener(ContextMenu* menu) : FocusListener(menu) {}

        virtual void onFocusLost(FocusEvent* event) override
        {
            if (getComponent()->isAutoHide())
            {
                LOG_LIB_INFO("Hide ContextMenu!");
                getComponent()->hide();
            }
        }
    };

    ContextMenu::ContextMenu(Scene* scene, Component* sourceComponent)
        : m_Source(sourceComponent)
    {
        assert(scene);

        setPadding(DEFAULT_PADDING);
        setBorder(&DEFAULT_BORDER);
        setBackground(&DEFAULT_BACKGROUND);
        setShadow(&DEFAULT_SHADOW);

        setFillAcross(true);

        setScene(scene);
        scene->registerContextMenu(this);
        getEventDispatcher()->attachHandler(FocusListener<ContextMenu>::EVENT_TYPES,
                                            new ContextMenuFocusListener(this));
    }

    ContextMenu::~ContextMenu()
    {
        VBox::~VBox();

        m_Source->getScene()->detachContextMenu(this);
    }

    void ContextMenu::render(const Sml::Rectangle<int32_t>& targetRegion)
    {
        Container::render(targetRegion);
    }

    Component* ContextMenu::getSource() { return m_Source; }
    void ContextMenu::setSource(Component* source) { m_Source = source; }

    bool ContextMenu::isAutoHide() const { return m_AutoHide; }
    void ContextMenu::setAutoHide(bool autoHide) { m_AutoHide = autoHide; }

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