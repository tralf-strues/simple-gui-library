/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file scroll_pane.cpp
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/controls/scroll_pane.h"
#include "scene/style/default_skins.h"

using namespace Sgl;

ScrollPane::ScrollPane()
{
    m_DefaultSkin = new DefaultSkins::ScrollPaneSkin(this);
    setSkin(m_DefaultSkin);
}

Component* ScrollPane::getContent() { return m_Content; }
void ScrollPane::setContent(Component* content) { assert(content); m_Content = content; }

const Sml::Rectangle<int32_t>& ScrollPane::getViewport() const { return m_Viewport; }
void ScrollPane::setViewport(const Sml::Rectangle<int32_t>& viewport) { m_Viewport = viewport; }

int32_t ScrollPane::getViewportX() const { return m_Viewport.pos.x; }
void ScrollPane::setViewportX(int32_t x) { m_Viewport.pos.x = x; }

int32_t ScrollPane::getViewportY() const { return m_Viewport.pos.y; }
void ScrollPane::setViewportY(int32_t y) { m_Viewport.pos.y = y; }

int32_t ScrollPane::getViewportWidth() const { return m_Viewport.width; }
void ScrollPane::setViewportWidth(int32_t width) { m_Viewport.width = width; }

int32_t ScrollPane::getViewportHeight() const { return m_Viewport.height; }
void ScrollPane::setViewportHeight(int32_t height) { m_Viewport.height = height; }