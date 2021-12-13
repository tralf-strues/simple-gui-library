/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scroll_pane.h
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "control.h"

namespace Sgl
{
    class ScrollPane : public Control
    {
    public:
        ScrollPane();

        Component* getContent();
        void setContent(Component* content);

        const Sml::Rectangle<int32_t>& getViewport() const;
        void setViewport(const Sml::Rectangle<int32_t>& viewport);

        int32_t getViewportX() const;
        void setViewportX(int32_t x);

        int32_t getViewportY() const;
        void setViewportY(int32_t y);

        int32_t getViewportWidth() const;
        void setViewportWidth(int32_t width);

        int32_t getViewportHeight() const;
        void setViewportHeight(int32_t height);

    private:
        Component*              m_Content = nullptr;
        Sml::Rectangle<int32_t> m_Viewport;
    };
};