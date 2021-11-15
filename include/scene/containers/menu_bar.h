/**
 * @author Nikita Mochalov (github.com/tralf-strues)s
 * @file menu_bar.h
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "../controls/button.h"
#include "context_menu.h"

namespace Sgl
{
    class Menu
    {
    public:
        Menu(Scene* scene, const char* title);
        ~Menu();

        const char* getTitle() const;
        Button* getTitleButton();
        ContextMenu* getContextMenu();

    private:
        Button*      m_TitleButton = nullptr;
        ContextMenu* m_ContextMenu = nullptr;
    };

    class MenuBar : public HBox
    {
    public:
        static const Background DEFAULT_BACKGROUND;
        static const Border     DEFAULT_BORDER;

    public:
        MenuBar(Scene* scene);
        virtual ~MenuBar() override;

        Menu* addMenu(const char* title);
        Menu* getMenu(const char* title);

        void showMenu(Menu* menu);
        void hideMenu(Menu* menu);

    private:
        std::list<Menu*> m_Menus;
    };
}