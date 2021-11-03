//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file menu_bar.h
//! @date 2021-10-31
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef MENU_BAR_H
#define MENU_BAR_H

#include "../components/button.h"
#include "list_menu.h"
#include "spaced_container.h"

class Menu
{
public:
    Menu() {}
    Menu(Renderer* renderer, const Font& font, const char* title);
    ~Menu();

    const char* getTitle() const;
    Button* getTitleButton();
    ListMenu* getMenuItems();

private:
    Button*   m_TitleButton = nullptr;
    ListMenu* m_MenuItems   = nullptr;
};

class MenuBar : public SpacedContainer
{
public:
    MenuBar(Renderer* renderer, const Font& font);
    virtual ~MenuBar() override;

    Menu* addMenu(const char* title);
    Menu* getMenu(const char* title);

    void showMenu(const char* title);
    void hideMenu(const char* title);

    virtual Component* getHitComponent(int32_t x, int32_t y) override;
    virtual void updateGraphics() override;
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;

private:
    const Font*      m_Font;
    std::list<Menu*> m_Menus;

    void setDefaultStyle();
    std::list<Component*>::iterator getFirstComponent();
};

#endif // MENU_BAR_H