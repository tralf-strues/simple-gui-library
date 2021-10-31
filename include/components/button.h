//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.h
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef BUTTON_H
#define BUTTON_H

#include "style/skin.h"
#include "label.h"

class Button : public Component, public Skinable
{
public:
    Button(Renderer* renderer, const Font& font, Color background = COLOR_WHITE);
    Button(Renderer* renderer, const Font& font, const char* label,
           Color foreground = COLOR_BLACK, Color background = COLOR_WHITE);

    const char* getLabel() const;
    void setLabel(const char* label);

    /* Component */
    virtual void updateGraphics() override;
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;

protected:
    Label m_Label;

    /* Skinable */
    virtual void applySkin() override;
};

#endif // BUTTON_H