//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file menu_bar.cpp
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "containers/menu_bar.h"

class MenuSelectListener : public Listener
{
public:
    MenuSelectListener(Menu* menu) : m_Menu(menu) {}

    virtual void onEvent(Event* event) override
    {
        assert(event->isInCategory(MouseEvent::getStaticCategory()));

        printf("MenuSelectListener triggered.\n");

        switch (event->getType())
        {
            case MouseButtonPressedEvent::getStaticType():
            {
                m_Menu->getMenuItems()->setVisible(!m_Menu->getMenuItems()->isVisible());
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
    Menu* m_Menu;
};

class MenuItemSelectListener : public Listener
{
public:
    MenuItemSelectListener(Label* label) : m_Label(label) {}

    virtual void onEvent(Event* event) override
    {
        
    }

private:
    Label* m_Label;
};

Menu::Menu(Renderer* renderer, const Font& font, const char* title)
    : m_TitleButton(new Button{renderer, font, title}),
      m_MenuItems(new ListMenu{renderer}) {}

Menu::~Menu()
{
    delete m_TitleButton;
    delete m_MenuItems;
}

const char* Menu::getTitle() const
{
    return m_TitleButton->getLabel()->getText();
}

Button* Menu::getTitleButton()
{
    return m_TitleButton;
}

ListMenu* Menu::getMenuItems()
{
    return m_MenuItems;
}

MenuBar::MenuBar(Renderer* renderer, const Font& font) 
    : SpacedContainer(renderer), m_Font(&font)
{
    // setSkin(&DEFAULT_SKIN_MENUBAR);
}

MenuBar::~MenuBar()
{
    SpacedContainer::~SpacedContainer();

    for (Menu* menu : m_Menus)
    {
        delete menu;
    }
}

Menu* MenuBar::addMenu(const char* title)
{
    assert(title);

    Menu* addedMenu = new Menu{m_Renderer, *m_Font, title};
    m_Menus.push_back(addedMenu);
    addedMenu->getMenuItems()->setVisible(false);
    addedMenu->getTitleButton()->getEventDispatcher()->attachHandler({MouseButtonPressedEvent::getStaticType(), 
                                                                      MouseExitedEvent::getStaticType()},
                                                                     new MenuSelectListener{addedMenu});

    // addedMenu->getTitleButton()->setWidth(addedMenu->getTitleButton().g);
    // FIXME:
    // m_Components.insert(getFirstComponent(), addedMenu->getTitleButton());
    m_Components.push_back(addedMenu->getTitleButton());
    // m_Components.push_back(addedMenu->getMenuItems());

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
        menu->getMenuItems()->setVisible(true);
    }
}

void MenuBar::hideMenu(const char* title)
{
    assert(title);

    Menu* menu = getMenu(title);
    if (menu != nullptr)
    {
        menu->getMenuItems()->setVisible(false);
    }
}

Component* MenuBar::getHitComponent(int32_t x, int32_t y)
{
    for (Menu* menu : m_Menus)
    {
        Component* hitComponent = menu->getMenuItems()->getHitComponent(x - m_X, y - m_Y);
        if (hitComponent != nullptr)
        {
            return hitComponent;
        }
    }

    return SpacedContainer::getHitComponent(x - m_X, y - m_Y);
}

void MenuBar::updateGraphics()
{
    SpacedContainer::updateGraphics();

    for (Menu* menu : m_Menus)
    {
        // FIXME:
        // if (menu->getMenuItems()->getWidth() < DEFAULT_LIST_MENU_MIN_WIDTH)
        // {
        //     menu->getMenuItems()->setWidth(DEFAULT_LIST_MENU_MIN_WIDTH);
        // }

        menu->getMenuItems()->updateGraphics();
    }
    
    // setDefaultStyle();
}

void MenuBar::render(Texture* target, const Rectangle<int32_t>& targetRegion)
{
    SpacedContainer::render(target, targetRegion);

    for (Menu* menu : m_Menus)
    {
        Button* titleButton = menu->getTitleButton();
        ListMenu* menuItems = menu->getMenuItems();

        menuItems->setX(titleButton->getX());
        menuItems->setY(titleButton->getY() + titleButton->getHeight());

        menuItems->render(target, targetRegion);
    }
}

void MenuBar::setDefaultStyle()
{
    // for (Menu* menu : m_Menus)
    // {
    //     for (Component* component : menu->getMenuItems()->getComponents())
    //     {
    //         if (component->getSkin() == nullptr)
    //         {
    //             component->setSkin(&DEFAULT_SKIN_MENU_ITEM);
    //             component->attachHandler({MouseEnteredEvent::getStaticType(),
    //                                       MouseExitedEvent::getStaticType()},
    //                                      new HoverListener{component, &DEFAULT_HOVER_STYLE_MENU_ITEM});
    //         }
    //     }
    // }
}

//! FIXME: 
std::list<Component*>::iterator MenuBar::getFirstComponent()
{
    auto it = m_Components.begin();

    for (int32_t i = 0; i < m_Menus.size(); ++i)
    {
        ++it;
    }

    return it;
}