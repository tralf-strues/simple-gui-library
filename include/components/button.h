//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.h
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef BUTTON_H
#define BUTTON_H

#include "label.h"

class Button : public Component
{
public:
    Button(Renderer* renderer, const Font& font, Color background = COLOR_WHITE);
    Button(Renderer* renderer, const Font& font, const char* label,
           Color foreground = COLOR_BLACK, Color background = COLOR_WHITE);

    /* Component */
    const char* getLabel() const;
    void setLabel(const char* label);

private:
    Label m_Label;
};

#endif // BUTTON_H