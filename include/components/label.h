//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file label.h
//! @date 2021-10-20
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef LABEL_H
#define LABEL_H

#include "component.h"

class Label : public Component
{
public:
    Label() {}
    Label(Renderer* renderer, const Font& font, Color foreground = COLOR_BLACK);
    Label(Renderer* renderer, const Font& font, const char* str, Color foreground = COLOR_BLACK);
    virtual ~Label() override = default;

    void setText(const char* str);
    const char* getText() const;
    bool isEmpty() const;

    /* Component */
    virtual void updateGraphics() override;
    virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;
    virtual int32_t getPrefWidth() const override;
    virtual int32_t getPrefHeight() const override;

private:
    Text m_Text;
};

#endif // LABEL_H