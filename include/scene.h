//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file scene.h
//! @date 2021-10-23
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include "sml/graphics_wrapper/color.h"
#include "events/gui_event_dispatching.h"
#include "container.h"

#ifndef SCENE_H
#define SCENE_H

class Scene : GuiEventTarget
{
public:
    

private:
    Renderer*  m_Renderer      = nullptr;
    Container* m_RootContainer = nullptr;
    Color      m_FillColor     = COLOR_WHITE;
};

#endif // SCENE_H