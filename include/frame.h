//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file frame.h
//! @date 2021-10-18
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef FRAME_H
#define FRAME_H

#include "sml/events/system_event_manager.h"
#include "container.h"

class Frame : public Container
{
public:
    static const int32_t DEFAULT_WIDTH  = 640;
    static const int32_t DEFAULT_HEIGHT = 360;

    Frame(int32_t width = DEFAULT_WIDTH,
          int32_t height = DEFAULT_HEIGHT,
          const char* title = nullptr);
    virtual ~Frame() override = default;

    void proccessSystemEvents();
    void show();

    Window* getWindow();

protected:
    Window             m_Window;
    SystemEventManager m_SystemEventManager;
    
    Component*         m_PrevMouseHoverComponent = nullptr;

    void proccessWindowEvent(WindowEvent* windowEvent);
    void proccessKeyboardEvent(KeyEvent* keyEvent);
    void proccessMouseEvent(MouseEvent* mouseEvent);
};

#endif // FRAME_H