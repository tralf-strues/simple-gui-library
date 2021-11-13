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

        void showMenu(const char* title);
        void hideMenu(const char* title);

        // virtual Component* getHitComponent(int32_t x, int32_t y) override;
        // virtual void updateGraphics() override;
        // virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;

    private:
        std::list<Menu*> m_Menus;
    };
}