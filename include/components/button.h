//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file button.h
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef BUTTON_H
#define BUTTON_H

#include "style/default_skins.h"
#include "label.h"

static const Vec2<int32_t> BUTTON_DEFAULT_MIN_MARGINS = {32, 0};

class Button : public Component
{
public:
    Button(Renderer* renderer, const Font& font, Color background = COLOR_WHITE);
    Button(Renderer* renderer, const Font& font, const char* label,
           Color foreground = COLOR_BLACK, Color background = COLOR_WHITE);
    virtual ~Button() override = default;

    Label* getLabel() const;
    void setLabel(const char* label);

    /* Component */
    virtual void updateGraphics() override;
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;

protected:
    Label m_Label;

    virtual void setDefaultStyle() override;
};

#endif // BUTTON_H