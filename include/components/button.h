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

class Button : public Component, public Skinnable<Button>
{
public:
    // FIXME: Make private and emit ActionEvents to a queue of gui events 
    ActionListener* actionListener = nullptr;

    Button(Renderer* renderer, const Font& font, const char* label = nullptr);
    virtual ~Button() override = default;

    Label* getLabel();
    void setLabel(const char* label);

    /* Component */
    virtual void updateGraphics() override;
    // virtual void render(Texture* target, const Rectangle<int32_t>& targetRegion) override;
    virtual int32_t getPrefWidth() const override;
    virtual int32_t getPrefHeight() const override;

protected:
    Label      m_Label;
    ButtonSkin m_DefaultSkin;
};

#endif // BUTTON_H