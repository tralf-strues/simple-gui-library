/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file context_menu.h
 * @date 2021-11-10
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "box_container.h"

namespace Sgl
{
    class Scene;
    class Button;

    class MenuItem : public Button
    {
    public:
        MenuItem(const char* label);
    };

    class ContextMenu : public VBox
    {
    public:
        static const Border              DEFAULT_BORDER;
        static const ColorFill           DEFAULT_BACKGROUND_FILL;
        static const Background          DEFAULT_BACKGROUND;
        static const ShadowSpecification DEFAULT_SHADOW;

    public:
        ContextMenu(Scene* scene, Component* sourceComponent = nullptr);

        virtual void render(const Sml::Rectangle<int32_t>& targetRegion) override;

        Component* getSource();
        void setSource(Component* source);

        void show();
        void hide();

    private:
        Component* m_Source = nullptr;
    };
}