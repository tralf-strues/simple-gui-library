// //------------------------------------------------------------------------------
// //! @author Nikita Mochalov (github.com/tralf-strues)
// //! @file gui_event.h
// //! @date 2021-10-23
// //! 
// //! @copyright Copyright (c) 2021
// //------------------------------------------------------------------------------

// #ifndef GUI_EVENT_H
// #define GUI_EVENT_H

// #include "sml/events/event.h"

// class EventTarget;

// struct GuiEvent : public Event
// {
//     EventTarget* target; ///< End component in the event dispatch chain.

//     GuiEvent(EventTarget* target, const Event& event);

//     GuiEvent(EventTarget* target, 
//              EventType type = INVALID_EVENT_TYPE,
//              EventCategory category = EVENT_CATEGORY_ANY)
//         : Event(type, category), target(target) {}
// };

// #endif // GUI_EVENT_H