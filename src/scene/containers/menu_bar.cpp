/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file menu_bar.cpp
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#include "core.h"
#include "scene/containers/menu_bar.h"
#include "scene/border.h"
#include "scene/scene.h"

namespace Sgl
{
    const std::initializer_list<Sml::EventType> MENU_BAR_LISTENER_EVENTS = {
            Sml::MouseButtonPressedEvent::getStaticType(),
            MouseExitedEvent::getStaticType(),
        };

    class MenuSelectListener : public Sml::Listener
    {
    public:
        MenuSelectListener(MenuBar& menuBar, const char* menu) : m_MenuBar(menuBar), m_Menu(menu) {}

        virtual void onEvent(Sml::Event* event) override
        {
            assert(event->isInCategory(Sml::MouseEvent::getStaticCategory()));

            LOG_INFO("MenuSelectListener triggered");

            switch (event->getType())
            {
                case Sml::MouseButtonPressedEvent::getStaticType():
                {
                    // m_Menu->getMenuItems()->setVisible(!m_Menu->getMenuItems()->isVisible());
                    m_MenuBar.showMenu(m_Menu);
                    break;
                }

                case MouseExitedEvent::getStaticType():
                {
                    // m_Menu->getMenuItems()->setVisible(false);
                    break;
                }

                default:
                {
                    assert(! "Invalid MenuSelectListener event type!");
                    break;
                }
            }
        }

    private:
        MenuBar&    m_MenuBar;
        const char* m_Menu;
    };

    Menu::Menu(Scene* scene, const char* title)
        : m_TitleButton(new Button{title}),
          m_ContextMenu(new ContextMenu(scene, m_TitleButton))
    {
        m_ContextMenu->setFillAcross(true);
    }

    Menu::~Menu()
    {
        delete m_TitleButton;
        delete m_ContextMenu;
    }

    const char* Menu::getTitle() const
    {
        return m_TitleButton->getLabel();
    }

    Button* Menu::getTitleButton()
    {
        return m_TitleButton;
    }

    ContextMenu* Menu::getContextMenu()
    {
        return m_ContextMenu;
    }

    const ColorFill  DEFAULT_CONTAINER_FILL      = {0xF5'F5'F5'FF};
    const Background MenuBar::DEFAULT_BACKGROUND = {{&DEFAULT_CONTAINER_FILL}, {}};
    const Border     MenuBar::DEFAULT_BORDER     = Border{1, 0xE9'E9'E9'FF};

    MenuBar::MenuBar(Scene* scene)
    {
        setScene(scene);
        setFillAcross(true);
        setBackground(&DEFAULT_BACKGROUND);
        setBorder(&DEFAULT_BORDER);
    }

    // TODO: delete context menus from the scene
    MenuBar::~MenuBar()
    {
        for (Menu* menu : m_Menus)
        {
            delete menu;
        }
    }

    Menu* MenuBar::addMenu(const char* title)
    {
        assert(title);

        Menu* addedMenu = new Menu{getScene(), title};
        m_Menus.push_back(addedMenu);
        addedMenu->getContextMenu()->setVisible(false);
        addedMenu->getTitleButton()->getEventDispatcher()->attachHandler(MENU_BAR_LISTENER_EVENTS,
                                                                        new MenuSelectListener{*this, title});

        // addedMenu->getTitleButton()->setWidth(addedMenu->getTitleButton().g);
        // FIXME:
        // m_Components.insert(getFirstComponent(), addedMenu->getTitleButton());
        // m_Components.push_back(addedMenu->getMenuItems());
        addChild(addedMenu->getTitleButton());
        getScene()->registerContextMenu(addedMenu->getContextMenu());

        return addedMenu;
    }

    Menu* MenuBar::getMenu(const char* title)
    {
        assert(title);

        for (auto menu : m_Menus)
        {
            if (strcmp(title, menu->getTitle()) == 0)
            {
                return menu;
            }
        }

        return nullptr;
    }

    void MenuBar::showMenu(const char* title)
    {
        assert(title);
        
        Menu* menu = getMenu(title);
        if (menu != nullptr)
        {
            menu->getContextMenu()->setVisible(true);
        }
    }

    void MenuBar::hideMenu(const char* title)
    {
        assert(title);

        Menu* menu = getMenu(title);
        if (menu != nullptr)
        {
            menu->getContextMenu()->setVisible(false);
        }
    }
}